// Copyright (c) 2019 Roland Bernard
 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "math/math.h"
#include "math/transform.h"

#include "gx.h"
#include "mesh/monkey.h"

#define max(x, y) (x > y ? x : y)
#define abs(x) (x > 0 ? x : -x)

typedef struct {
	// Vert
	mat4_t mvp;
	mat4_t mv;
	mat4_t v;
	const float (*pos)[3];
	const float (*uv)[2];
	const float (*nor)[3];
	const unsigned short (*ind);
	unsigned short num_ind;
} udata_t;

void vshader(void* udata, int vid, gx_attr_t* out) {
	udata_t* d = (udata_t*)udata;
	unsigned int ind = d->ind[vid%(d->num_ind)];
	vec4_t pos = vec4(.5*d->pos[ind][0], .5*d->pos[ind][1], .5*d->pos[ind][2], 1.0);
	out->pos = mat4_vec4_mul(d->mvp, pos);
	vec4_t norm = vec4_norm(mat4_vec4_mul(d->mv, vec4(d->nor[ind][0], d->nor[ind][1], d->nor[ind][2], 0.0)));
	out->data[0] = norm.xyzw[0];
	out->data[1] = norm.xyzw[1];
	out->data[2] = norm.xyzw[2];
	vec4_t l = vec4_norm(mat4_vec4_mul(d->v, vec4(1,0,0,0)));
	vec4_t e = vec4_norm(vec4_scale(-1, mat4_vec4_mul(d->mv, pos)));
	out->data[3] = l.xyzw[0];
	out->data[4] = l.xyzw[1];
	out->data[5] = l.xyzw[2];
	out->data[6] = e.xyzw[0];
	out->data[7] = e.xyzw[1];
	out->data[8] = e.xyzw[2];
}

void fshader(void* udata, gx_attr_t* in, gx_color_t* color) {
	vec3_t n = vec3(in->data[0], in->data[1], in->data[2]);
	vec3_t l = vec3(in->data[3], in->data[4], in->data[5]);
	vec3_t e = vec3(in->data[6], in->data[7], in->data[8]);
	vec3_t negl = vec3_scale(-1, l);
	scalar_t c = vec3_dot(l, n);
	scalar_t a;
	if(c < 0) {
		c = 0;
		a = 0;
	} else {
		a = vec3_dot(e, vec3_reflect(negl, n));
		if(a < 0)
			a = 0;
		else {
			a *= a;
			a *= a;
			a *= a;
		}
	}
	scalar_t h = c*0.4+a*0.57+0.03;
	color[0][0] = h;
	color[0][1] = h;
	color[0][2] = h;
}

void planevshder(void* udata, int vid, gx_attr_t* out) {
	static const float p[18][3] = {
		{ -1.0, 0.0, -1.0 },
		{ 1.0, 0.0, 1.0 },
		{ 1.0, 0.0, -1.0 },
		{ -1.0, 0.0, -1.0 },
		{ -1.0, 0.0, 1.0 },
		{ 1.0, 0.0, 1.0 },
		{ 0.0, -1.0, -1.0 },
		{ 0.0, 1.0, 1.0 },
		{ 0.0, 1.0, -1.0 },
		{ 0.0, -1.0, -1.0 },
		{ 0.0, -1.0, 1.0 },
		{ 0.0, 1.0, 1.0 },
		{ -1.0, -1.0, 0.0 },
		{ 1.0, 1.0, 0.0 },
		{ 1.0, -1.0, 0.0 },
		{ -1.0, -1.0, 0.0 },
		{ -1.0, 1.0, 0.0 },
		{ 1.0, 1.0, 0.0 },
	};
	static const float t[6][2] = {
		{ 0.0, 0.0 },
		{ 1.0, 1.0 },
		{ 1.0, 0.0 },
		{ 0.0, 0.0 },
		{ 0.0, 1.0 },
		{ 1.0, 1.0 }
	};
	mat4_t* d = (mat4_t*)udata;
	if(vid/18 != 1) {
		out->pos = mat4_vec4_mul(*d, vec4(p[vid%18][0], p[vid%18][1], p[vid%18][2], 1.0));
		out->data[0] = t[vid%6][0];
		out->data[1] = t[vid%6][1];
	} else {
		out->pos = mat4_vec4_mul(*d, vec4(p[(35-vid)%18][0], p[(35-vid)%18][1], p[(35-vid)%18][2], 1.0));
		out->data[0] = t[(35-vid)%6][0];
		out->data[1] = t[(35-vid)%6][1];
	}
}

void planefshader(void* udata, gx_attr_t* in, gx_color_t* color) {
	short on = ((int)(in->data[0]*20)%2 != (int)(in->data[1]*20)%2);
	color[0][0] = (on ? 0.0 : 0.1);
	color[0][1] = (on ? 0.0 : 0.1);
	color[0][2] = (on ? 0.0 : 0.1);
}

static int width = 500;
static int height = 500;

