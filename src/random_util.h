#pragma once

#include <mrld/mrld.h>

namespace random_util
{
    float next_float(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    float next_float(float max) {
        return next_float(0.0f, max);
    }

    void position_body_randomly(mrld::Body *body, float min_pos, float max_pos) {
        body->t.position.x = random_util::next_float(min_pos, max_pos);
        body->t.position.z = random_util::next_float(min_pos, max_pos);
        body->t.rotation = mrld::quat(mrld::vec3(0.0f, 1.0f, 0.0f), random_util::next_float(mrld::math_constants::pi * 2.0f));
    }
}
