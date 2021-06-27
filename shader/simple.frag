#version 330 core

in vec2 frag_tex_coord;

uniform sampler2D tex;

out vec4 frag_texture;

void main() {
    frag_texture = texture(tex, frag_tex_coord);
}
