#ifndef TOY_TRACER_VERTEX_SHADER_HPP
#define TOY_TRACER_VERTEX_SHADER_HPP

#include "math.hpp"

namespace toy_tracer
{
class VertexShader {
    using Vector3 = math::Vector<float, 3>;

  public:
    virtual ~VertexShader()                                       = default;
    virtual Vector3 perform(const Vector3& vertex) const noexcept = 0;
};

} // namespace toy_tracer

#endif
