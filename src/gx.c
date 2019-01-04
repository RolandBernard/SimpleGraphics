
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "gx.h"

#define NUM_RENDER_THREADS 12
#define PROC_UNIT 500

scalar_t gx_near_plane = 1e-2;

void clear_color(gx_render_target_t* target, gx_color_t color) {
	for(int i = 0; i < target->width*target->height; i++) {
		target->cbuffer[i][0] = color[0];
		target->cbuffer[i][1] = color[1];
		target->cbuffer[i][2] = color[2];
	}
}

void clear_depth(gx_render_target_t* target, scalar_t d) {
	for(int i = 0; i < target->width*target->height; i++)
		target->dbuffer[i] = d;
}

scalar_t edge_function(vec2_t v0, vec2_t v1, vec2_t v2) {
	return (v2.xy[0] - v0.xy[0])*(v1.xy[1] - v0.xy[1]) - (v2.xy[1] - v0.xy[1])*(v1.xy[0] - v0.xy[0]);
}

typedef struct {
	gx_render_target_t* target;
	void* sudata;
	int attr_size;
	int num_tri;
	pthread_mutex_t* progress_lock;
	int progress;
	vert_shader_t vshader;
	frag_shader_t fshader;
} gx_render_udata_t;

static void* render_thread(void* data) {
	gx_render_udata_t* d = (gx_render_udata_t*)data;
	int width = d->target->width;
	int height = d->target->height;
	vert_shader_t vshader = d->vshader;
	frag_shader_t fshader = d->fshader;
	void* udata = d->sudata;
	int attr_size = d->attr_size;
	scalar_t* dbuffer = d->target->dbuffer;
	gx_color_t* cbuffer = d->target->cbuffer;
	int num_tri = d->num_tri;
	gx_attr_t* v[4];
	vec2_t vp[4];
	for(int j = 0; j < 4; j++)
		v[j] = (gx_attr_t*)malloc(offsetof(gx_attr_t, data) + sizeof(scalar_t)*attr_size);
	gx_attr_t* f = (gx_attr_t*)malloc(offsetof(gx_attr_t, data) + sizeof(scalar_t)*attr_size);

	pthread_mutex_t* progress_lock = d->progress_lock;
	int pu = (num_tri/PROC_UNIT)+1;

	// For every triangle
	while(1) {
		int t; // Triangle index
	
		pthread_mutex_lock(progress_lock);
		t = d->progress;
		d->progress+=pu;
		pthread_mutex_unlock(progress_lock);

		if(t >= num_tri)
			break;

		for(int i = t; i < t+pu && i < num_tri; i++) {
			short draw_mode = 3;
			short num_neg = 0;
			for(int j = 0; j < 3; j++) {
				vshader(udata, 3*i+j, v[j]);
				if(v[j]->pos.xyzw[3] < gx_near_plane)
					num_neg++;
			}

			if(num_neg == 3) {
				continue; // Don't draw at all
			} else if(num_neg == 2) {
				short n = (v[0]->pos.xyzw[3] > gx_near_plane ? 0 : (v[1]->pos.xyzw[3] > gx_near_plane ? 1 : 2));
				// Colculate thw two new pints
				for(short l = 1; l <= 2; l++) {
					short j = (n+l)%3;
					scalar_t w0 = (gx_near_plane - v[j]->pos.xyzw[3]) / (v[n]->pos.xyzw[3] -  v[j]->pos.xyzw[3]);
					scalar_t w1 = (1.0-w0);
					v[j]->pos.xyzw[0] = w0*v[n]->pos.xyzw[0] + w1*v[j]->pos.xyzw[0];
					v[j]->pos.xyzw[1] = w0*v[n]->pos.xyzw[1] + w1*v[j]->pos.xyzw[1];
					v[j]->pos.xyzw[2] = w0*v[n]->pos.xyzw[2] + w1*v[j]->pos.xyzw[2];
					v[j]->pos.xyzw[3] = w0*v[n]->pos.xyzw[3] + w1*v[j]->pos.xyzw[3];
					for(int k = 0; k < attr_size; k++)
						v[j]->data[k] = w0*v[n]->data[k] + w1*v[j]->data[k];
				}
			} else if(num_neg == 1) {
				short j = (v[0]->pos.xyzw[3] < gx_near_plane ? 0 : (v[1]->pos.xyzw[3] < gx_near_plane ? 1 : 2));
				// Compute the two new points
				for(short l = 1; l <= 2; l++) {
					short n = (j+l)%3;
					short d = (l == 2 ? j : 3);
					scalar_t w0 = (gx_near_plane - v[j]->pos.xyzw[3]) / (v[n]->pos.xyzw[3] - v[j]->pos.xyzw[3]);
					scalar_t w1 = (1.0-w0);
					v[d]->pos.xyzw[0] = w0*v[n]->pos.xyzw[0] + w1*v[j]->pos.xyzw[0];
					v[d]->pos.xyzw[1] = w0*v[n]->pos.xyzw[1] + w1*v[j]->pos.xyzw[1];
					v[d]->pos.xyzw[2] = w0*v[n]->pos.xyzw[2] + w1*v[j]->pos.xyzw[2];
					v[d]->pos.xyzw[3] = w0*v[n]->pos.xyzw[3] + w1*v[j]->pos.xyzw[3];
					for(int k = 0; k < attr_size; k++)
						v[d]->data[k] = w0*v[n]->data[k] + w1*v[j]->data[k];
				}
				// Bring points in same order every time
				gx_attr_t* tmp[3];
				for(short l = 0; l < 3; l++)
					tmp[l] = v[l];
				v[0] = tmp[(j+1)%3];
				v[1] = tmp[(j+2)%3];
				v[2] = tmp[j];
				draw_mode = 4;
			}

			if(draw_mode == 3) {
				scalar_t minxf = width;
				scalar_t minyf = height;
				scalar_t minz = 1.0;
				scalar_t maxxf = 0;
				scalar_t maxyf = 0;
				scalar_t maxz = -1.0;

				// For all 3 Vert
				for(int j = 0; j < 3; j++) {
					scalar_t winv = 1 / v[j]->pos.xyzw[3];

					v[j]->pos.xyzw[2] *= winv;
					v[j]->pos.xyzw[0] *= winv;
					v[j]->pos.xyzw[1] *= winv;
					v[j]->pos.xyzw[3] = winv;

					vp[j].xy[0] = (1 + v[j]->pos.xyzw[0])*.5*width;
					vp[j].xy[1] = (1 + v[j]->pos.xyzw[1])*.5*height;

					for(int k = 0; k < attr_size; k++)
						v[j]->data[k] *= winv;
				
					// Find bounding box
					if(vp[j].xy[0] < minxf)
						minxf = (vp[j].xy[0]);
					if(vp[j].xy[0] > maxxf)
						maxxf = (vp[j].xy[0]);
					if(vp[j].xy[1] < minyf)
						minyf = (vp[j].xy[1]);
					if(vp[j].xy[1] > maxyf)
						maxyf = (vp[j].xy[1]);
					if(v[j]->pos.xyzw[2] > maxz)
						maxz = v[j]->pos.xyzw[2];
					if(v[j]->pos.xyzw[2] < minz)
						minz = v[j]->pos.xyzw[2];
				}
				if(minz < 1.0 && maxz > -1.0 && minxf < width && maxxf > 0.0 && minyf < height && maxyf > 0.0) {
					if(maxxf >= width)
						maxxf = width-1.0;
					if(minxf < 0.0)
						minxf = 0.0;
					if(maxyf >= height)
						maxyf = height-1.0;
					if(minyf < 0.0)
						minyf = 0.0;
					short minx = (short)minxf;
					short maxx = (short)(maxxf+1.0);
					short miny = (short)minyf;
					short maxy = (short)(maxyf+1.0);

					// Calculate area of the triangle (2x)
					scalar_t area = 1/edge_function(vp[0], vp[1], vp[2]);

					if(area <= 0) {
						// For every pixel in the bounding box
						for(short x = minx; x < maxx; x++)
							for(short y = miny; y < maxy; y++) {
								vec2_t p = vec2(x + .5, y + .5);
								scalar_t w0;
								scalar_t w1;
								scalar_t w2;
								if(((w0 = edge_function(vp[1], vp[2], p)) < 0 ||
									(w0 == 0 && (vp[1].xy[1] > vp[2].xy[1] || (vp[1].xy[1] == vp[2].xy[1] && vp[1].xy[0] > vp[2].xy[0])))) && 
									((w1 = edge_function(vp[2], vp[0], p)) < 0 ||
									(w1 == 0 && (vp[2].xy[1] > vp[0].xy[1] || (vp[2].xy[1] == vp[0].xy[1] && vp[2].xy[0] > vp[0].xy[0])))) && 
									((w2 = edge_function(vp[0], vp[1], p)) < 0 ||
									(w2 == 0 && (vp[0].xy[1] > vp[1].xy[1] || (vp[0].xy[1] == vp[1].xy[1] && vp[0].xy[0] > vp[1].xy[0])))) ) {
									w0 *= area;
									w1 *= area;
									w2 *= area;
	
									scalar_t z = (v[0]->pos.xyzw[2]*w0 + v[1]->pos.xyzw[2]*w1 + v[2]->pos.xyzw[2]*w2);
	
									if(z > -1.0 && z < 1.0) {
										int buffer_index = (height - y - 1)*width + x;
										scalar_t* dloc = dbuffer + buffer_index;
										// Check Z-buffer
										if(z < *dloc) {
											*dloc = z;
	
											scalar_t w = 1 / (v[0]->pos.xyzw[3]*w0 + v[1]->pos.xyzw[3]*w1 + v[2]->pos.xyzw[3]*w2);
											f->pos.xyzw[0] = w*(v[0]->pos.xyzw[0]*w0 + v[1]->pos.xyzw[0]*w1 + v[2]->pos.xyzw[0]*w2);
											f->pos.xyzw[1] = w*(v[0]->pos.xyzw[1]*w0 + v[1]->pos.xyzw[1]*w1 + v[2]->pos.xyzw[1]*w2);
											f->pos.xyzw[2] = z*w;
											f->pos.xyzw[3] = w;
	
											for(int k = 0; k < attr_size; k++)
												f->data[k] =  w*(v[0]->data[k]*w0 + v[1]->data[k]*w1 + v[2]->data[k]*w2);
	
											fshader(udata, f, cbuffer + buffer_index);
										}
									}
								}
							}
					}
				}
			} else /*if(draw_mode == 4)*/ {
				scalar_t minxf = width;
				scalar_t minyf = height;
				scalar_t minz = 1.0;
				scalar_t maxxf = 0;
				scalar_t maxyf = 0;
				scalar_t maxz = -1.0;

				// For all 4 Vert
				for(int j = 0; j < 4; j++) {
					scalar_t winv = 1 / v[j]->pos.xyzw[3];

					v[j]->pos.xyzw[2] *= winv;
					v[j]->pos.xyzw[0] *= winv;
					v[j]->pos.xyzw[1] *= winv;
					v[j]->pos.xyzw[3] = winv;

					vp[j].xy[0] = (1 + v[j]->pos.xyzw[0])*.5*width;
					vp[j].xy[1] = (1 + v[j]->pos.xyzw[1])*.5*height;

					for(int k = 0; k < attr_size; k++)
						v[j]->data[k] *= winv;
				
					// Find bounding box
					if(vp[j].xy[0] < minxf)
						minxf = (vp[j].xy[0]);
					if(vp[j].xy[0] > maxxf)
						maxxf = (vp[j].xy[0]);
					if(vp[j].xy[1] < minyf)
						minyf = (vp[j].xy[1]);
					if(vp[j].xy[1] > maxyf)
						maxyf = (vp[j].xy[1]);
					if(v[j]->pos.xyzw[2] > maxz)
						maxz = v[j]->pos.xyzw[2];
					if(v[j]->pos.xyzw[2] < minz)
						minz = v[j]->pos.xyzw[2];
				}
				if(minz < 1.0 && maxz > -1.0 && minxf < width && maxxf > 0.0 && minyf < height && maxyf > 0.0) {
					if(maxxf >= width)
						maxxf = width-1.0;
					if(minxf < 0.0)
						minxf = 0.0;
					if(maxyf >= height)
						maxyf = height-1.0;
					if(minyf < 0.0)
						minyf = 0.0;
					short minx = (short)minxf;
					short maxx = (short)(maxxf+1.0);
					short miny = (short)minyf;
					short maxy = (short)(maxyf+1.0);

					// Calculate area of the triangles (2x)
					scalar_t area0 = 1/edge_function(vp[0], vp[1], vp[2]);
					scalar_t area1 = 1/edge_function(vp[0], vp[2], vp[3]);

					if(area0 <= 0 /*|| area1 <= 0*/) { // If area0 is <= 0, area1 has to be?
						// For every pixel in the bounding box
						for(short x = minx; x < maxx; x++)
							for(short y = miny; y < maxy; y++) {
								vec2_t p = vec2(x + .5, y + .5);
								scalar_t w0;
								scalar_t w1;
								scalar_t w2;
								if(((w0 = edge_function(vp[1], vp[2], p)) < 0 ||
									(w0 == 0 && (vp[1].xy[1] > vp[2].xy[1] || (vp[1].xy[1] == vp[2].xy[1] && vp[1].xy[0] > vp[2].xy[0])))) && 
									((w1 = edge_function(vp[2], vp[0], p)) < 0 ||
									(w1 == 0 && (vp[2].xy[1] > vp[0].xy[1] || (vp[2].xy[1] == vp[0].xy[1] && vp[2].xy[0] > vp[0].xy[0])))) && 
									((w2 = edge_function(vp[0], vp[1], p)) < 0 ||
									(w2 == 0 && (vp[0].xy[1] > vp[1].xy[1] || (vp[0].xy[1] == vp[1].xy[1] && vp[0].xy[0] > vp[1].xy[0])))) ) {
									w0 *= area0;
									w1 *= area0;
									w2 *= area0;
	
									scalar_t z = (v[0]->pos.xyzw[2]*w0 + v[1]->pos.xyzw[2]*w1 + v[2]->pos.xyzw[2]*w2);
	
									if(z > -1.0 && z < 1.0) {
										int buffer_index = (height - y - 1)*width + x;
										scalar_t* dloc = dbuffer + buffer_index;
										// Check Z-buffer
										if(z < *dloc) {
											*dloc = z;
	
											scalar_t w = 1 / (v[0]->pos.xyzw[3]*w0 + v[1]->pos.xyzw[3]*w1 + v[2]->pos.xyzw[3]*w2);
											f->pos.xyzw[0] = w*(v[0]->pos.xyzw[0]*w0 + v[1]->pos.xyzw[0]*w1 + v[2]->pos.xyzw[0]*w2);
											f->pos.xyzw[1] = w*(v[0]->pos.xyzw[1]*w0 + v[1]->pos.xyzw[1]*w1 + v[2]->pos.xyzw[1]*w2);
											f->pos.xyzw[2] = z*w;
											f->pos.xyzw[3] = w;
	
											for(int k = 0; k < attr_size; k++)
												f->data[k] =  w*(v[0]->data[k]*w0 + v[1]->data[k]*w1 + v[2]->data[k]*w2);
	
											fshader(udata, f, cbuffer + buffer_index);
										}
									}
								} else if(
									((w0 = edge_function(vp[2], vp[3], p)) < 0 ||
									(w0 == 0 && (vp[2].xy[1] > vp[3].xy[1] || (vp[2].xy[1] == vp[3].xy[1] && vp[2].xy[0] > vp[3].xy[0])))) && 
									((w1 = edge_function(vp[3], vp[0], p)) < 0 ||
									(w1 == 0 && (vp[3].xy[1] > vp[0].xy[1] || (vp[3].xy[1] == vp[0].xy[1] && vp[3].xy[0] > vp[0].xy[0])))) && 
									((w2 = edge_function(vp[0], vp[2], p)) < 0 ||
									(w2 == 0 && (vp[0].xy[1] > vp[2].xy[1] || (vp[0].xy[1] == vp[2].xy[1] && vp[0].xy[0] > vp[2].xy[0])))) ) {
									w0 *= area1;
									w1 *= area1;
									w2 *= area1;
	
									scalar_t z = (v[0]->pos.xyzw[2]*w0 + v[2]->pos.xyzw[2]*w1 + v[3]->pos.xyzw[2]*w2);
	
									if(z > -1.0 && z < 1.0) {
										int buffer_index = (height - y - 1)*width + x;
										scalar_t* dloc = dbuffer + buffer_index;
										// Check Z-buffer
										if(z < *dloc) {
											*dloc = z;
	
											scalar_t w = 1 / (v[0]->pos.xyzw[3]*w0 + v[2]->pos.xyzw[3]*w1 + v[3]->pos.xyzw[3]*w2);
											f->pos.xyzw[0] = w*(v[0]->pos.xyzw[0]*w0 + v[2]->pos.xyzw[0]*w1 + v[3]->pos.xyzw[0]*w2);
											f->pos.xyzw[1] = w*(v[0]->pos.xyzw[1]*w0 + v[2]->pos.xyzw[1]*w1 + v[3]->pos.xyzw[1]*w2);
											f->pos.xyzw[2] = z*w;
											f->pos.xyzw[3] = w;
	
											for(int k = 0; k < attr_size; k++)
												f->data[k] =  w*(v[0]->data[k]*w0 + v[2]->data[k]*w1 + v[3]->data[k]*w2);
	
											fshader(udata, f, cbuffer + buffer_index);
										}
									}
									
								}
							}
					}
				}
			}
		}
	}

	free(f);
	for(int j = 0; j < 4; j++)
		free(v[j]);

	return NULL;
}

void render(gx_render_target_t* target, void* udata, int attr_size, int num_tri, vert_shader_t vshader, frag_shader_t fshader) {
	gx_render_udata_t render_data;
	render_data.target = target;
	render_data.sudata = udata;
	render_data.attr_size = attr_size;
	render_data.vshader = vshader;
	render_data.fshader = fshader;
	render_data.num_tri = num_tri;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	render_data.progress_lock = &mutex;
	render_data.progress = 0;

	pthread_t threads[NUM_RENDER_THREADS] = { 0 };
	
	for(int i = 0; i < NUM_RENDER_THREADS; i++) {
		pthread_create(&(threads[i]), NULL, render_thread, (void*)(&render_data));
	}
	for(int i = 0; i < NUM_RENDER_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
}
