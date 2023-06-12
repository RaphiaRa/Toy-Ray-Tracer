#ifndef toy_tracer_CAMERA_HPP
#define toy_tracer_CAMERA_HPP

#include "math.hpp"
#include "scene_node.hpp"
#include "scene_object.hpp"

namespace toy_tracer
{
class Camera final : public SceneObject {
    using Vector3 = math::Vector<float, 3>;

  public:
    Camera(float viewportWidth, float viewportHeight, float focalLength = 1.0)
            : viewportWidth_(viewportWidth),
              viewportHeight_(viewportHeight),
              focalLength_(focalLength),
              origin_{ 0.0f, 0.0f, 0.0f },
              node_(nullptr)
    {
    }

    float viewportWidth() const
    {
        return viewportWidth_;
    }

    float viewportHeight() const
    {
        return viewportHeight_;
    }

    float focalLength() const
    {
        return focalLength_;
    }

    const math::Vector<float, 3>& origin() const
    {
        return origin_;
    }

    void notifyAttached(SceneNode* node) override
    {
        node_ = node;
    }

    void notifyDetached() override
    {
        node_ = nullptr;
    }

    void notifyNodeUpdated() override
    {
        origin_ = node_->pos();
    }

  private:
    float viewportWidth_;
    float viewportHeight_;
    float focalLength_;
    Vector3 origin_;
    SceneNode* node_;
};
} // namespace toy_tracer
#endif
