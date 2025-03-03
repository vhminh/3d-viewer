#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;

// uniform sampler2D normal_map;

// uniform sampler2D ambient_map_0;
// uniform sampler2D ambient_map_1;

// uniform sampler2D diffuse_map_0;
// uniform sampler2D diffuse_map_1;

// uniform sampler2D specular_map_0;
// uniform sampler2D specular_map_1;

// uniform int normal_map_count;
// uniform int ambient_map_count;
// uniform int diffuse_map_count;
// uniform int specular_map_count;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec2 f_tex_coord;

void main() {
	gl_Position = projection_mat * view_mat * model_mat * vec4(pos.xyz, 1.0);
	f_tex_coord = tex_coord;
}
