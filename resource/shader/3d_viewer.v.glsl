#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 tangent;
layout (location = 2) in vec3 bitangent;
layout (location = 3) in vec3 normal;
layout (location = 4) in vec2 tex_coord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out VS_OUT {
	out vec3 position;
	out vec2 tex_coord;
	out vec3 normal;
	out mat3 tangent_mat;
} v_out;

void main() {
	gl_Position = projection_mat * view_mat * model_mat * vec4(pos.xyz, 1.0);

	v_out.position = vec3(model_mat * vec4(pos, 1.0));
	v_out.tex_coord = tex_coord;

	vec3 tangent_world = normalize(vec3(model_mat * vec4(tangent, 0.0)));
	vec3 bitangent_world = normalize(vec3(model_mat * vec4(bitangent, 0.0)));
	vec3 normal_world = normalize(vec3(model_mat * vec4(normal, 0.0)));

	v_out.normal = normal_world;
	v_out.tangent_mat = mat3(tangent_world, bitangent_world, normal_world);
}
