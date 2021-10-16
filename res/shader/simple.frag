#version 330 core

in vec2 frag_tex_coord;

uniform sampler2D tex;
uniform vec3 light_color;

out vec4 frag_texture;

void main() {
    frag_texture = texture(tex, frag_tex_coord) * vec4(light_color, 1.0f);
}

