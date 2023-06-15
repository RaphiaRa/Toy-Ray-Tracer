#ifndef TOY_TRACER_RENDERER_HPP
#define TOY_TRACER_RENDERER_HPP

#include "camera.hpp"
#include "ray.hpp"
#include "renderable.hpp"
#include "scene_node.hpp"
#include "world.hpp"

#include <vector>

namespace toy_tracer
{
class Renderer final {
  public:
    ~Renderer() = default;
    Renderer(int width, int height)
            : width_(width), height_(height), camera_(nullptr)
    {
    }

    void setCamera(const Camera& camera) noexcept
    {
        camera_ = &camera;
    }

    /**
     * @brief Render the scene to the given buffer
     */
    void render(void* buffer, size_t size, const World& world) const noexcept;

  private:
    int width_;
    int height_;
    const Camera* camera_;
    std::vector<Renderable*> renderables_;
};
} // namespace toy_tracer

#endif
