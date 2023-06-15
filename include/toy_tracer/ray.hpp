#ifndef TOY_TRACER_RAY_HPP
#define TOY_TRACER_RAY_HPP

#include "math.hpp"

namespace toy_tracer
{
class Ray {
  public:
    Ray(const math::Vector<float, 3>& origin, const math::Vector<float, 3>& direction)
            : origin_(origin), direction_(direction)
    {
    }

    const math::Vector<float, 3>& origin() const
    {
        return origin_;
    }

    const math::Vector<float, 3>& direction() const
    {
        return direction_;
    }

    math::Vector<float, 3> at(float t) const
    {
        return origin_ + t * direction_;
    }

  private:
    math::Vector<float, 3> origin_;
    math::Vector<float, 3> direction_;
};
} // namespace toy_tracer

#endif
