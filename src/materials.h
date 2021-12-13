#pragma once

#include <string>

#include <mrld/mrld.h>

namespace materials
{
    mrld::material get_floor_material()
    {
        mrld::material floor;
        floor.name = std::string("floor");
        floor.ambient = mrld::vec3(1.0f, 1.0f, 1.0f) * 0.1f,
        floor.diffuse = mrld::vec3(0.33f, 0.5f, 0.27f),
        floor.specular = mrld::vec3(1.0f, 1.0f, 1.0f) * 0.25f,
        floor.specular_e = 2.0f,
        floor.dissolve = 1.0f;
        return floor;
    }

    mrld::material get_wall_material()
    {
        mrld::material wall;
        wall.name = std::string("wall");
        wall.ambient = mrld::vec3(0.8f, 0.3f, 0.3f) * 10.0f;
        wall.diffuse = mrld::vec3();
        wall.specular = mrld::vec3();
        wall.specular_e = 2.0f,
        wall.dissolve = 0.2f;
        return wall;
    }
}
