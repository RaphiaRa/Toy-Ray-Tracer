#ifndef TOY_TRACER_HITABLE_HPP
#define TOY_TRACER_HITABLE_HPP

#include "math.hpp"
#include "ray.hpp"
#include "vertex_shader.hpp"

#include <cstdint>

namespace toy_tracer
{
struct HitRecord {
    float distance;
    math::Vector<std::uint8_t, 3> rgb;
};

class Hitable {
  public:
    using Vector3 = math::Vector<float, 3>;
    using Matrix3 = math::Matrix<float, 3, 3>;

    virtual ~Hitable()                                                                    = default;
    virtual bool hit(const Ray& ray, const VertexShader& shader, HitRecord& record) const = 0;
};
} // namespace toy_tracer

#endif
