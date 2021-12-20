#include "world.h"
#include "random_util.h"
#include "bodies_generator.h"

const std::string World::_vertex_path = std::string("../res/shader/vertex_3d.shader");
const std::string World::_fragment_path = std::string("../res/shader/fragment_3d.shader");
const std::string World::_hud_vertex_path = std::string("../res/shader/vertex_hud.shader");
const std::string World::_hud_fragment_path = std::string("../res/shader/fragment_hud.shader");

World::World(mrld::Window *window, mrld::Camera *player_cam, mrld::PhysicsEngine *physics_world)
        : _window{window}
        , _shader{_vertex_path.c_str(), _fragment_path.c_str()}
        , _renderer{&_shader}
        , _player_camera{player_cam}
        , _hud_shader{_hud_vertex_path.c_str(), _hud_fragment_path.c_str()}
        , _hud_renderer{&_hud_shader}
        , _hud_camera{mrld::OrthographicCamera(_window, 0.0f, static_cast<float>(_window->get_width()), 0.0f, static_cast<float>(_window->get_height()), -1.0f, 1.0f)}
        , _physics_world{physics_world}
{
    _shader.create_shader_program();
    _world_layer = mrld::Layer(&_shader, &_renderer, _player_camera);

    _hud_shader.create_shader_program();
    _hud_layer = mrld::Layer(&_hud_shader, &_hud_renderer, &_hud_camera);

    mrld::Texture *crosshair = new mrld::Texture("../res/crosshair.png", true);
    _hud_layer.add(new mrld::Sprite(mrld::vec3(_window->get_width() / 2.0f, _window->get_height() / 2.0f, 0.0f), _crosshair_size, crosshair));

    mrld::directional_light light;
    light.direction = mrld::vec3(1.0f, -1.0f, -1.0f);
    light.specular = mrld::vec3(1.0f, 1.0f, 1.0f);
    light.diffuse = mrld::vec3(1.0f, 1.0f, 1.0f);
    light.ambient = mrld::vec3(0.7f, 1.0f, 0.7f) * 0.1f;
    _shader.set_directional_light(light);

    std::vector<mrld::Body*> bodies = generate_game_bodies();
    for (auto *body : bodies) {
        add(body->get_model());
        _physics_world->add(body);
    }
}

World::~World()
{
    for (auto *o : _models) {
        delete o;
    }
}

void World::draw()
{
    _world_layer.draw();
    _hud_layer.draw();
}

void World::add(mrld::Model *o)
{
    if (o) {
        _models.push_back(o);
        _world_layer.add(o);
    }
}

std::vector<mrld::Body*> World::generate_game_bodies()
{
    std::vector<mrld::Body*> bodies;

    mrld::Body *floor = bodies_generator::create_floor(mrld::vec2(1000.0f, 1000.0f), mrld::vec3(0.0f, 0.0f, 0.0f));
    mrld::Texture *grass_tex = new mrld::Texture("../res/grass.png", true);
    floor->get_model()->set_texture(grass_tex);
    bodies.push_back(floor);
    mrld::physics_properties wall_props;

    for (uint32_t i = 0; i < _tree1_count; ++i) {
        mrld::Body *tree = bodies_generator::create_tree1();
        random_util::position_body_randomly(tree, -_objects_generation_range, _objects_generation_range);
        bodies.push_back(tree);
    }
    for (uint32_t i = 0; i < _tree2_count; ++i) {
        mrld::Body *tree = bodies_generator::create_tree2();
        random_util::position_body_randomly(tree, -_objects_generation_range, _objects_generation_range);
        bodies.push_back(tree);
    }
    for (uint32_t i = 0; i < _trunk_count; ++i) {
        mrld::Body *trunk = bodies_generator::create_trunk();
        random_util::position_body_randomly(trunk, -_objects_generation_range, _objects_generation_range);
        bodies.push_back(trunk);
    }
    for (uint32_t i = 0; i < _stone1_count; ++i) {
        mrld::Body *stone1 = bodies_generator::create_stone1();
        random_util::position_body_randomly(stone1, -_objects_generation_range, _objects_generation_range);
        bodies.push_back(stone1);
    }
    for (uint32_t i = 0; i < _stone2_count; ++i) {
        mrld::Body *stone2 = bodies_generator::create_stone2();
        random_util::position_body_randomly(stone2, -_objects_generation_range, _objects_generation_range);
        bodies.push_back(stone2);
    }
    for (uint32_t i = 0; i < _flower_count; ++i) {
        mrld::Body *flower = bodies_generator::create_flower();
        random_util::position_body_randomly(flower, -_objects_generation_range, _objects_generation_range);
        bodies.push_back(flower);
    }

    wall_props.mass_inv = 0.0f;
    mrld::vec2 wall_size(_world_size * 2.0f, _world_size * 2.0f);
    mrld::material wall_m = materials::get_wall_material();
    mrld::Body *left_wall = new mrld::Body(
            bodies_generator::create_plane(wall_size, mrld::vec3(-_world_size, 0.0f, 0.0f), mrld::vec3(-1.0f, 0.0f, 0.0f)),
            new mrld::PlaneCollider(mrld::vec3(1.0f, 0.0f, 0.0f), -_world_size),
            wall_props);
    mrld::Body *right_wall = new mrld::Body(
            bodies_generator::create_plane(wall_size, mrld::vec3(_world_size, 0.0f, 0.0f), mrld::vec3(1.0f, 0.0f, 0.0f)),
            new mrld::PlaneCollider(mrld::vec3(-1.0f, 0.0f, 0.0f), -_world_size),
            wall_props);
    mrld::Body *front_wall = new mrld::Body(
            bodies_generator::create_plane(wall_size, mrld::vec3(0.0f, 0.0f, -_world_size), mrld::vec3(0.0f, 0.0f, -1.0f)),
            new mrld::PlaneCollider(mrld::vec3(0.0f, 0.0f, 1.0f), -_world_size),
            wall_props);
    mrld::Body *back_wall = new mrld::Body(
            bodies_generator::create_plane(wall_size, mrld::vec3(0.0f, 0.0f, _world_size), mrld::vec3(0.0f, 0.0f, 1.0f)),
            new mrld::PlaneCollider(mrld::vec3(0.0f, 0.0f, -1.0f), -_world_size),
            wall_props);
    left_wall->get_model()->assign_material(wall_m);
    right_wall->get_model()->assign_material(wall_m);
    front_wall->get_model()->assign_material(wall_m);
    back_wall->get_model()->assign_material(wall_m);
    bodies.push_back(right_wall);
    bodies.push_back(back_wall);
    bodies.push_back(front_wall);
    bodies.push_back(left_wall);

    return bodies;
}
