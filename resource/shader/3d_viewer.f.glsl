#version 330 core
#define MAX_NUM_UV_CHANNELS 2
#define PI 3.141592653589793

in VS_OUT {
	vec3 position;
	vec2 tex_coords[MAX_NUM_UV_CHANNELS];
	vec3 normal;
	mat3 tangent_mat;
} f_in;

uniform vec3 camera_position;

// material
uniform sampler2D base_color_texture;
uniform bool has_base_color_texture;
uniform vec4 base_color_factor;
uniform int base_color_uv_channel;

uniform sampler2D normal_texture;
uniform bool has_normal_texture;
uniform int normal_uv_channel;

uniform sampler2D metallic_texture;
uniform bool has_metallic_texture;
uniform float metallic_factor;
uniform int metallic_uv_channel;

uniform sampler2D roughness_texture;
uniform bool has_roughness_texture;
uniform float roughness_factor;
uniform int roughness_uv_channel;

uniform sampler2D ambient_occlusion_texture;
uniform bool has_ao_texture;
uniform float ao_strength;
uniform int ao_uv_channel;

// lights
struct Attenuation {
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 color;
	vec3 direction;
};

struct PointLight {
	vec3 color;
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

// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#_material_pbrmetallicroughness_basecolortexture
vec4 get_base_color() {
	if (has_base_color_texture) {
		return texture(base_color_texture, f_in.tex_coords[base_color_uv_channel]) * base_color_factor;
	} else {
		return vec4(1.0) * base_color_factor;
	}
}

vec3 get_normal() {
	if (has_normal_texture) {
		vec3 normal = texture(normal_texture, f_in.tex_coords[normal_uv_channel]).rgb;
		normal = normal * 2.0 - 1.0;
		return normalize(f_in.tangent_mat * normal);
	} else {
		return normalize(f_in.normal);
	}
}

// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#_material_pbrmetallicroughness_metallicroughnesstexture
float get_metallic()  {
	if (has_metallic_texture) {
		return texture(metallic_texture, f_in.tex_coords[metallic_uv_channel]).b * metallic_factor;
	} else {
		return metallic_factor;
	}
}
 
// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#_material_pbrmetallicroughness_metallicroughnesstexture
float get_roughness() {
	if (has_roughness_texture) {
		return texture(roughness_texture, f_in.tex_coords[roughness_uv_channel]).g * roughness_factor;
	} else {
		return roughness_factor;
	}
}

float get_ambient_occlusion() {
	if (has_ao_texture) {
		return texture(ambient_occlusion_texture, f_in.tex_coords[ao_uv_channel]).r;
	} else {
		return 0.0; // no occlusion
	}
}

// Trowbridge-Reitz GGX
float D(float roughness, vec3 normal, vec3 halfway) {
	float a = roughness * roughness;
	float n_dot_h = max(0.0, dot(normal, halfway));
	float denom = (n_dot_h * n_dot_h) * (a * a - 1) + 1;
	denom = PI * denom * denom;
	return a * a / denom;
}

// Schlick-GGX
float G_sub(float roughness, vec3 normal, vec3 view_or_light) {
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
	float n_dot_v = max(0.0, dot(normal, -view_or_light));
	return n_dot_v / (n_dot_v * (1.0 - k) + k);
}

// Smith
float G(float roughness, vec3 normal, vec3 view, vec3 light) {
	return G_sub(roughness, normal, view) * G_sub(roughness, normal, light);
}

// Fresnel-Schlick
vec3 F(vec3 albedo, float metallic, vec3 halfway, vec3 view) {
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	// vec3 F0 = albedo * metallic;
	float h_dot_v = max(0.0, dot(halfway, -view));
	return F0 + (1.0 - F0) * pow(1.0 - h_dot_v, 5.0);
}

vec3 calculate_light(vec3 albedo, vec3 normal, float metallic, float roughness, vec3 light_dir, vec3 light_color, float attenuation) {
	vec3 view = normalize(f_in.position - camera_position);
	vec3 halfway = normalize(-light_dir - view);

	vec3 lambert = albedo / PI;
	vec3 F = F(albedo, metallic, halfway, view);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;
	vec3 DFG = D(roughness, normal, halfway) * F * G(roughness, normal, view, light_dir);
	vec3 cook_torrance = DFG / (4 * max(0.001, dot(-light_dir, normal)) * max(0.001, dot(-view, normal)));
	return (kD * lambert + cook_torrance) * light_color * attenuation * max(0.0, dot(normal, -light_dir));
}

void main() {
	f_out = vec4(0.0);
	vec3 color = vec3(0.0);
	vec4 albedo = get_base_color();
	if (albedo.a < 0.1) {
		discard;
	}
	vec3 normal = get_normal();
	float metallic = get_metallic();
	float roughness = get_roughness();
	float ao = get_ambient_occlusion();
	for (int i = 0; i < num_directional_lights; ++i) {
		DirectionalLight light = directional_lights[i];
		color += calculate_light(vec3(albedo), normal, metallic, roughness, normalize(light.direction), light.color, 1.0);
	}
	for (int i = 0; i < num_point_lights; ++i) {
		PointLight light = point_lights[i];
		vec3 light_ptr = f_in.position - light.position;
		float distance = length(light_ptr);
		float attenuation = 1.0 / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * distance * distance);
		color += calculate_light(vec3(albedo), normal, metallic, roughness, normalize(light_ptr), light.color, attenuation);
	}
	float occlusion = 1.0 + ao_strength * (ao - 1.0);
	f_out = vec4(color * (1.0 - occlusion), albedo.a);
}

