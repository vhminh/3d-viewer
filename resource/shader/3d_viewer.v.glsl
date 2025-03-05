#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec3 f_position;
out vec3 f_normal;
out vec2 f_tex_coord;

void main() {
	gl_Position = projection_mat * view_mat * model_mat * vec4(pos.xyz, 1.0);
	f_tex_coord = tex_coord;
	f_position = vec3(model_mat * vec4(pos, 1.0));
	f_normal = vec3(model_mat * vec4(normal, 0.0));
}
