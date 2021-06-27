#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec2 frag_tex_coord;

void main() {
    gl_Position = projection_mat * view_mat * model_mat * vec4(pos, 1.0);
    frag_tex_coord = tex_coord;
}
