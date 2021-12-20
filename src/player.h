#pragma once

#include <map>

#include <mrld/mrld.h>

class Player
{
public:
    enum class MoveDirection {
        FORWARD = 0b1000,
        BACKWARD = 0b0100,
        LEFT = 0b0010,
        RIGHT = 0b0001
    };

    Player(mrld::Window *window);

    inline mrld::RigidBody *get_player_object() { return &_player_object; }

    void update(float dt_f);
    void move_forward();
    void move_backward();
    void move_left();
    void move_right();
    void jump();
    void push_object(mrld::Body *object, float current_distance);
    void pull_object(mrld::Body *object, float current_distance);

    inline mrld::FPSCamera *get_camera() { return &_camera; }
    inline float get_height() const { return _player_height; }
private:
    const float _player_height;
    const mrld::vec3 _starting_position;
    const mrld::vec3 _starting_look_target;

    const float _speed;
    const float _z_near;
    const float _z_far;
    const float _fov;
    const float _aspect;
    mrld::SphereCollider _player_collider;
    mrld::RigidBody _player_object;
    mrld::FPSCamera _camera;

    uint32_t _move_direction = 0u;
    const float _jump_height = 4.0f;
    const float _speed_damping_factor = 80000.0f;
    const float _player_reach_distance = 3.0f;
    const float _push_strength = 500.0f;
    const float _pull_strength = 500.0f;
};
