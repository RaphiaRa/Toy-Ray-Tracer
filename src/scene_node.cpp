#include <cassert>
#include <toy_tracer/scene_node.hpp>

using toy_tracer::SceneGraph;
using toy_tracer::SceneNode;
using Vector3 = toy_tracer::math::Vector<float, 3>;
using Vector2 = toy_tracer::math::Vector<float, 2>;
using Matrix3 = toy_tracer::math::Matrix<float, 3, 3>;

SceneNode::SceneNode(std::string_view id) noexcept
        : id_(id) {}

SceneNode::SceneNode(SceneGraph* graph, std::string_view id) noexcept
        : id_(id), graph_(graph) {}

const std::string& SceneNode::id() const noexcept
{
    return id_;
}

void SceneNode::setPos(const Vector3& pos)
{
    pos_ = pos;
    notifyNeedsUpdate();
}

void SceneNode::translate(const Vector3& v)
{
    pos_ += v;
    notifyNeedsUpdate();
}

void SceneNode::setScale(const Vector3& scale)
{
    scale_ = scale;
    notifyNeedsUpdate();
}

void SceneNode::scale(const Vector3& scale)
{
    scale_[0] *= scale[0];
    scale_[1] *= scale[1];
    scale_[2] *= scale[2];
    notifyNeedsUpdate();
}

void SceneNode::setRot(const math::Matrix<float, 3, 3>& rot)
{
    rot_ = rot;
    notifyNeedsUpdate();
}

void SceneNode::rotate(const math::Matrix<float, 3, 3>& rot)
{
    rot_ *= rot;
    notifyNeedsUpdate();
}

void SceneNode::rotateX(float angle)
{
    Matrix3 rot = Matrix3{
        Vector3{ 1, 0, 0 },
        Vector3{ 0, std::cos(angle), -std::sin(angle) },
        Vector3{ 0, std::sin(angle), std::cos(angle) },
    };
    rotate(rot);
}

void SceneNode::rotateY(float angle)
{
    Matrix3 rot = Matrix3{
        Vector3{ std::cos(angle), 0, std::sin(angle) },
        Vector3{ 0, 1, 0 },
        Vector3{ -std::sin(angle), 0, std::cos(angle) },
    };
    rotate(rot);
}

void SceneNode::rotateZ(float angle)
{
    Matrix3 rot = Matrix3{
        Vector3{ std::cos(angle), -std::sin(angle), 0 },
        Vector3{ std::sin(angle), std::cos(angle), 0 },
        Vector3{ 0, 0, 1 },
    };
    rotate(rot);
}

void SceneNode::setIsFlipped(bool isFlipped)
{
    isFlipped_ = isFlipped;
    notifyNeedsUpdate();
}

void SceneNode::flip()
{
    isFlipped_ = !isFlipped_;
    notifyNeedsUpdate();
}

void SceneNode::setIsVisible(bool isVisible)
{
    isVisible_ = isVisible;
    notifyNeedsUpdate();
}

void SceneNode::attach(SceneObject* obj)
{
    sceneObjects_.push_back(obj);
    obj->notifyAttached(this);
    notifyNeedsUpdate(UpdateType::childNodes);
}

void SceneNode::detach(SceneObject* obj)
{
    obj->notifyDetached();
    sceneObjects_.remove(obj);
    notifyNeedsUpdate(UpdateType::childNodes);
}

void SceneNode::attach(SceneNode* node)
{
    childNodes_.push_back(node);
    node->notifyAttached(this);
    notifyNeedsUpdate(UpdateType::childNodes);
}

void SceneNode::detach(SceneNode* node)
{
    node->notifyDetached();
    childNodes_.remove(node);
    notifyNeedsUpdate(UpdateType::childNodes);
}

void SceneNode::notifyAttached(SceneNode* parent)
{
    assert(parent_ == nullptr && "A scene node can only be attached to one node");
    parent_ = parent;
    this->setGraph(parent->graph());
}

void SceneNode::notifyDetached()
{
    parent_ = nullptr;
    this->unsetGraph();
}

void SceneNode::notifyNeedsUpdate(UpdateType type)
{
    if ((neededUpdate_ & UpdateType::pos) == UpdateType::none && (type & UpdateType::pos) == UpdateType::pos) {
        for (auto node : childNodes_)
            node->notifyNeedsUpdate(UpdateType::pos);
        neededUpdate_ = neededUpdate_ | UpdateType::pos;
    }
    if ((neededUpdate_ & UpdateType::childNodes) == UpdateType::none && (type & UpdateType::childNodes) == UpdateType::childNodes) {
        if (parent_)
            parent_->notifyNeedsUpdate(UpdateType::childNodes);
        neededUpdate_ = neededUpdate_ | UpdateType::childNodes;
    }
}

void SceneNode::update()
{
    if (neededUpdate_ == UpdateType::none)
        return;
    if ((neededUpdate_ & UpdateType::pos) != UpdateType::none) {
        if (!parent_) // if this is the root node
        {
            absPos_       = pos_;
            absScale_     = scale_;
            absRot_       = rot_;
            absIsFlipped_ = isFlipped_;
            absIsVisible_ = isVisible_;

        } else {
            const Vector3 scale = parent_->absScale_;
            //const float f       = (parent_->absIsFlipped_ ? -1 : 1);
            Matrix3 rot         = parent_->absRot_;

            absPos_       = parent_->absPos_ + rot * pos_;
            absPos_       = parent_->absPos_ + pos_;
            absScale_     = { scale[0] * scale_[0], scale[1] * scale_[1], scale[2] * scale_[2] };
            absRot_       = parent_->rot_ * rot_;
            absIsFlipped_ = !(parent_->absIsFlipped_ == isFlipped_);
            absIsVisible_ = parent_->absIsVisible_ && isVisible_;
        }
        for (auto obj : sceneObjects_)
            obj->notifyNodeUpdated();
        for (auto node : childNodes_)
            node->update();
        neededUpdate_ = UpdateType::none;
    } else if ((neededUpdate_ & UpdateType::childNodes) != UpdateType::none) {
        for (auto obj : sceneObjects_)
            obj->notifyNodeUpdated();
        for (auto node : childNodes_)
            node->update();
        neededUpdate_ = UpdateType::none;
    }
}

void SceneNode::setGraph(SceneGraph* graph)
{
    graph_ = graph;
    if (graph_) {
        for (auto obj : sceneObjects_)
            graph_->notifyAdded(obj);
        for (auto node : childNodes_)
            node->setGraph(graph);
    }
}

void SceneNode::unsetGraph()
{
    if (graph_) {
        for (auto obj : sceneObjects_)
            graph_->notifyRemoved(obj);
        for (auto node : childNodes_)
            node->unsetGraph();
    }
    graph_ = nullptr;
}

SceneGraph* SceneNode::graph()
{
    return graph_;
}

SceneGraph::SceneGraph() noexcept
        : root_(this, "root") {}

void SceneGraph::notifyAdded(SceneObject* node)
{
    for (auto obs : observers_)
        obs->notifyAdded(node);
}

void SceneGraph::notifyRemoved(SceneObject* node)
{
    for (auto obs : observers_)
        obs->notifyRemoved(node);
}

void SceneGraph::addObserver(SceneGraphObserver* observer)
{
    observers_.push_back(observer);
}

void SceneGraph::rmObserver(SceneGraphObserver* observer)
{
    auto iter = std::find(observers_.begin(), observers_.end(), observer);
    assert(iter != observers_.end() && "Observer to remove must exist");
    observers_.erase(iter);
}
