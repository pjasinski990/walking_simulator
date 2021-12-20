#include <algorithm>

#include "game.h"

Game *Game::_instance{nullptr};

Game::Game()
        : _window_width{1280u}
        , _window_height{720u}
        , _sky_color{mrld::vec3(0.0f, 0.8f, 1.0f)}
        , _window{"Game", _window_width, _window_height}
        , _keyboard_handler{{mrld::KeyCode::W, mrld::KeyCode::S, mrld::KeyCode::A, mrld::KeyCode::D, mrld::KeyCode::SPACE, mrld::KeyCode::ESCAPE}}
        , _mouse_handler{{mrld::MouseButton::BUTTON_LEFT, mrld::MouseButton::BUTTON_RIGHT}}
        , _loop{_tps, step}
{
    srand(static_cast<uint32_t>(time(nullptr)));
    _window.set_clear_color(_sky_color);
    _player = new Player(&_window);
    _world = new World(&_window, _player->get_camera(), &_physics_world);
    _physics_world.add(_player->get_player_object());

    _physics_world.add_solver(&_sps);
    _physics_world.add_solver(&_is);
}

Game::~Game()
{
    for (auto *o : _bodies) {
        delete o->get_collider();
        delete o->get_model();
        delete o;
    }
}

Game &Game::get_instance() {
    if (_instance) {
        return *_instance;
    }
    else {
        _instance = new Game();
        return *_instance;
    }
}

void Game::start()
{
    _loop.start();
}

void Game::cleanup()
{
    delete _instance;
}

void Game::step(uint64_t dt_micros, uint64_t accumulator)
{
    Game &instance = Game::get_instance();
    if (instance._window.should_close()) {
        instance._loop.stop();
    }

    while (accumulator > dt_micros) {
        instance.handle_keys();
        instance._physics_world.step(accumulator * 0.000001f);
        instance._player->update(dt_micros * 0.000001f);
        accumulator -= dt_micros;

        if (accumulator < dt_micros) {
            instance._physics_world.update_models();
        }
    }
    instance._physics_world.interpolate_previous_state(static_cast<float>(accumulator) / static_cast<float>(dt_micros));

    instance._window.clear();
    instance._world->draw();
    instance._window.update();
}

void Game::handle_keys()
{
    if (_keyboard_handler.is_key_down(mrld::KeyCode::W)) {
        _player->move_forward();
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::S)) {
        _player->move_backward();
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::A)) {
        _player->move_left();
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::D)) {
        _player->move_right();
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::ESCAPE)) {
        _player->get_camera()->toggle_cursor_enabled();
        _keyboard_handler.debounce(mrld::KeyCode::ESCAPE);
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::SPACE)) {
        if (is_player_on_the_ground(*_player)) {
            _player->jump();
        }
    }

    if (_mouse_handler.is_button_down(mrld::MouseButton::BUTTON_LEFT)) {
        // Get objects in front of the player

        auto cols = get_objects_in_front_of(_player);
        mrld::collision &closest_collision = cols.front();
        _player->push_object(closest_collision.a, -1.0f * closest_collision.coll_p.collision_depth);
    }
    else if (_mouse_handler.is_button_down(mrld::MouseButton::BUTTON_RIGHT)) {
        auto cols = get_objects_in_front_of(_player);
        mrld::collision &closest_collision = cols.front();
        _player->pull_object(closest_collision.a, -1.0f * closest_collision.coll_p.collision_depth);
    }
}

bool Game::is_player_on_the_ground(Player &p)
{
    auto collisions = _physics_world.shoot_ray(p.get_camera()->get_position(), mrld::vec3(0.0f, -1.0f, 0.0f));
    for (auto &c : collisions) {
        if (-1.0f * c.coll_p.collision_depth <= p.get_height() / 2.0f + _player_on_ground_tolerance) {
            return true;
        }
    }
    return false;
}

std::vector<mrld::collision> Game::get_objects_in_front_of(Player *p)
{
    auto cols = _physics_world.shoot_ray(_player->get_camera()->get_position(), _player->get_camera()->get_direction());
    std::sort(cols.begin(), cols.end(),
              [](const mrld::collision &c1, const mrld::collision &c2) { return c1.coll_p.collision_depth > c2.coll_p.collision_depth; });
    if (cols.front().a == _player->get_player_object()) {
        cols.erase(cols.begin());
    }
    return cols;
}
