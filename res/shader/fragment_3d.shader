#version 460

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float specular_e;
    float dissolve;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 v_tex_coord;
in float v_tex_slot;
in float v_material_slot;
in vec3 v_position;
in vec3 v_normal;
in vec4 v_color;
in vec3 v_camera_position;
in vec3 v_camera_direction;

out vec4 f_color;

uniform sampler2D[32] textures;
uniform Material[32] materials;
uniform DirectionalLight directional_light;
uniform PointLight[32] point_lights;
uniform int lights_count = 0;

vec3 calc_directional_light(DirectionalLight l, vec3 normal, vec3 view_direction, Material material);
vec3 calc_point_light(PointLight l, vec3 normal, vec3 frag_position, vec3 view_direction, Material material);

void main()
{
    // Use directional light
    if (v_material_slot >= 0.0f) {
        uint m_slot = uint(v_material_slot + 0.5f);
        vec3 view_direction = normalize(v_camera_position - v_position);
        f_color = vec4(calc_directional_light(directional_light, v_normal, view_direction, materials[m_slot]), 1.0f);
    }
    if (v_tex_slot >= 0.0f) {
        uint t_slot = uint(v_tex_slot + 0.5f);
        vec4 tex_color = texture(textures[t_slot], v_tex_coord);
        f_color *= tex_color;
    }

    // Add point lights
    for (int i = 0; i < lights_count; ++i) {
        vec3 res = vec3(0.0f);
        if (v_material_slot >= 0.0f) {
            uint m_slot = uint(v_material_slot + 0.5f);
            vec3 view_direction = normalize(v_camera_position - v_position);
            res = calc_point_light(point_lights[i], v_normal, v_position, view_direction, materials[m_slot]);
        }
        if (v_tex_slot >= 0.0f) {
            uint t_slot = uint(v_tex_slot + 0.5f);
            vec4 tex_color = texture(textures[t_slot], v_tex_coord);
            res *= tex_color.xyz;
        }
        f_color += vec4(res, 0.0f);
    }
}

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction, Material material) {
    vec3 light_direction = normalize(-light.direction);
    float diff = max(dot(normal, light_direction), 0.0f);

    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), material.specular_e);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return ambient + diffuse + specular;
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction, Material material) {
    vec3 light_direction = normalize(light.position - frag_position);
    float diff = max(dot(normal, light_direction), 0.0f);

    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), material.specular_e);

    float distance = length(light.position - frag_position);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * material.ambient * attenuation;
    vec3 diffuse = light.diffuse * diff * material.diffuse * attenuation;
    vec3 specular = light.specular * spec * material.specular * attenuation;

    return ambient + diffuse + specular;
}
