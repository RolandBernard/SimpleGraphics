
#include "mat3.h"

#include "mat2.h"

vec3_t mat3_vec3_mul(mat3_t m, vec3_t v) {
	vec3_t ret = {
		.xyz = {
			m.d[0][0]*v.xyz[0] + m.d[0][1]*v.xyz[1] + m.d[0][2]*v.xyz[2],
			m.d[1][0]*v.xyz[0] + m.d[1][1]*v.xyz[1] + m.d[1][2]*v.xyz[2],
			m.d[2][0]*v.xyz[0] + m.d[2][1]*v.xyz[1] + m.d[2][2]*v.xyz[2]
		}
	};
	return ret;
}

mat3_t mat3_scale(scalar_t s, mat3_t m) {
	mat3_t ret = {
		.d = {
			{ m.d[0][0]*s, m.d[0][1]*s, m.d[0][2]*s },
			{ m.d[1][0]*s, m.d[1][1]*s, m.d[1][2]*s },
			{ m.d[2][0]*s, m.d[2][1]*s, m.d[2][2]*s }
		}
	};
	return ret;
}

mat3_t mat3_add(mat3_t m, mat3_t n) {
	mat3_t ret = {
		.d = {
			{ m.d[0][0]+n.d[0][0], m.d[0][1]+n.d[0][1], m.d[0][2]+n.d[0][2] },
			{ m.d[1][0]+n.d[1][0], m.d[1][1]+n.d[1][1], m.d[1][2]+n.d[1][2] },
			{ m.d[2][0]+n.d[2][0], m.d[2][1]+n.d[2][1], m.d[2][2]+n.d[2][2] }
		}
	};
	return ret;
}

mat3_t mat3_mul(mat3_t m, mat3_t n) {
	mat3_t ret = {
		.d = {
			{
				m.d[0][0]*n.d[0][0] + m.d[0][1]*n.d[1][0] + m.d[0][2]*n.d[2][0],
				m.d[0][0]*n.d[0][1] + m.d[0][1]*n.d[1][1] + m.d[0][2]*n.d[2][1],
				m.d[0][0]*n.d[0][2] + m.d[0][1]*n.d[1][2] + m.d[0][2]*n.d[2][2]
			},
			{
				m.d[1][0]*n.d[0][0] + m.d[1][1]*n.d[1][0] + m.d[1][2]*n.d[2][0],
				m.d[1][0]*n.d[0][1] + m.d[1][1]*n.d[1][1] + m.d[1][2]*n.d[2][1],
				m.d[1][0]*n.d[0][2] + m.d[1][1]*n.d[1][2] + m.d[1][2]*n.d[2][2]
			},
			{
				m.d[2][0]*n.d[0][0] + m.d[2][1]*n.d[1][0] + m.d[2][2]*n.d[2][0],
				m.d[2][0]*n.d[0][1] + m.d[2][1]*n.d[1][1] + m.d[2][2]*n.d[2][1],
				m.d[2][0]*n.d[0][2] + m.d[2][1]*n.d[1][2] + m.d[2][2]*n.d[2][2]
			}
		}
	};
	return ret;
}

mat3_t mat3_adj(mat3_t m) {
	return mat3_trans(mat3_com(m));
}

mat3_t mat3_trans(mat3_t m) {
	mat3_t ret = {
		.d = {
			{ m.d[0][0], m.d[1][0], m.d[2][0] },
			{ m.d[0][1], m.d[1][1], m.d[2][1] },
			{ m.d[0][2], m.d[1][2], m.d[2][2] }
		}
	};
	return ret;
}

mat3_t mat3_inv(mat3_t m) {
	return mat3_scale(1 / mat3_det(m), mat3_adj(m));
}

mat2_t mat3_sub(mat3_t m, int i, int j) {
	mat2_t ret;
	for(int k = 0; k < 3; k++)
		for(int p = 0; p < 3; p++)
			if(k != i && p != j)
				ret.d[(k < i ? k : k-1)][(p < j ? p : p-1)] = m.d[k][p];
	return ret;
}

mat3_t mat3_com(mat3_t m) {
	mat3_t ret = {
		.d = {
			{ mat2_det(mat3_sub(m,0,0)), mat2_det(mat3_sub(m,0,1)), mat2_det(mat3_sub(m,0,2)) },
			{ mat2_det(mat3_sub(m,1,0)), mat2_det(mat3_sub(m,1,1)), mat2_det(mat3_sub(m,1,2)) },
			{ mat2_det(mat3_sub(m,2,0)), mat2_det(mat3_sub(m,2,1)), mat2_det(mat3_sub(m,2,2)) }
		}
	};
	return ret;
}

scalar_t mat3_det(mat3_t m) {
	return m.d[0][0]*mat2_det(mat3_sub(m,0,0)) - m.d[0][1]*mat2_det(mat3_sub(m,0,1)) + m.d[0][2]*mat2_det(mat3_sub(m,0,2));
};

