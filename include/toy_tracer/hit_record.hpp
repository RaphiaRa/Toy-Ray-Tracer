#ifndef TOY_TRACER_HIT_RECORD_HPP
#define TOY_TRACER_HIT_RECORD_HPP

#include "math.hpp"

namespace toy_tracer
{
struct HitRecord {
    float distance;
    math::Vector<float, 3> rgb;
    math::Vector<float, 3> normal;
};
} // namespace toy_tracer

#endif
