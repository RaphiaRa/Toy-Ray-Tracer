#ifndef TOY_TRACER_WORLD_HPP
#define TOY_TRACER_WORLD_HPP

#include "ray.hpp"
#include "renderable.hpp"
#include "scene_node.hpp"

#include <random>

namespace toy_tracer
{

inline math::Vector<float, 3> random_unit() noexcept
{
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    static std::mt19937 gen;
    using Vector3 = math::Vector<float, 3>;
    Vector3 p;
    do {
        p = Vector3{ dist(gen), dist(gen), dist(gen) };
    } while (math::length(p) >= 1.0f);
    return p;
}

class World : public SceneGraphObserver {
  public:
    using ColorVector = math::Vector<float, 3>;
    ~World()          = default;
    World()           = default;

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

    std::optional<HitRecord>
    hit_renderables(const toy_tracer::Ray& ray) const noexcept
    {
        std::optional<HitRecord> hitRecord = std::nullopt;
        for (const auto& renderable : renderables_) {
            if (auto record = renderable->hit(ray)) {
                if (!hitRecord || record->distance < hitRecord->distance) {
                    hitRecord = record;
                }
            }
        }
        return hitRecord;
    }

    ColorVector
    recursive_hit(const toy_tracer::Ray& ray, std::size_t depth = 30) const noexcept
    {
        using Vector3 = math::Vector<float, 3>;
        if (depth == 0) {
            return { 0.0f, 0.0f, 0.0f };
        }

        if (auto hitRecord = hit_renderables(ray)) {
            Vector3 target = ray.at(hitRecord->distance) + math::normalize(hitRecord->normal) + random_unit();
            Ray newRay(ray.at(hitRecord->distance), target - ray.at(hitRecord->distance));
            return 0.6f * recursive_hit(newRay, depth - 1);
        }
        return { 255.0f, 255.0f, 255.0f };
    }

    ColorVector hit(const Ray& ray) const noexcept
    {
        return recursive_hit(ray);
    }

  private:
    std::vector<Renderable*> renderables_;
};
} // namespace toy_tracer

#endif
