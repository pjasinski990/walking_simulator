#pragma once

#include <mrld/mrld.h>
#include "materials.h"

static mrld::Model *tree1_model = nullptr;
static mrld::Model *tree2_model = nullptr;
static mrld::Model *tree3_model = nullptr;
static mrld::Model *tree4_model = nullptr;

mrld::Body *create_floor(const mrld::vec2 &size, const mrld::vec3 &center)
{
    mrld::VertexData floor[4];
    mrld::vec3 normal(0.0f, 1.0f, 0.0f);
    mrld::vec3 v1(center + mrld::vec3(-size.x / 2.0f, 0.0f, -size.y / 2.0f));
    mrld::vec3 v2(center + mrld::vec3(size.x / 2.0f, 0.0f, -size.y / 2.0f));
    mrld::vec3 v3(center + mrld::vec3(size.x / 2.0f, 0.0f, size.y / 2.0f));
    mrld::vec3 v4(center + mrld::vec3(-size.x / 2.0f, 0.0f, size.y / 2.0f));

    floor[0] = {v1, normal, mrld::vec2(0, 0), -1, -1, 0xffa0a0a0 };
    floor[1] = { v2, normal, mrld::vec2(size.x, 0), -1, -1,  0xffa0a0a0 };
    floor[2] = { v3, normal, mrld::vec2(size.x, size.y), -1, -1,  0xffa0a0a0 };
    floor[3] = { v4, normal, mrld::vec2(0, size.y), -1, -1,  0xffa0a0a0 };
    uint16_t floor_indices[] = {0, 3, 2, 2, 1, 0};

    mrld::Model *floor_model = new mrld::Model(floor, 4, floor_indices, 6);
    floor_model->assign_material(materials::get_floor_material());
    mrld::PlaneCollider *floor_collider = new mrld::PlaneCollider(mrld::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    mrld::Body *floor_o = new mrld::Body(floor_model, floor_collider, mrld::physics_properties());
    floor_o->phys_properties.mass_inv = 0.0f;
    return floor_o;
}

mrld::Body *create_tree1()
{
    if (!tree1_model) {
        tree1_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/tree1.obj", "../res/models/tree1.mtl"));
    }
    mrld::SphereCollider *tree_collider = new mrld::SphereCollider(mrld::vec3(0.0f, 2.0f, 0.0f), 1.0f);
    mrld::Body *tree = new mrld::Body(new mrld::Model(*tree1_model), tree_collider, mrld::physics_properties());
    tree->phys_properties.mass_inv = 0.0f;
    return tree;
}

mrld::Body *create_tree2()
{
    if (!tree2_model) {
        tree2_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/tree2.obj", "../res/models/tree2.mtl"));
    }
    mrld::SphereCollider *tree_collider = new mrld::SphereCollider(mrld::vec3(0.0f, 2.0f, 0.0f), 1.0f);
    mrld::Body *tree = new mrld::Body(new mrld::Model(*tree2_model), tree_collider, mrld::physics_properties());
    tree->phys_properties.mass_inv = 0.0f;
    return tree;
}

mrld::Body *create_tree3()
{
    if (!tree3_model) {
        tree3_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/tree3.obj", "../res/models/tree3.mtl"));
    }
    mrld::SphereCollider *tree_collider = new mrld::SphereCollider(mrld::vec3(0.0f, 2.0f, 0.0f), 1.0f);
    mrld::Body *tree = new mrld::Body(new mrld::Model(*tree3_model), tree_collider, mrld::physics_properties());
    tree->phys_properties.mass_inv = 0.0f;
    return tree;
}

mrld::Body *create_tree4()
{
    if (!tree4_model) {
        tree4_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/tree4.obj", "../res/models/tree4.mtl"));
    }
    mrld::SphereCollider *tree_collider = new mrld::SphereCollider(mrld::vec3(0.0f, 2.0f, 0.0f), 1.0f);
    mrld::Body *tree = new mrld::Body(new mrld::Model(*tree4_model), tree_collider, mrld::physics_properties());
    tree->phys_properties.mass_inv = 0.0f;
    return tree;
}
