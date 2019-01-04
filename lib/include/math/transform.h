#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "scalar.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

#include "mat2.h"
#include "mat3.h"
#include "mat4.h"

mat3_t mat2_to_mat3(mat2_t m);

mat4_t mat3_to_mat4(mat3_t m);

mat4_t mat2_to_mat4(mat2_t m);

mat2_t mat2_scale_trans(scalar_t s);

mat3_t mat3_scale_trans(scalar_t s);

mat4_t mat4_scale_trans(scalar_t s);

mat2_t mat2_rotation_trans(scalar_t a);

mat3_t mat3_rotation_trans(vec3_t axis, scalar_t a);

mat3_t mat3_translate_trans(vec2_t d);

mat4_t mat4_translate_trans(vec3_t d);

mat4_t mat4_view_trans(vec3_t pos, vec3_t lookat, vec3_t up);

mat4_t mat4_proj_trans(scalar_t ar, scalar_t near, scalar_t far, scalar_t fov);

#endif
