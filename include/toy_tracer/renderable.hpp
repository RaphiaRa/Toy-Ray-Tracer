#ifndef TOY_TRACER_RENDERABLE_HPP
#define TOY_TRACER_RENDERABLE_HPP

#include "hitable.hpp"
#include "vertex_shader.hpp"

namespace toy_tracer
{
class Renderable {
  public:
    virtual ~Renderable()                                              = default;
    virtual std::size_t hitableCount() const noexcept                  = 0;
    virtual const Hitable* hitableAt(std::size_t index) const noexcept = 0;
    virtual const VertexShader& vertexShader() const noexcept          = 0;
};
} // namespace toy_tracer

#endif
