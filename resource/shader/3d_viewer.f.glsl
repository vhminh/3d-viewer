#version 330 core

in vec2 f_tex_coord;
out vec4 f_out;

uniform sampler2D ambient_map_0;
uniform sampler2D diffuse_map_0;

void main() {
	// f_out = texture(ambient_map_0, f_tex_coord) + texture(diffuse_map_0, f_tex_coord);
	f_out = texture(diffuse_map_0, f_tex_coord);
}

