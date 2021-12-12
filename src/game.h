#pragma once

#include <mrld/mrld.h>

#include "game_loop.h"
#include "world.h"
#include "player.h"

class Game
{
public:
    Game(const Game &o) = delete;
    ~Game();
    Game &operator=(const Game &o) = delete;
    void start();
    void cleanup();

    static uint64_t step(uint64_t dt_micros, uint64_t accumulator);
    static Game &get_instance();

private:
    Game();
    void handle_keys(float dt);
    bool is_player_on_the_ground();

    const uint32_t _window_width;
    const uint32_t _window_height;
    const mrld::vec3 _sky_color;
    mrld::Window _window;

    Player *_player;
    World *_world;
    mrld::KeyboardHandler _keyboard_handler;
    mrld::MouseHandler _mouse_handler;

    mrld::SimplePositionCorrectionSolver _sps;
    mrld::ImpulseSolver _is;
    mrld::PhysicsEngine _physics_world;
    std::vector<mrld::Body*> _bodies;
    GameLoop _loop;

    static constexpr float _objects_generation_range = 50.0f;
    static constexpr float _world_size = 50.0f;
    static const uint32_t _tree1_count = 10u;
    static const uint32_t _tree2_count = 10u;
    static const uint32_t _tps = 128u;

    static Game *_instance;
    static std::vector<mrld::Body*> generate_game_bodies();
};
