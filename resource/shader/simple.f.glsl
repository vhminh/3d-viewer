#version 330 core

in vec2 f_tex_coord;
in vec3 f_normal;
in vec3 f_position;

uniform sampler2D tex;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 camera_origin;

out vec4 output;

float light_strength_given_angle(vec3 v1, vec3 v2) {
	return max(dot(normalize(v1), normalize(v2)), 0.0);
}

void main() {
	float ambient = 0.2;
	vec3 light_dir = f_position - light_pos;
	float diffuse = light_strength_given_angle(-light_dir, f_normal) * 0.8;
	vec3 reflected_light_dir = reflect(light_dir, f_normal);
	float specular = pow(light_strength_given_angle(reflected_light_dir, camera_origin - f_position), 256) * 0.5;
	output = texture(tex, f_tex_coord) * (vec4(light_color, 1.0) * (ambient + diffuse + specular));
}

