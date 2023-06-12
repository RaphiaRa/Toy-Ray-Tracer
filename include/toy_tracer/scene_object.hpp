#ifndef toy_tracer_SCENE_OBJECT_HPP
#define toy_tracer_SCENE_OBJECT_HPP

namespace toy_tracer
{

class SceneNode;
class SceneObject {
  protected:
    virtual void notifyNodeUpdated()             = 0;
    virtual void notifyAttached(SceneNode* node) = 0;
    virtual void notifyDetached()                = 0;

  public:
    virtual ~SceneObject() = default;
    friend class SceneNode;
};
} // namespace toy_tracer

#endif
