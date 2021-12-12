#pragma once

#include <mrld/mrld.h>

class Player
{
public:
    Player(mrld::Window *window);

    inline mrld::RigidBody *get_player_object() { return &_player_object; }

    void update();
    void move_left(float dt);
    void move_right(float dt);
    void move_forward(float dt);
    void move_backward(float dt);

    inline mrld::FPSCamera *get_camera() { return &_camera; }
    inline float get_height() const { return _player_height; }
private:
    const float _player_height;
    const mrld::vec3 _starting_position;
    const mrld::vec3 _starting_look_target;

    const float _speed;
    const float _jump_height;
    const float _z_near;
    const float _z_far;
    const float _fov;
    const float _aspect;
    mrld::SphereCollider _player_collider;
    mrld::RigidBody _player_object;
    mrld::FPSCamera _camera;
};
