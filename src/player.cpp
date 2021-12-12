#include "player.h"

Player::Player(mrld::Window *window)
        : _player_height{2.0f}
        , _starting_position{mrld::vec3(0.0f, _player_height, 1.0f)}
        , _starting_look_target{mrld::vec3(0.0f, _player_height, 0.0f)}
        , _jump_height{5.0f}
        , _speed{100.0f}
        , _z_near{0.1f}
        , _z_far{500.0f}
        , _fov{45.0f}
        , _player_collider{_starting_position - mrld::vec3(0.0f, _player_height / 2.0f, 0.0f), _player_height}
        , _aspect{static_cast<float>(window->get_width()) / static_cast<float>(window->get_height())}
        , _player_object{nullptr, &_player_collider, mrld::physics_properties()}
        , _camera{window, _starting_position, _starting_look_target, mrld::unit_vec::y, _aspect, _z_near, _z_far, _fov}
{
    _player_object.phys_properties.bounciness = 0.0f;
    _player_object.phys_properties.mass = 80.0f;
    _player_object.phys_properties.mass_inv = 1.0f / 80.0f;
    _player_object.phys_properties.friction_d = 1.0f;
    _player_object.phys_properties.friction_s = 1.0f;
}

void Player::update()
{
    _camera.set_position(_player_object.t.position);
}

void Player::move_left(float dt)
{
    _player_object.phys_properties.velocity += _camera.get_left() * _speed * dt;
}

void Player::move_right(float dt)
{
    _player_object.phys_properties.velocity += _camera.get_right() * _speed * dt;
}

void Player::move_forward(float dt)
{
    _player_object.phys_properties.velocity += _camera.get_front() * _speed * dt;
}

void Player::move_backward(float dt)
{
    _player_object.phys_properties.velocity += _camera.get_back() * _speed * dt;
}