int main(void) {

	Display* display = XOpenDisplay(NULL);
	int screen_num = DefaultScreen(display);
	Window root = RootWindow(display, screen_num);
	Visual* visual = DefaultVisual(display, screen_num);

	unsigned char (*fdata)[4] = (unsigned char(*)[4])malloc(sizeof(unsigned char[4])*width*height);

	XImage* img = XCreateImage(display, visual, DefaultDepth(display, screen_num), ZPixmap, 0, (char*)fdata, width, height, 32, 0);

	Window win = XCreateSimpleWindow(display, root, 50, 50, width, height, 1, 0, 0);
	XSelectInput(display, win, KeyPressMask | KeyReleaseMask | StructureNotifyMask);
	XMapWindow(display, win);

	Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(display, win, &wmDeleteMessage, 1);

	udata_t data;
	data.pos = (const float(*)[3])suzanneVertices;
	data.uv = (const float(*)[2])suzanneTexCoords;
	data.nor = (const float(*)[3])suzanneNormals;
	data.ind = suzanneIndices;
	data.num_ind = NUM_SUZANNE_OBJECT_INDEX;

	gx_render_target_t* target = gx_create_render_target(width, height);

	gx_near_plane = 0.05;

	int run = 0;
	clock_t avg = 0;
	int running = 1;
	float angle = 0;
	XEvent event;
	do {
		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		run++;
		while(XPending(display)) {
			XNextEvent(display, &event);
			if(event.xclient.data.l[0] == wmDeleteMessage) {
				running = 0;
			} else if(event.type == ConfigureNotify) {
				XConfigureEvent xce = event.xconfigure;

				if(width != xce.width || height != xce.height) {
					width = xce.width;
					height = xce.height;
					XDestroyImage(img);
					fdata = (unsigned char(*)[4])malloc(sizeof(unsigned char[4])*width*height);
					img = XCreateImage(display, visual, DefaultDepth(display, screen_num), ZPixmap, 0, (char*)fdata, width, height, 32, 0);
					gx_free_render_target(target);
					target = gx_create_render_target(width, height);
				}
			}
		}

		angle += 0.04;

		scalar_t z = sinf(angle)*2+3;//(cosf(angle))*3*(1.0+sinf(angle/12));
		scalar_t x = cosf(angle)*2+3;//(sinf(angle))*3*(1.0+sinf(angle/12));
		scalar_t y = 5;//*(1.0+cos(angle/12));

		//computeMatricesFromInputs();
		mat4_t proj = mat4_proj_trans((float)width/(float)height, 0.1, 300.0, 1.0);
		mat4_t view = mat4_view_trans(vec3(x, y, z), vec3(3,3,3), vec3(0,1,0));
		mat4_t model;
		for(int i = 0; i < 16; i++) {
			model.d[i%4][i/4] = suzanneTransform[i];
		}

		model = mat4_mul(mat4_translate_trans(vec3(3,3,3)),
				mat4_mul(mat3_to_mat4(mat3_mul(mat3_rotation_trans(vec3(0,1,0), angle/13),
					mat3_mul(mat3_rotation_trans(vec3(1,0,0), angle/7+1),
						mat3_scale_trans(0.05)))), model));

		data.v = view;
		data.mv = mat4_mul(view, model);
		data.mvp = mat4_mul(proj, data.mv);

		//mat4_t planemvp = mat4_mul(proj, mat4_mul(view, mat3_to_mat4(mat3_scale_trans(6.0))));

		// Rendering
		gx_color_t black = { 0.0, 0.0, 0.0 };
		gx_clear_color(target, black);
		gx_clear_depth(target, 1.0);
		gx_render(target, &data, 9, data.num_ind/3, vshader, fshader);
		//gx_render(target, &planemvp, 2, 12, planevshder, planefshader);

		// Drawing to screen
		for(int i = 0; i < width*height; i++) {
				fdata[i][2] = (unsigned char)(int)((target->cbuffer[i][0])*255.0);
				fdata[i][1] = (unsigned char)(int)((target->cbuffer[i][1])*255.0);
				fdata[i][0] = (unsigned char)(int)((target->cbuffer[i][2])*255.0);
				fdata[i][3] = 255;
		}
		

		clock_gettime(CLOCK_MONOTONIC, &end);
		int t = (end.tv_sec - start.tv_sec) * 1000000;
		t += (end.tv_nsec - start.tv_nsec) / 1000;
		avg = avg + (t-avg)/run;
		if(run >= 10) {
			fprintf(stderr, "Render time: %g\n", avg/1000000.0);
			run = 0;
			avg = 0;
		}

		XPutImage(display, win, DefaultGC(display, screen_num), img, 0, 0, 0, 0, width, height);
		if(t > 20000)
			while(t > 20000) {
				angle += 0.04;
				t -= 20000;
			}
		usleep(20000-t);
	} while (running);

	gx_free_render_target(target);

	XDestroyImage(img);
	XDestroyWindow(display, win);
	XCloseDisplay(display);

	return 0;
}
