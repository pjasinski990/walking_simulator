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

    /**
     * @brief Steps game physics simulation forward
     * This method runs every frame.
     *
     * @param dt_micros the size of time step for physics
     * @param accumulator however many microseconds need to be stepped forward by physics
     */
    static void step(uint64_t dt_micros, uint64_t accumulator);
    static Game &get_instance();

private:
    Game();
    void handle_keys();
    /**
     * @brief Returns true if p has a Body below it closer than _player_on_ground_tolerance
     */
    bool is_player_on_the_ground(Player &p);
    /**
     * @brief Returns objects with colliders in order of their distance to player (closest first)
     */
    std::vector<mrld::collision> get_objects_in_front_of(Player *p);

    const uint32_t _window_width;
    const uint32_t _window_height;
    const mrld::vec3 _sky_color;
    mrld::Window _window;

    Player *_player;
    World *_world;
    mrld::KeyboardHandler _keyboard_handler;
    mrld::MouseHandler _mouse_handler;

    mrld::PhysicsEngine _physics_world;
    mrld::SimplePositionCorrectionSolver _sps;
    mrld::ImpulseSolver _is;
    std::vector<mrld::Body*> _bodies;
    GameLoop _loop;

    const float _player_on_ground_tolerance = 0.1f;
    static const uint32_t _tps = 128u;

    static Game *_instance;
};
