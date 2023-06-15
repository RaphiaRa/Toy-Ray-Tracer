#ifndef TOY_TRACER_VERTEX_MAP_HPP
#define TOY_TRACER_VERTEX_MAP_HPP

#include "math.hpp"

namespace toy_tracer
{
class VertexMap {
    using Vector3 = math::Vector<float, 3>;

  public:
    virtual ~VertexMap() = default;

    virtual Vector3 map(const Vector3& vertex) const noexcept = 0;
};

} // namespace toy_tracer

#endif
