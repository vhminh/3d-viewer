#version 330 core

in vec2 f_tex_coord;
in vec3 f_normal;
in vec3 f_position;

uniform sampler2D tex;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 camera_origin;

out vec4 f_output;

float light_strength_given_angle(vec3 v1, vec3 v2) {
	return max(dot(normalize(v1), normalize(v2)), 0.0);
}

void main() {
	// ambient
	float ambient = 0.2;

	// diffuse
	vec3 light_dir = normalize(f_position - light_pos);
	float diffuse = light_strength_given_angle(-light_dir, f_normal) * 0.8;

	// specular
	vec3 view_dir = normalize(f_position - camera_origin);
	vec3 halfway = normalize(-light_dir - view_dir);
	float specular = pow(light_strength_given_angle(halfway, f_normal), 256) * 0.5;

	// attenuation
	float dist = length(light_pos - f_position);
	float attenuation = 1.0 / (1.0 + 0.045*dist + 0.0075*dist*dist);

	f_output = texture(tex, f_tex_coord) * (vec4(light_color, 1.0) * (ambient + diffuse + specular) * attenuation);
}

