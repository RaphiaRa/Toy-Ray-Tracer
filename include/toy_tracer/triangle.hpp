#ifndef TOY_TRACER_TRIANGLE_HPP
#define TOY_TRACER_TRIANGLE_HPP

#include "hit_record.hpp"
#include "math.hpp"
#include "ray.hpp"
#include "vertex_map.hpp"

namespace toy_tracer
{

class Triangle {
  public:
    using Vector3 = math::Vector<float, 3>;
    using Matrix3 = math::Matrix<float, 3, 3>;

    Triangle() = default;

    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2)
            : v0_(v0), v1_(v1), v2_(v2)
    {
    }

    std::optional<HitRecord> hit(const Ray& ray, const VertexMap& vertexMap) const
    {
        HitRecord record;
        Vector3 v0 = vertexMap.map(v0_);
        Vector3 v1 = vertexMap.map(v1_);
        Vector3 v2 = vertexMap.map(v2_);

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;
        Vector3 D  = ray.direction();
        Vector3 O  = ray.origin();

        // Determinant
        float det = math::dot(math::cross(e0, e1), -D);
        if (det < 0)
            return std::nullopt;
        if (det < std::numeric_limits<float>::epsilon())
            return std::nullopt;

        // Solve (O - v0) = (e0, e1, -D) * (u, v, t)
        // using Cramer's rule
        Vector3 Y = O - v0;
        Vector3 X = Vector3{
            math::dot(math::cross(Y, e1), -D),
            math::dot(math::cross(e0, Y), -D),
            math::dot(math::cross(e0, e1), Y),
        } / det;
        if (X[0] < 0 || X[1] < 0 || X[2] < 0 || X[0] + X[1] > 1)
            return std::nullopt;
        record.distance = X[2];

        // shade
        Vector3 normal = math::normalize(math::cross(e0, e1));
        float shade    = det;
        Vector3 color  = Vector3{ 50.0, 50.0, 50.0 } + Vector3{ 100.0, 100.0, 100.0 } * shade;
        record.rgb     = color;
        record.normal  = normal;
        return record;
    }

    const Vector3& v0() const noexcept
    {
        return v0_;
    }

    const Vector3& v1() const noexcept
    {
        return v1_;
    }

    const Vector3& v2() const noexcept
    {
        return v2_;
    }

  private:
    Vector3 v0_;
    Vector3 v1_;
    Vector3 v2_;
};
} // namespace toy_tracer

#endif
