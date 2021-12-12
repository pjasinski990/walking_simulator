#version 460

in vec2 v_tex_coord;
in float v_tex_slot;
in vec4 v_color;

out vec4 f_color;

uniform sampler2D[32] textures;

void main()
{
//    if (v_tex_slot >= 0.0f) {
//        uint slot = uint(v_tex_slot + 0.5f);
//        vec4 tex_color = texture(textures[slot], v_tex_coord);
//        f_color = tex_color;
//    }
//    else {
//        f_color = v_color;
//    }
    f_color = v_color;
}
