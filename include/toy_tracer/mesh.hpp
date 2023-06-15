#ifndef TOY_TRACER_MESH_HPP
#define TOY_TRACER_MESH_HPP

#include "math.hpp"
#include "renderable.hpp"
#include "scene_node.hpp"
#include "scene_object.hpp"
#include "triangle.hpp"
#include "vertex_map.hpp"

#include <fstream>

namespace toy_tracer
{
class Mesh : public SceneObject, public Renderable {
    using Vector3 = math::Vector<float, 3>;

    class Map : public VertexMap {
        using Vector3 = Mesh::Vector3;
        using Vector4 = math::Vector<float, 4>;
        using Matrix3 = math::Matrix<float, 3, 3>;

      public:
        Map()
                : t_{}
        {
        }

        Map(const Vector3& translate, const Vector3& scale, const Matrix3& rot) noexcept
                : t_{ Vector4{ rot[0][0] * scale[0], rot[0][1] * scale[1], rot[0][2] * scale[2], translate[0] },
                      Vector4{ rot[1][0] * scale[0], rot[1][1] * scale[1], rot[1][2] * scale[2], translate[1] },
                      Vector4{ rot[2][0] * scale[0], rot[2][1] * scale[1], rot[2][2] * scale[2], translate[2] } }
        {
        }

        Vector3 map(const Vector3& vertex) const noexcept override
        {
            return t_ * Vector4{ vertex[0], vertex[1], vertex[2], 1.0f };
        }

      private:
        math::Matrix<float, 3, 4> t_;
    };

    class BoundingBox {
      public:
        using Vector3 = Mesh::Vector3;

        BoundingBox() = default;

        BoundingBox(Vector3 min, Vector3 max)
        {
            triangles_.emplace_back(Vector3{ min[0], min[1], min[2] }, Vector3{ min[0], max[1], min[2] }, Vector3{ max[0], min[1], min[2] });
            triangles_.emplace_back(Vector3{ max[0], min[1], min[2] }, Vector3{ min[0], max[1], min[2] }, Vector3{ max[0], max[1], min[2] });
            triangles_.emplace_back(Vector3{ min[0], min[1], max[2] }, Vector3{ max[0], min[1], max[2] }, Vector3{ min[0], max[1], max[2] });
            triangles_.emplace_back(Vector3{ max[0], min[1], max[2] }, Vector3{ max[0], max[1], max[2] }, Vector3{ min[0], max[1], max[2] });
            triangles_.emplace_back(Vector3{ min[0], min[1], min[2] }, Vector3{ min[0], min[1], max[2] }, Vector3{ min[0], max[1], min[2] });
            triangles_.emplace_back(Vector3{ min[0], max[1], min[2] }, Vector3{ min[0], min[1], max[2] }, Vector3{ min[0], max[1], max[2] });
            triangles_.emplace_back(Vector3{ max[0], min[1], min[2] }, Vector3{ max[0], max[1], min[2] }, Vector3{ max[0], min[1], max[2] });
            triangles_.emplace_back(Vector3{ max[0], min[1], max[2] }, Vector3{ max[0], max[1], min[2] }, Vector3{ max[0], max[1], max[2] });
            triangles_.emplace_back(Vector3{ min[0], min[1], min[2] }, Vector3{ max[0], min[1], min[2] }, Vector3{ min[0], min[1], max[2] });
            triangles_.emplace_back(Vector3{ max[0], min[1], min[2] }, Vector3{ max[0], min[1], max[2] }, Vector3{ min[0], min[1], max[2] });
            triangles_.emplace_back(Vector3{ min[0], max[1], min[2] }, Vector3{ min[0], max[1], max[2] }, Vector3{ max[0], max[1], min[2] });
            triangles_.emplace_back(Vector3{ max[0], max[1], min[2] }, Vector3{ min[0], max[1], max[2] }, Vector3{ max[0], max[1], max[2] });
        }

        std::optional<HitRecord> hit(const Ray& ray, const VertexMap& map) const noexcept
        {
            for (auto& triangle : triangles_) {
                std::optional<HitRecord> record;
                if ((record = triangle.hit(ray, map)))
                    return record;
            }
            return std::nullopt;
        }

      private:
        std::vector<Triangle> triangles_;
    };

  public:
    Mesh() = default;

    Mesh(std::vector<Triangle> triangles)
            : triangles_(), vertexMap_(), node_(nullptr)
    {
        Vector3 max;
        Vector3 min;
        triangles_.reserve(triangles.size());
        for (auto& triangle : triangles) {
            max[0] = std::max(max[0], std::max(triangle.v0()[0], std::max(triangle.v1()[0], triangle.v2()[0])));
            max[1] = std::max(max[1], std::max(triangle.v0()[1], std::max(triangle.v1()[1], triangle.v2()[1])));
            max[2] = std::max(max[2], std::max(triangle.v0()[2], std::max(triangle.v1()[2], triangle.v2()[2])));
            min[0] = std::min(min[0], std::min(triangle.v0()[0], std::min(triangle.v1()[0], triangle.v2()[0])));
            min[1] = std::min(min[1], std::min(triangle.v0()[1], std::min(triangle.v1()[1], triangle.v2()[1])));
            min[2] = std::min(min[2], std::min(triangle.v0()[2], std::min(triangle.v1()[2], triangle.v2()[2])));
            triangles_.emplace_back(triangle);
        }
        boundingBox_ = BoundingBox(min, max);
    }

    static Mesh fromStlFile(const std::string& filename)
    {
        using Vector3 = math::Vector<float, 3>;
        struct Header {
            char header[80];
            uint32_t triangleCount;
        };

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + filename);
        }

        std::vector<Triangle> triangles;
        Header header;
        file.read(reinterpret_cast<char*>(&header), sizeof(Header));
        triangles.resize(header.triangleCount);
        for (auto& triangle : triangles) {
            Vector3 normal;
            Vector3 v0;
            Vector3 v1;
            Vector3 v2;
            uint16_t attributeByteCount;
            file.read(reinterpret_cast<char*>(&normal), sizeof(Vector3));
            file.read(reinterpret_cast<char*>(&v0), sizeof(Vector3));
            file.read(reinterpret_cast<char*>(&v1), sizeof(Vector3));
            file.read(reinterpret_cast<char*>(&v2), sizeof(Vector3));
            file.read(reinterpret_cast<char*>(&attributeByteCount), sizeof(uint16_t));
            triangle = Triangle(v0, v1, v2);
        }
        return Mesh(std::move(triangles));
    }

    Mesh(const Mesh&)            = delete;
    Mesh(Mesh&&)                 = default;
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&)      = default;
    ~Mesh() override             = default;

    std::optional<HitRecord> hit(const Ray& ray) const noexcept override
    {
        if (!boundingBox_.hit(ray, vertexMap_))
            return std::nullopt;

        std::optional<HitRecord> hitRecord = std::nullopt;
        for (auto& triangle : triangles_) {
            if (auto record = triangle.hit(ray, vertexMap_)) {
                if (!hitRecord || record->distance < hitRecord->distance) {
                    hitRecord = record;
                }
            }
        }
        return hitRecord;
    }

    void notifyNodeUpdated() override
    {
        vertexMap_ = Map(node_->absPos(), node_->absScale(), node_->absRot());
    }

    void notifyAttached(SceneNode* node) override
    {
        node_ = node;
    }
    void notifyDetached() override
    {
        node_ = nullptr;
    }

  private:
    std::vector<Triangle> triangles_;
    BoundingBox boundingBox_;
    Map vertexMap_;
    SceneNode* node_;
};
} // namespace toy_tracer

#endif
