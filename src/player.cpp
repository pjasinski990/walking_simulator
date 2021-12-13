#include "player.h"

Player::Player(mrld::Window *window)
        : _player_height{2.0f}
        , _starting_position{mrld::vec3(0.0f, _player_height, 1.0f)}
        , _starting_look_target{mrld::vec3(0.0f, _player_height, 0.0f)}
        , _speed{100.0f}
        , _z_near{0.1f}
        , _z_far{500.0f}
        , _fov{45.0f}
        , _player_collider{_starting_position - mrld::vec3(0.0f, _player_height / 2.0f, _player_height / 2.0f), _player_height}
        , _aspect{static_cast<float>(window->get_width()) / static_cast<float>(window->get_height())}
        , _player_object{nullptr, &_player_collider, mrld::physics_properties()}
        , _camera{window, _starting_position, _starting_look_target, mrld::unit_vec::y, _aspect, _z_near, _z_far, _fov}
{
    _player_object.phys_properties.bounciness = 0.0f;
    _player_object.phys_properties.mass = 80.0f;
    _player_object.phys_properties.mass_inv = 1.0f / 80.0f;
    _player_object.phys_properties.friction_d = 0.0f;
    _player_object.phys_properties.friction_s = 0.0f;
}

void Player::update(float dt_f)
{
    mrld::physics_properties &props = _player_object.phys_properties;
    uint32_t direction = _move_direction;
    if (direction) {
        mrld::vec3 res;
        if (direction & static_cast<uint32_t>(MoveDirection::FORWARD)) {
            res += _camera.get_front();
        }
        if (direction & static_cast<uint32_t>(MoveDirection::BACKWARD)) {
            res += _camera.get_back();
        }
        if (direction & static_cast<uint32_t>(MoveDirection::LEFT)) {
            res += _camera.get_left();
        }
        if (direction & static_cast<uint32_t>(MoveDirection::RIGHT)) {
            res += _camera.get_right();
        }
        if (res.magnitude_squared() > 1e-4f) {
            _player_object.phys_properties.velocity += res.normalized() * _speed * dt_f;
            _move_direction = 0u;
        }
    }

    float y = props.acceleration.y;
    props.acceleration -= props.velocity * _speed_damping_factor * dt_f;
    // damp only horizontal velocity
    props.acceleration.y = y;
    _camera.set_position(_player_object.t.position);
}

void Player::move_forward()
{
    _move_direction |= static_cast<uint32_t>(MoveDirection::FORWARD);
}

void Player::move_backward()
{
    _move_direction |= static_cast<uint32_t>(MoveDirection::BACKWARD);
}
void Player::move_left()
{
    _move_direction |= static_cast<uint32_t>(MoveDirection::LEFT);
}

void Player::move_right()
{
    _move_direction |= static_cast<uint32_t>(MoveDirection::RIGHT);
}

void Player::jump()
{
    _player_object.phys_properties.velocity.y = _jump_height;
}

