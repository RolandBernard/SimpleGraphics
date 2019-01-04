// Copyright (c) 2019 Roland Bernard
 

#include "transform.h"

mat3_t mat2_to_mat3(mat2_t m) {
	mat3_t ret = {
		.d = {
			{ m.d[0][0], m.d[0][1], 0.0 },
			{ m.d[1][0], m.d[1][1], 0.0 },
			{ 0.0, 0.0, 1.0 }
		}
	};
	return ret;
}

mat4_t mat3_to_mat4(mat3_t m) {
	mat4_t ret = {
		.d = {
			{ m.d[0][0], m.d[0][1], m.d[0][2], 0.0 },
			{ m.d[1][0], m.d[1][1], m.d[1][2], 0.0 },
			{ m.d[2][0], m.d[2][1], m.d[2][2], 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 }
		}
	};
	return ret;
}

mat4_t mat2_to_mat4(mat2_t m) {
	mat4_t ret = {
		.d = {
			{ m.d[0][0], m.d[0][1], 0.0, 0.0 },
			{ m.d[1][0], m.d[1][1], 0.0, 0.0 },
			{ 0.0, 0.0, 1.0, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 }
		}
	};
	return ret;
}

mat2_t mat2_scale_trans(scalar_t s) {
	mat2_t ret = {
		.d = {
			{ s, 0.0 },
			{ 0.0, s }
		}
	};
	return ret;
}

mat3_t mat3_scale_trans(scalar_t s) {
	mat3_t ret = {
		.d = {
			{ s, 0.0, 0.0 },
			{ 0.0, s, 0.0 },
			{ 0.0, 0.0, s }
		}
	};
	return ret;
}

mat4_t mat4_scale_trans(scalar_t s) {
	mat4_t ret = {
		.d = {
			{ s, 0.0, 0.0, 0.0 },
			{ 0.0, s, 0.0, 0.0 },
			{ 0.0, 0.0, s, 0.0 },
			{ 0.0, 0.0, 0.0, s }
		}
	};
	return ret;
}

mat2_t mat2_rotation_trans(scalar_t a) {
	mat2_t ret = {
		.d = {
			{ cosf(a), -sinf(a) },
			{ sinf(a), cosf(a) }
		}
	};
	return ret;
}

mat3_t mat3_rotation_trans(vec3_t axis, scalar_t a) {
	vec3_t n = vec3_norm(axis);
	scalar_t u = n.xyz[0], v = n.xyz[1], w = n.xyz[2];
	mat3_t ret = {
		.d = {
			{ sqr(u)+(1-sqr(u))*cosf(a), u*v*(1-cosf(a))-w*sinf(a), u*w*(1-cosf(a))+v*sinf(a) },
			{ u*v*(1-cosf(a))+w*sinf(a), sqr(v)+(1-sqr(v))*cosf(a), v*w*(1-cosf(a))-u*sinf(a) },
			{ u*w*(1-cosf(a))-v*sinf(a), v*w*(1-cosf(a))+u*sinf(a), sqr(w)+(1-sqr(w))*cosf(a) }
		}
	};
	return ret;
}

mat3_t mat3_translate_trans(vec2_t d) {
	mat3_t ret = {
		.d = {
			{ 1.0, 0.0, d.xy[0] },
			{ 0.0, 1.0, d.xy[1] },
			{ 0.0, 0.0, 1.0 }
		}
	};
	return ret;
}

mat4_t mat4_translate_trans(vec3_t d) {
	mat4_t ret = {
		.d = {
			{ 1.0, 0.0, 0.0, d.xyz[0] },
			{ 0.0, 1.0, 0.0, d.xyz[1] },
			{ 0.0, 0.0, 1.0, d.xyz[2] },
			{ 0.0, 0.0, 0.0, 1.0 }
		}
	};
	return ret;
}

mat4_t mat4_view_trans(vec3_t eye, vec3_t center, vec3_t up) {
	vec3_t f = vec3_norm(vec3_add(center, vec3_scale(-1.0, eye)));
	vec3_t u = vec3_norm(up);
	vec3_t s = vec3_norm(vec3_cross(f, u));
	u = vec3_cross(s, f);
	mat4_t ret = {
		.d = {
			{ s.xyz[0], s.xyz[1], s.xyz[2], -vec3_dot(s, eye) },
			{ u.xyz[0], u.xyz[1], u.xyz[2], -vec3_dot(u, eye) },
			{ -f.xyz[0], -f.xyz[1], -f.xyz[2], vec3_dot(f, eye) },
			{ 0.0, 0.0, 0.0, 1.0 }
		}
	};
	return ret;
}

mat4_t mat4_proj_trans(scalar_t ar, scalar_t near, scalar_t far, scalar_t fov) {
	mat4_t ret = {
		.d = {
			{ 1/(ar*tanf(fov*.5)), 0.0, 0.0, 0.0 },
			{ 0.0, 1/(tanf(fov*.5)), 0.0, 0.0 },
			{ 0.0, 0.0, -(far+near)/(far-near), -2*far*near/(far-near) },
			{ 0.0, 0.0, -1.0, 0.0 }
		}
	};
	return ret;
}

