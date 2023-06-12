#ifndef toy_tracer_TRIANGLE_HPP
#define toy_tracer_TRIANGLE_HPP

#include "hitable.hpp"
#include "math.hpp"
#include "ray.hpp"

namespace toy_tracer
{
class Triangle : public Hitable {
  public:
    using Vector3 = math::Vector<float, 3>;
    using Matrix3 = math::Matrix<float, 3, 3>;

    Triangle() = default;

    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2)
            : v0_(v0), v1_(v1), v2_(v2)
    {
    }

    bool hit(const Ray& ray, const VertexShader& shader, HitRecord& record) const override
    {
        Vector3 v0 = shader.perform(v0_);
        Vector3 v1 = shader.perform(v1_);
        Vector3 v2 = shader.perform(v2_);

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;
        Vector3 D  = ray.direction();
        Vector3 O  = ray.origin();

        // Determinant
        float det = math::dot(math::cross(e0, e1), -D);
        if (det < 0)
            return false;
        if (det < std::numeric_limits<float>::epsilon())
            return false;

        // Solve (O - v0) = (e0, e1, -D) * (u, v, t)
        // using Cramer's rule
        Vector3 Y = O - v0;
        Vector3 X = Vector3{
            math::dot(math::cross(Y, e1), -D),
            math::dot(math::cross(e0, Y), -D),
            math::dot(math::cross(e0, e1), Y),
        } / det;
        if (X[0] < 0 || X[1] < 0 || X[0] + X[1] > 1)
            return false;
        record.distance = X[2];

        // shade
        Vector3 normal = math::normalize(math::cross(e0, e1));
        float shade    = math::dot(normal, -D);
        Vector3 color  = Vector3{ 50.0, 50.0, 50.0 } + Vector3{ 100.0, 100.0, 100.0 } * shade;
        record.rgb     = { static_cast<uint8_t>(color[0]), static_cast<uint8_t>(color[1]), static_cast<uint8_t>(color[2]) };
        return true;
    }

  private:
    math::Vector<float, 3> v0_;
    math::Vector<float, 3> v1_;
    math::Vector<float, 3> v2_;
};
} // namespace toy_tracer

#endif
