#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in float tex_slot;
layout (location = 4) in float material_slot;
layout (location = 5) in vec4 color;

uniform mat4 model_matrix = mat4(1.0f);
uniform mat4 vp_matrix = mat4(1.0f);
uniform vec3 camera_position = vec3(0.0f, 0.0f, 1.0f);
uniform vec3 camera_direction = vec3(0.0f, 0.0f, -1.0f);

out vec3 v_position;
out vec2 v_tex_coord;
out float v_tex_slot;
out float v_material_slot;

out vec3 v_normal;
out vec4 v_color;

out vec3 v_camera_position;
out vec3 v_camera_direction;

void main()
{
    vec4 world_position = model_matrix * vec4(position, 1.0f);
    v_position = world_position.xyz;
    gl_Position = vp_matrix * world_position;

    v_tex_coord = tex_coord;
    v_tex_slot = tex_slot;
    v_material_slot = material_slot;

    mat3 normals_model_matrix = mat3(model_matrix);
    v_normal = normalize(normals_model_matrix * normal);
    v_color = color;

    v_camera_position = camera_position;
    v_camera_direction = camera_direction;
}
