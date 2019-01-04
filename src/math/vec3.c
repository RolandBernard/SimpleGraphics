
#include "vec3.h"

vec3_t vec3(scalar_t x, scalar_t y, scalar_t z) {
	vec3_t ret = {
		.xyz = { x, y, z }
	};
	return ret;
}

vec3_t vec3_scale(scalar_t s, vec3_t v) {
	vec3_t ret = {
		.xyz = { v.xyz[0]*s, v.xyz[1]*s, v.xyz[2]*s }
	};
	return ret;
}

vec3_t vec3_add(vec3_t v, vec3_t u) {
	vec3_t ret = {
		.xyz = { v.xyz[0]+u.xyz[0], v.xyz[1]+u.xyz[1], v.xyz[2]+u.xyz[2] }
	};
	return ret;
}

vec3_t vec3_reflect(vec3_t v, vec3_t n) {
	vec3_t nnorm = vec3_norm(n);
	return vec3_add(v, vec3_scale(-2*vec3_dot(v, nnorm), nnorm));
}

vec3_t vec3_cross(vec3_t v, vec3_t u) {
	vec3_t ret = {
		.xyz = {
			v.xyz[1]*u.xyz[2] - v.xyz[2]*u.xyz[1],
			v.xyz[2]*u.xyz[0] - v.xyz[0]*u.xyz[2],
			v.xyz[0]*u.xyz[1] - v.xyz[1]*u.xyz[0]
		}
	};
	return ret;
}

vec3_t vec3_norm(vec3_t v) {
	return vec3_scale(1 / vec3_mag(v), v);
}

scalar_t vec3_mag(vec3_t v) {
	return sqrtf(sqr(v.xyz[0]) + sqr(v.xyz[1]) + sqr(v.xyz[2]));
}

scalar_t vec3_dot(vec3_t v, vec3_t u) {
	return v.xyz[0]*u.xyz[0] + v.xyz[1]*u.xyz[1] + v.xyz[2]*u.xyz[2];
}

