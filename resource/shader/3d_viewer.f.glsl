#version 330 core

in vec3 f_position;
in vec3 f_normal;
in vec2 f_tex_coord;

uniform vec3 camera_position;

// material
uniform sampler2D albedo_map;
uniform vec3 albedo_color;
uniform bool use_albedo_map;

uniform sampler2D normal_map;
uniform bool use_normal_map;

uniform sampler2D metallic_map;
uniform float metallic_factor;
uniform bool use_metallic_map;

uniform sampler2D roughness_map;
uniform float roughness_factor;
uniform bool use_roughness_map;

uniform sampler2D ambient_occlusion_map;
uniform float ambient_occlusion_factor;
uniform bool use_ambient_occlusion_map;

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

vec4 get_albedo() {
	if (use_albedo_map) {
		return texture(albedo_map, f_tex_coord);
	} else {
		return vec4(albedo_color, 1.0);
	}
}
vec4 albedo = get_albedo();

vec3 calculate_light(vec3 light_dir, vec3 diffuse_color, float attenuation) {
	// diffuse
	float diff = max(0.0, dot(normalize(-light_dir), normalize(f_normal)));
	vec3 diffuse = diffuse_color * diff * albedo.rgb * attenuation;

	vec3 specular = vec3(0.0);

	return diffuse + specular;
}

void main() {
	f_out = vec4(0.0);
	vec3 color = vec3(0.0);
	for (int i = 0; i < num_directional_lights; ++i) {
		DirectionalLight light = directional_lights[i];
		color += calculate_light(light.direction, light.diffuse, 1.0);
	}
	for (int i = 0; i < num_point_lights; ++i) {
		PointLight light = point_lights[i];
		vec3 light_ptr = f_position - light.position;
		float distance = length(light_ptr);
		float attenuation = 1.0 / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * distance * distance);
		color += calculate_light(normalize(light_ptr), light.diffuse, attenuation);
	}
	f_out = vec4(color, albedo.a);
}

