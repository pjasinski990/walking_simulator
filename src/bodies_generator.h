#pragma once

#include <mrld/mrld.h>
#include "materials.h"

static mrld::Model *tree1_model = nullptr;
static mrld::Model *tree2_model = nullptr;
static mrld::Model *trunk_model = nullptr;
static mrld::Model *stone1_model = nullptr;
static mrld::Model *stone2_model = nullptr;
static mrld::Model *flower_model = nullptr;

namespace bodies_generator
{
    mrld::Model *create_plane(const mrld::vec2 &size, const mrld::vec3 &origin, const mrld::vec3 &normal, float tex_density = 1.0f)
    {
        mrld::vec3 start(0.0f, 1.0f, 0.0f);
        mrld::vec3 v1(mrld::vec3(-size.x / 2.0f, 0.0f, -size.y / 2.0f));
        mrld::vec3 v2(mrld::vec3(size.x / 2.0f, 0.0f, -size.y / 2.0f));
        mrld::vec3 v3(mrld::vec3(size.x / 2.0f, 0.0f, size.y / 2.0f));
        mrld::vec3 v4(mrld::vec3(-size.x / 2.0f, 0.0f, size.y / 2.0f));

        mrld::vec3 axis = start.cross(normal);
        if (axis.magnitude_squared() > 0.1f) {
            float ct = -1.0f * start.dot(normal);
            float s = -1.0f * sqrtf((1.0f + ct) * 2.0f);
            float is = 1.0f / s;
            mrld::quat q(s * 0.5f,axis.x * is,axis.y * is,axis.z * is);
            mrld::mat4 rot = q.normalized().create_rotation_matrix();
            v1 = rot * v1 + origin;
            v2 = rot * v2 + origin;
            v3 = rot * v3 + origin;
            v4 = rot * v4 + origin;
        }

        mrld::VertexData plane[4];
        plane[0] = {v1, normal, mrld::vec2(0, 0), -1, -1, 0xffa0a0a0 };
        plane[1] = { v2, normal, mrld::vec2(size.x, 0) * tex_density, -1, -1,  0xffa0a0a0 };
        plane[2] = { v3, normal, mrld::vec2(size.x, size.y) * tex_density, -1, -1,  0xffa0a0a0 };
        plane[3] = { v4, normal, mrld::vec2(0, size.y) * tex_density, -1, -1,  0xffa0a0a0 };
        uint16_t plane_indices[] = {0, 3, 2, 2, 1, 0};

        return new mrld::Model(plane, 4, plane_indices, 6);
    }

    mrld::Body *create_floor(const mrld::vec2 &size, const mrld::vec3 &origin, float tex_density = 1.0f)
    {
        mrld::Model *floor_model = create_plane(size, origin, mrld::vec3(0.0f, 1.0f, 0.f), tex_density);
        floor_model->assign_material(materials::get_floor_material());
        mrld::PlaneCollider *floor_collider = new mrld::PlaneCollider(mrld::vec3(0.0f, 1.0f, 0.0f), 0.0f);
        mrld::Body *floor_o = new mrld::Body(floor_model, floor_collider, mrld::physics_properties());
        floor_o->phys_properties.mass = 1.0f;
        floor_o->phys_properties.mass_inv = 0.0f;
        return floor_o;
    }

    mrld::Body *create_tree1()
    {
        if (!tree1_model) {
            tree1_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/tree1.obj", "../res/models/tree1.mtl"));
        }
        mrld::SphereCollider *collider = new mrld::SphereCollider(mrld::vec3(0.0f, 1.6f, 0.0f), 0.8f);
        mrld::Body *tree = new mrld::Body(new mrld::Model(*tree1_model), collider, mrld::physics_properties());
        tree->phys_properties.mass_inv = 0.0f;
        return tree;
    }

    mrld::Body *create_tree2()
    {
        if (!tree2_model) {
            tree2_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/tree2.obj", "../res/models/tree2.mtl"));
        }
        mrld::SphereCollider *collider = new mrld::SphereCollider(mrld::vec3(0.0f, 1.6f, 0.0f), 0.8f);
        mrld::Body *tree = new mrld::Body(new mrld::Model(*tree2_model), collider, mrld::physics_properties());
        tree->phys_properties.mass_inv = 0.0f;
        return tree;
    }

    mrld::Body *create_trunk()
    {
        if (!trunk_model) {
            trunk_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/trunk.obj", "../res/models/trunk.mtl"));
        }
        mrld::SphereCollider *collider = new mrld::SphereCollider(mrld::vec3(0.0f, 0.5f, 0.0f), 0.5f);
        mrld::Body *trunk = new mrld::RigidBody(new mrld::Model(*trunk_model), collider, mrld::physics_properties());
        trunk->phys_properties.mass = 40.0f;
        trunk->phys_properties.mass_inv = 1.0f / trunk->phys_properties.mass;
        trunk->phys_properties.friction_d = 1.0f;
        trunk->t.scale = mrld::vec3(0.5f);
        return trunk;
    }

    mrld::Body *create_stone1()
    {
        if (!stone1_model) {
            stone1_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/stone1.obj", "../res/models/stone1.mtl"));
        }
        mrld::SphereCollider *collider = new mrld::SphereCollider(mrld::vec3(0.0f, 0.5f, 0.0f), 0.5f);
        mrld::Body *stone1 = new mrld::RigidBody(new mrld::Model(*stone1_model), collider, mrld::physics_properties());
        stone1->phys_properties.mass = 16.0f;
        stone1->phys_properties.mass_inv = 1.0f / stone1->phys_properties.mass;
        stone1->t.scale = mrld::vec3(0.2f);
        stone1->phys_properties.friction_d = 1.0f;
        stone1->phys_properties.bounciness = 0.1f;
        return stone1;
    }

    mrld::Body *create_stone2()
    {
        if (!stone2_model) {
            stone2_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/stone2.obj", "../res/models/stone2.mtl"));
        }
        mrld::SphereCollider *collider = new mrld::SphereCollider(mrld::vec3(0.0f, 0.5f, 0.0f), 0.5f);
        mrld::Body *stone2 = new mrld::RigidBody(new mrld::Model(*stone2_model), collider, mrld::physics_properties());
        stone2->phys_properties.mass = 16.0f;
        stone2->phys_properties.mass_inv = 1.0f / stone2->phys_properties.mass;
        stone2->t.scale = mrld::vec3(0.2f);
        stone2->phys_properties.friction_d = 1.0f;
        stone2->phys_properties.bounciness = 0.1f;
        return stone2;
    }

    mrld::Body *create_flower()
    {
        if (!flower_model) {
            flower_model = new mrld::Model(mrld::ObjModelParser::parse_obj_to_model("../res/models/flower.obj", "../res/models/flower.mtl"));
        }
        mrld::Body *flower = new mrld::Body(new mrld::Model(*flower_model), nullptr, mrld::physics_properties());
        flower->phys_properties.mass_inv = 1.0f / flower->phys_properties.mass;
        flower->t.scale = mrld::vec3(0.2f);
        return flower;
    }
}
