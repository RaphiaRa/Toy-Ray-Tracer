#ifndef TOY_TRACER_SCENE_NODE_HPP
#define TOY_TRACER_SCENE_NODE_HPP
#include "math.hpp"
#include "scene_object.hpp"

#include <functional>
#include <list>
#include <string>
#include <string_view>
#include <vector>

namespace toy_tracer
{
class SceneNode;
class SceneGraph;

class SceneNode final {
    using Vector3 = math::Vector<float, 3>;
    using Matrix3 = math::Matrix<float, 3, 3>;

  public:
    enum class UpdateType : unsigned int {
        none       = 0,
        pos        = (1 << 1),
        childNodes = (1 << 2),
    };

    friend inline UpdateType operator|(UpdateType lh, UpdateType rh) { return static_cast<UpdateType>(static_cast<int>(lh) | static_cast<int>(rh)); }
    friend inline UpdateType operator&(UpdateType lh, UpdateType rh) { return static_cast<UpdateType>(static_cast<int>(lh) & static_cast<int>(rh)); }

    SceneNode(std::string_view id = "Node") noexcept;

    const std::string& id() const noexcept;

    void setPos(const Vector3& pos);
    void translate(const Vector3& v);
    void setScale(const Vector3& scale);
    void scale(const Vector3& scale);
    void setRot(const Matrix3& rot);
    void rotate(const Matrix3& rot);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void setIsFlipped(bool isFlipped);
    void flip();
    void setIsVisible(bool isVisible);

    const Vector3& absPos() const { return absPos_; }
    const Vector3& pos() const { return pos_; }
    const Vector3& scale() const { return scale_; }
    const Vector3& absScale() const { return absScale_; }
    const Matrix3& rot() const { return rot_; }
    const Matrix3& absRot() const { return absRot_; }
    bool isFlipped() const { return isFlipped_; }
    bool absIsFlipped() const { return absIsFlipped_; }
    bool isVisible() const { return isVisible_; }
    bool absIsVisible() const { return absIsVisible_; }

    void attach(SceneObject* obj);
    void detach(SceneObject* obj);
    void attach(SceneNode* node);
    void detach(SceneNode* node);
    void update();

  private:
    friend class SceneGraph;
    SceneNode(SceneGraph* graph, std::string_view id) noexcept;
    void notifyAttached(SceneNode* parent);
    void notifyDetached();
    void notifyNeedsUpdate(UpdateType type = UpdateType::pos | UpdateType::childNodes);

    void setGraph(SceneGraph* graph);
    void unsetGraph();
    SceneGraph* graph();

    std::string id_          = "Node";
    SceneNode* parent_       = nullptr;
    SceneGraph* graph_       = nullptr;
    UpdateType neededUpdate_ = UpdateType::none;

    Vector3 pos_    = { { 0.0, 0.0, 0.0 } };
    Vector3 scale_  = { { 1.0, 1.0, 1.0 } };
    Matrix3 rot_    = { Vector3{ 1.0, 0.0, 0.0 }, Vector3{ 0.0, 1.0, 0.0 }, Vector3{ 0.0, 0.0, 1.0 } };
    bool isFlipped_ = false;
    bool isVisible_ = true;

    Vector3 absPos_    = { { 0.0, 0.0, 0.0 } };
    Vector3 absScale_  = { { 1.0, 1.0, 1.0 } };
    Matrix3 absRot_    = { Vector3{ 1.0, 0.0, 0.0 }, Vector3{ 0.0, 1.0, 0.0 }, Vector3{ 0.0, 0.0, 1.0 } };
    bool absIsFlipped_ = false;
    bool absIsVisible_ = true;

    std::list<SceneNode*> childNodes_;
    std::list<SceneObject*> sceneObjects_;
};

class SceneGraphObserver {
  public:
    virtual ~SceneGraphObserver()                 = default;
    virtual void notifyAdded(SceneObject* node)   = 0;
    virtual void notifyRemoved(SceneObject* node) = 0;
};

class SceneGraph final {
  public:
    SceneGraph() noexcept;
    void notifyAdded(SceneObject* node);
    void notifyRemoved(SceneObject* node);
    void addObserver(SceneGraphObserver* observer);
    void rmObserver(SceneGraphObserver* observer);
    SceneNode& rootNode() { return root_; }

  private:
    SceneNode root_;
    std::vector<SceneGraphObserver*> observers_;
};
} // namespace toy_tracer
#endif
