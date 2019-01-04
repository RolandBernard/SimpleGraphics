// Copyright (c) 2019 Roland Bernard
 

#include "mat4.h"

#include "mat3.h"

vec4_t mat4_vec4_mul(mat4_t m, vec4_t v) {
	vec4_t ret = {
		.xyzw = {
			m.d[0][0]*v.xyzw[0] + m.d[0][1]*v.xyzw[1] + m.d[0][2]*v.xyzw[2] + m.d[0][3]*v.xyzw[3],
			m.d[1][0]*v.xyzw[0] + m.d[1][1]*v.xyzw[1] + m.d[1][2]*v.xyzw[2] + m.d[1][3]*v.xyzw[3],
			m.d[2][0]*v.xyzw[0] + m.d[2][1]*v.xyzw[1] + m.d[2][2]*v.xyzw[2] + m.d[2][3]*v.xyzw[3],
			m.d[3][0]*v.xyzw[0] + m.d[3][1]*v.xyzw[1] + m.d[3][2]*v.xyzw[2] + m.d[3][3]*v.xyzw[3]
		}
	};
	return ret;
}

mat4_t mat4_scale(scalar_t s, mat4_t m) {
	mat4_t ret = {
		.d = {
			{ m.d[0][0]*s, m.d[0][1]*s, m.d[0][2]*s, m.d[0][3]*s },
			{ m.d[1][0]*s, m.d[1][1]*s, m.d[1][2]*s, m.d[1][3]*s },
			{ m.d[2][0]*s, m.d[2][1]*s, m.d[2][2]*s, m.d[2][3]*s },
			{ m.d[3][0]*s, m.d[3][1]*s, m.d[3][2]*s, m.d[3][3]*s }
		}
	};
	return ret;
}

mat4_t mat4_add(mat4_t m, mat4_t n) {
	mat4_t ret = {
		.d = {
			{ m.d[0][0]+n.d[0][0], m.d[0][1]+n.d[0][1], m.d[0][2]+n.d[0][2], m.d[0][3]+n.d[0][3] },
			{ m.d[1][0]+n.d[1][0], m.d[1][1]+n.d[1][1], m.d[1][2]+n.d[1][2], m.d[1][3]+n.d[1][3] },
			{ m.d[2][0]+n.d[2][0], m.d[2][1]+n.d[2][1], m.d[2][2]+n.d[2][2], m.d[2][3]+n.d[2][3] },
			{ m.d[3][0]+n.d[3][0], m.d[3][1]+n.d[3][1], m.d[3][2]+n.d[3][2], m.d[3][3]+n.d[3][3] }
		}
	};
	return ret;
}

