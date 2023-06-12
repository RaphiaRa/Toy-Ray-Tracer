#ifndef toy_tracer_RENDERER_HPP
#define toy_tracer_RENDERER_HPP

#include "camera.hpp"
#include "ray.hpp"
#include "renderable.hpp"
#include "scene_node.hpp"

#include <vector>

namespace toy_tracer
{
class Renderer final : public SceneGraphObserver {
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

    void notifyAdded(SceneObject* node) override
    {
        if (auto renderable = dynamic_cast<Renderable*>(node)) {
            renderables_.push_back(renderable);
        }
    }

    void notifyRemoved(SceneObject* node) override
    {
        if (auto renderable = dynamic_cast<Renderable*>(node)) {
            auto it = std::find(renderables_.begin(), renderables_.end(), renderable);
            if (it != renderables_.end()) {
                renderables_.erase(it);
            }
        }
    }

    /**
     * @brief Render the scene to the given buffer
     */
    void render(void* buffer, size_t size);

  private:
    int width_;
    int height_;
    const Camera* camera_;
    std::vector<Renderable*> renderables_;
};
} // namespace toy_tracer

#endif
