#version 330 core

in vec3 f_position;
in vec3 f_normal;
in vec2 f_tex_coord;

uniform vec3 camera_position;

// material
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float shininess_strength;
};
uniform Material material;

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

vec4 tex = texture(diffuse_map_0, f_tex_coord);

vec4 calculate_light(vec3 light_dir, vec3 diffuse_color, float attenuation) {
	// diffuse
	float diff = max(0.0, dot(normalize(-light_dir), normalize(f_normal)));
	vec4 diffuse = vec4(diffuse_color * diff * material.diffuse, 1.0) * tex * attenuation;

	// specular
	// vec3 reflected_light = reflect(light_dir, f_normal);
	// float spec = max(0.0, dot(normalize(reflected_light), normalize(camera_position - f_position)));
	// spec = pow(spec, material.shininess) * material.shininess_strength;
	// vec4 specular = vec4(diffuse_color * spec * material.diffuse, 1.0) * tex * attenuation; // TODO: use specular attributes
	vec4 specular = vec4(0.0);

	return diffuse + specular;
}

void main() {
	f_out = vec4(0.0);
	for (int i = 0; i < num_directional_lights; ++i) {
		DirectionalLight light = directional_lights[i];
		f_out += calculate_light(light.direction, light.diffuse, 1.0);
	}
	for (int i = 0; i < num_point_lights; ++i) {
		PointLight light = point_lights[i];
		vec3 light_ptr = f_position - light.position;
		float distance = length(light_ptr);
		float attenuation = 1.0 / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * distance * distance);
		f_out += calculate_light(normalize(light_ptr), light.diffuse, attenuation);
	}
}

