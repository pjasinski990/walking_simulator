#pragma once

#include <vector>

#include <mrld/mrld.h>

class World
{
public:
    World(mrld::Camera *cam);
    ~World();
    void draw();

    inline void set_camera(mrld::Camera *cam) { _camera = cam; }
    void add(mrld::Model *o);

private:
    mrld::Shader _shader;
    mrld::Renderer3D _renderer;
    mrld::Camera *_camera;
    mrld::Layer _world_layer;
    std::vector<mrld::Model*> _models;

    static const std::string _vertex_path;
    static const std::string _fragment_path;
};


