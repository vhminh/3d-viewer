#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform vec3 light_pos;

out vec2 f_tex_coord;
out vec3 f_normal;
out vec3 f_position;
out vec3 f_light_pos;

void main() {
	gl_Position = projection_mat * view_mat * model_mat * vec4(pos, 1.0);
	f_tex_coord = tex_coord;
	f_normal = (model_mat * vec4(normal, 1.0)).xyz;
	f_position = (model_mat * vec4(pos, 1.0)).xyz;
	f_light_pos = light_pos;
}

