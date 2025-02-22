#version 330 core

in vec2 f_tex_coord;
in vec3 f_normal;
in vec3 f_position;
in vec3 f_light_pos;

uniform sampler2D tex;
uniform vec3 light_color;

out vec4 output;

void main() {
	float ambient = 0.2;
	float diffuse = max(dot(normalize(f_light_pos - f_position), normalize(f_normal)), 0) * 1.5;
	output = texture(tex, f_tex_coord) * vec4(light_color * (ambient + diffuse), 1.0f);
}

