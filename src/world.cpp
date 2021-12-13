#include "world.h"

const std::string World::_vertex_path = std::string("../res/shader/vertex_3d.shader");
const std::string World::_fragment_path = std::string("../res/shader/fragment_3d.shader");

World::World(mrld::Camera *cam)
        : _shader{_vertex_path.c_str(), _fragment_path.c_str()}
        , _renderer{&_shader}
        , _camera{cam}
{
    _shader.create_shader_program();
    _world_layer = mrld::Layer(&_shader, &_renderer, _camera);

    mrld::directional_light light;
    light.direction = mrld::vec3(1.0f, -1.0f, -1.0f);
    light.specular = mrld::vec3(1.0f, 1.0f, 1.0f);
    light.diffuse = mrld::vec3(1.0f, 1.0f, 1.0f);
    light.ambient = mrld::vec3(0.7f, 1.0f, 0.7f) * 0.1f;
    _shader.set_directional_light(light);
}

World::~World()
{
    for (auto *o : _models) {
        delete o;
    }
}

void World::draw()
{
    _world_layer.draw();
}

void World::add(mrld::Model *o)
{
    if (o) {
        _models.push_back(o);
        _world_layer.add(o);
    }
}
