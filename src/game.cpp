#include "game.h"
#include "bodies_generator.h"

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
    _window.set_clear_color(_sky_color);
    _player = new Player(&_window);
    _world = new World(_player->get_camera());
    _physics_world.add(_player->get_player_object());

    _physics_world.add_solver(&_sps);
    _physics_world.add_solver(&_is);

    // Init scene
    std::vector<mrld::Body*> bodies = generate_game_bodies();
    for (auto *body : bodies) {
        _world->add(body->get_model());
        _physics_world.add(body);
    }
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

uint64_t Game::step(uint64_t dt_micros, uint64_t accumulator)
{
    Game &instance = Game::get_instance();
    if (instance._window.should_close()) {
        instance._loop.stop();
    }

    float dt_f = dt_micros * 0.000001f;
    while (accumulator > dt_micros) {
        instance.handle_keys(dt_micros * 0.000001f);
        instance._physics_world.step(dt_f);
        accumulator -= dt_micros;

        if (accumulator < dt_micros) {
            instance._physics_world.update_models();
        }
    }
    instance._physics_world.interpolate_previous_state(static_cast<float>(accumulator) / static_cast<float>(dt_micros));

    instance._player->update();

    instance._window.clear();
    instance._world->draw();
    instance._window.update();
    return accumulator;
}

void Game::handle_keys(float dt)
{
    if (_keyboard_handler.is_key_down(mrld::KeyCode::W)) {
        _player->move_forward(dt);
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::S)) {
        _player->move_backward(dt);
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::A)) {
        _player->move_left(dt);
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::D)) {
        _player->move_right(dt);
    }
    if (_keyboard_handler.is_key_down(mrld::KeyCode::ESCAPE)) {
        _player->get_camera()->toggle_cursor_enabled();
        _keyboard_handler.debounce(mrld::KeyCode::ESCAPE);
    }
}

bool Game::is_player_on_the_ground()
{
    auto collisions = _physics_world.shoot_ray(_player->get_camera()->get_position(), mrld::vec3(0.0f, -1.0f, 0.0f));
    for (auto &c : collisions) {
        std::cout << c.coll_p.collision_depth <<std::endl;
        if (-1.0f * c.coll_p.collision_depth <= _player->get_height() / 2.0f) {
            return true;
        }
    }
    return false;
}

std::vector<mrld::Body*> Game::generate_game_bodies()
{
    std::vector<mrld::Body*> bodies;

    mrld::Body *floor = create_floor(mrld::vec2(1000.0f, 1000.0f), mrld::vec3(0.0f, 0.0f, 0.0f));
    bodies.push_back(floor);
    mrld::physics_properties wall_props;
    wall_props.mass_inv = 0.0f;
    mrld::Body *left_wall = new mrld::Body(nullptr, new mrld::PlaneCollider(mrld::vec3(1.0f, 0.0f, 0.0f), -_world_size), wall_props);
    mrld::Body *right_wall = new mrld::Body(nullptr, new mrld::PlaneCollider(mrld::vec3(-1.0f, 0.0f, 0.0f), -_world_size), wall_props);
    mrld::Body *back_wall = new mrld::Body(nullptr, new mrld::PlaneCollider(mrld::vec3(1.0f, 0.0f, 1.0f), -_world_size), wall_props);
    mrld::Body *front_wall = new mrld::Body(nullptr, new mrld::PlaneCollider(mrld::vec3(0.0f, 0.0f, -1.0f), -_world_size), wall_props);
    bodies.push_back(left_wall);
    bodies.push_back(right_wall);
    bodies.push_back(back_wall);
    bodies.push_back(front_wall);

    for (uint32_t i = 0; i < _tree1_count; ++i) {
        mrld::Body *tree = create_tree3();
        const float rand_x = static_cast<float>(rand()) / RAND_MAX * 2.0f * _objects_generation_range - _objects_generation_range;
        const float rand_y = static_cast<float>(rand()) / RAND_MAX * 2.0f * _objects_generation_range - _objects_generation_range;
        const float rand_rads = static_cast<float>(rand()) / RAND_MAX * 2.0f * mrld::math_constants::pi;
        tree->t.position.x = rand_x;
        tree->t.position.z = rand_y;
        tree->t.rotation = mrld::quat(mrld::vec3(0.0f, 1.0f, 0.0f), rand_rads);
        bodies.push_back(tree);
    }

    for (uint32_t i = 0; i < _tree2_count; ++i) {
        mrld::Body *tree = create_tree4();
        const float rand_x = static_cast<float>(rand()) / RAND_MAX * 2.0f * _objects_generation_range - _objects_generation_range;
        const float rand_y = static_cast<float>(rand()) / RAND_MAX * 2.0f * _objects_generation_range - _objects_generation_range;
        const float rand_rads = static_cast<float>(rand()) / RAND_MAX * 2.0f * mrld::math_constants::pi;
        tree->t.position.x = rand_x;
        tree->t.position.z = rand_y;
        tree->t.rotation = mrld::quat(mrld::vec3(0.0f, 1.0f, 0.0f), rand_rads);
        bodies.push_back(tree);
    }

    return bodies;
}
