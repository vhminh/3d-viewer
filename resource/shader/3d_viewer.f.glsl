#version 330 core

in vec3 f_position;
in vec3 f_normal;
in vec2 f_tex_coord;

uniform vec3 camera_position;

// textures
uniform sampler2D normal_map;
uniform sampler2D diffuse_map_0;

// lights
struct Attenuation {
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	Attenuation attenuation;
	vec3 direction;
};

struct PointLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	Attenuation attenuation;
	vec3 position;
};

#define MAX_NUM_DIRECTIONAL_LIGHTS 8
#define MAX_NUM_POINT_LIGHTS 24

uniform DirectionalLight directional_lights[MAX_NUM_DIRECTIONAL_LIGHTS];
uniform PointLight point_lights[MAX_NUM_POINT_LIGHTS];
uniform int num_directional_lights;
uniform int num_point_lights;

out vec4 f_out;

void main() {
	vec3 ambient = vec3(0.0);

	vec3 diffuse = vec3(0.0);
	for (int i = 0; i < num_directional_lights; ++i) {
		DirectionalLight light = directional_lights[i];
		float strength = max(0.0, dot(normalize(-light.direction), normalize(f_normal)));
		diffuse += light.diffuse * strength;
	}
	for (int i = 0; i < num_point_lights; ++i) {
		PointLight light = point_lights[i];
		float distance = length(f_position - light.position);
		float attenuation = 1.0 / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * distance * distance);
		vec3 light_dir = f_position - light.position;
		float strength = max(0.0, dot(normalize(-light_dir), normalize(f_normal)));
		diffuse += light.diffuse * strength * attenuation;
	}

	vec4 tex = texture(diffuse_map_0, f_tex_coord);

	f_out = tex * vec4(ambient + diffuse, 1.0);
}