mat4_t mat4_mul(mat4_t m, mat4_t n) {
	mat4_t ret = {
		.d = {
			{
				m.d[0][0]*n.d[0][0] + m.d[0][1]*n.d[1][0] + m.d[0][2]*n.d[2][0] + m.d[0][3]*n.d[3][0],
				m.d[0][0]*n.d[0][1] + m.d[0][1]*n.d[1][1] + m.d[0][2]*n.d[2][1] + m.d[0][3]*n.d[3][1],
				m.d[0][0]*n.d[0][2] + m.d[0][1]*n.d[1][2] + m.d[0][2]*n.d[2][2] + m.d[0][3]*n.d[3][2],
				m.d[0][0]*n.d[0][3] + m.d[0][1]*n.d[1][3] + m.d[0][2]*n.d[2][3] + m.d[0][3]*n.d[3][3]
			},
			{
				m.d[1][0]*n.d[0][0] + m.d[1][1]*n.d[1][0] + m.d[1][2]*n.d[2][0] + m.d[1][3]*n.d[3][0],
				m.d[1][0]*n.d[0][1] + m.d[1][1]*n.d[1][1] + m.d[1][2]*n.d[2][1] + m.d[1][3]*n.d[3][1],
				m.d[1][0]*n.d[0][2] + m.d[1][1]*n.d[1][2] + m.d[1][2]*n.d[2][2] + m.d[1][3]*n.d[3][2],
				m.d[1][0]*n.d[0][3] + m.d[1][1]*n.d[1][3] + m.d[1][2]*n.d[2][3] + m.d[1][3]*n.d[3][3]
			},
			{
				m.d[2][0]*n.d[0][0] + m.d[2][1]*n.d[1][0] + m.d[2][2]*n.d[2][0] + m.d[2][3]*n.d[3][0],
				m.d[2][0]*n.d[0][1] + m.d[2][1]*n.d[1][1] + m.d[2][2]*n.d[2][1] + m.d[2][3]*n.d[3][1],
				m.d[2][0]*n.d[0][2] + m.d[2][1]*n.d[1][2] + m.d[2][2]*n.d[2][2] + m.d[2][3]*n.d[3][2],
				m.d[2][0]*n.d[0][3] + m.d[2][1]*n.d[1][3] + m.d[2][2]*n.d[2][3] + m.d[2][3]*n.d[3][3]
			},
			{
				m.d[3][0]*n.d[0][0] + m.d[3][1]*n.d[1][0] + m.d[3][2]*n.d[2][0] + m.d[3][3]*n.d[3][0],
				m.d[3][0]*n.d[0][1] + m.d[3][1]*n.d[1][1] + m.d[3][2]*n.d[2][1] + m.d[3][3]*n.d[3][1],
				m.d[3][0]*n.d[0][2] + m.d[3][1]*n.d[1][2] + m.d[3][2]*n.d[2][2] + m.d[3][3]*n.d[3][2],
				m.d[3][0]*n.d[0][3] + m.d[3][1]*n.d[1][3] + m.d[3][2]*n.d[2][3] + m.d[3][3]*n.d[3][3]
			}
		}
	};
	return ret;
}

mat4_t mat4_adj(mat4_t m) {
	return mat4_trans(mat4_com(m));
}

mat4_t mat4_trans(mat4_t m) {
	mat4_t ret = {
		.d = {
			{ m.d[0][0], m.d[1][0], m.d[2][0], m.d[3][0] },
			{ m.d[0][1], m.d[1][1], m.d[2][1], m.d[3][1] },
			{ m.d[0][2], m.d[1][2], m.d[2][2], m.d[3][2] },
			{ m.d[0][3], m.d[1][3], m.d[2][3], m.d[3][3] }
		}
	};
	return ret;
}

mat4_t mat4_inv(mat4_t m) {
	return mat4_scale(1 / mat4_det(m), mat4_adj(m));
}

mat3_t mat4_sub(mat4_t m, int i, int j) {
	mat3_t ret;
	for(int k = 0; k < 3; k++)
		for(int p = 0; p < 3; p++)
			if(k != i && p != j)
				ret.d[(k < i ? k : k-1)][(p < j ? p : p-1)] = m.d[k][p];
	return ret;
}

mat4_t mat4_com(mat4_t m) {
	mat4_t ret = {
		.d = {
			{ mat3_det(mat4_sub(m,0,0)), mat3_det(mat4_sub(m,0,1)), mat3_det(mat4_sub(m,0,2)), mat3_det(mat4_sub(m,0,3)) },
			{ mat3_det(mat4_sub(m,1,0)), mat3_det(mat4_sub(m,1,1)), mat3_det(mat4_sub(m,1,2)), mat3_det(mat4_sub(m,1,3)) },
			{ mat3_det(mat4_sub(m,2,0)), mat3_det(mat4_sub(m,2,1)), mat3_det(mat4_sub(m,2,2)), mat3_det(mat4_sub(m,2,3)) },
			{ mat3_det(mat4_sub(m,3,0)), mat3_det(mat4_sub(m,3,1)), mat3_det(mat4_sub(m,3,2)), mat3_det(mat4_sub(m,3,3)) }
		}
	};
	return ret;
}

scalar_t mat4_det(mat4_t m) {
	return m.d[0][0]*mat3_det(mat4_sub(m,0,0)) - m.d[0][1]*mat3_det(mat4_sub(m,0,1)) + m.d[0][2]*mat3_det(mat4_sub(m,0,2)) - m.d[0][3]*mat3_det(mat4_sub(m,0,3));
}

