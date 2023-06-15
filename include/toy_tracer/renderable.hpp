#ifndef TOY_TRACER_RENDERABLE_HPP
#define TOY_TRACER_RENDERABLE_HPP

#include "hit_record.hpp"
#include "ray.hpp"

#include <optional>

namespace toy_tracer
{

class Renderable {
  public:
    virtual ~Renderable()                                               = default;
    virtual std::optional<HitRecord> hit(const Ray& ray) const noexcept = 0;
    /**
    virtual std::size_t triangleCount() const noexcept                                                                                            = 0;
    virtual const Triangle* triangleAt(std::size_t index) const noexcept                                                                          = 0;
    virtual const math::Vector<float, 3>& vertexAt(std::size_t vertexIndex) const noexcept                                                        = 0;
    virtual const math::Vector<float, 3>& vertexNormalAt() virtual const math::Vector<float, 3>& normalAt(std::size_t normalIndex) const noexcept = 0;
    virtual const VertexShader& vertexShader() const noexcept                                                                                     = 0;
    **/
};
} // namespace toy_tracer

#endif
