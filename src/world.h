#pragma once

#include <vector>

#include <mrld/mrld.h>

class World
{
public:
    World(mrld::Window *window, mrld::Camera *cam, mrld::PhysicsEngine *physics_world);
    ~World();
    void draw();

    inline void set_camera(mrld::Camera *cam) { _player_camera = cam; }
    void add(mrld::Model *o);

private:
    mrld::Window *_window;
    mrld::Shader _shader;
    mrld::Renderer3D _renderer;
    mrld::Camera *_player_camera;
    mrld::Shader _hud_shader;
    mrld::Renderer2D _hud_renderer;
    mrld::OrthographicCamera _hud_camera;

    mrld::Layer _world_layer;
    mrld::Layer _hud_layer;
    mrld::PhysicsEngine *_physics_world;
    std::vector<mrld::Model*> _models;

    const mrld::vec2 _crosshair_size = mrld::vec2(16, 16);

    static constexpr float _objects_generation_range = 30.0f;
    static constexpr float _world_size = 30.0f;
    static const uint32_t _tree1_count = 16u;
    static const uint32_t _tree2_count = 16u;
    static const uint32_t _trunk_count = 10u;
    static const uint32_t _stone1_count = 16u;
    static const uint32_t _stone2_count = 16u;
    static const uint32_t _flower_count = 128u;

    static const std::string _vertex_path;
    static const std::string _fragment_path;
    static const std::string _hud_vertex_path;
    static const std::string _hud_fragment_path;
    static std::vector<mrld::Body*> generate_game_bodies();
};
