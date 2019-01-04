// Copyright (c) 2019 Roland Bernard
 

#include "mat2.h"

vec2_t mat2_vec2_mul(mat2_t m, vec2_t v) {
	vec2_t ret = {
		.xy = { m.d[0][0]*v.xy[0] + m.d[0][1]*v.xy[1], m.d[1][0]*v.xy[0] + m.d[1][1]*v.xy[1] }
	};
	return ret;
}

mat2_t mat2_scale(scalar_t s, mat2_t m) {
	mat2_t ret = {
		.d = {
			{ m.d[0][0]*s, m.d[0][1]*s },
			{ m.d[1][0]*s, m.d[1][1]*s }
		}
	};
	return ret;
}

mat2_t mat2_add(mat2_t m, mat2_t n) {
	mat2_t ret = {
		.d = {
			{ m.d[0][0]+n.d[0][0], m.d[0][1]+n.d[0][1] },
			{ m.d[1][0]+n.d[1][0], m.d[1][1]+n.d[1][1] }
		}
	};
	return ret;
}

mat2_t mat2_mul(mat2_t m, mat2_t n) {
	mat2_t ret = {
		.d = {
			{ m.d[0][0]*n.d[0][0] + m.d[0][1]*n.d[1][0], m.d[0][0]*n.d[0][1] + m.d[0][1]*n.d[1][1] },
			{ m.d[1][0]*n.d[0][0] + m.d[1][1]*n.d[1][0], m.d[1][0]*n.d[0][1] + m.d[1][1]*n.d[1][1] }
		}
	};
	return ret;
}

mat2_t mat2_adj(mat2_t m) {
	return mat2_trans(mat2_com(m));
}

mat2_t mat2_trans(mat2_t m) {
	mat2_t ret = {
		.d = {
			{ m.d[0][0], m.d[1][0] },
			{ m.d[0][1], m.d[1][1] }
		}
	};
	return ret;
}

mat2_t mat2_inv(mat2_t m) {
	return mat2_scale(1 / mat2_det(m), mat2_adj(m));
}

mat2_t mat2_com(mat2_t m) {
	mat2_t ret = {
		.d = {
			{ m.d[1][1], m.d[1][0] },
			{ m.d[0][1], m.d[0][0] }
		}
	};
	return ret;
}

scalar_t mat2_det(mat2_t m) {
	return m.d[0][0]*m.d[1][1] - m.d[0][1]*m.d[1][0];
}

