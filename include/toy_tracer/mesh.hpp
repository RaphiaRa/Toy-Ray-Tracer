#ifndef TOY_TRACER_MESH_HPP
#define TOY_TRACER_MESH_HPP

#include "math.hpp"
#include "renderable.hpp"
#include "scene_node.hpp"
#include "scene_object.hpp"
#include "triangle.hpp"
#include "vertex_shader.hpp"

#include <fstream>

namespace toy_tracer
{
class Mesh : public SceneObject, public Renderable {

    class VertexTransformator : public VertexShader {
        using Vector3 = math::Vector<float, 3>;
        using Matrix3 = math::Matrix<float, 3, 3>;

      public:
        VertexTransformator()
                : translate_{ 0.0, 0.0, 0.0 },
                  scale_{ 1.0, 1.0, 1.0 },
                  rot_{ Vector3{ 1.0, 0.0, 0.0 }, Vector3{ 0.0, 1.0, 0.0 }, Vector3{ 0.0, 0.0, 1.0 } }
        {
        }

        VertexTransformator(const Vector3& translate, const Vector3& scale, const Matrix3& rot) noexcept
                : translate_(translate), scale_(scale), rot_(rot)
        {
        }

        Vector3 perform(const Vector3& vertex) const noexcept override
        {
            Vector3 rotated = rot_ * vertex;
            return (Vector3{ rotated[0] * scale_[0], rotated[1] * scale_[1], rotated[2] * scale_[2] } + translate_);
        }

      private:
        Vector3 translate_;
        Vector3 scale_;
        Matrix3 rot_;
    };

  public:
    Mesh() = default;

    Mesh(std::vector<Triangle> triangles)
            : triangles_(std::move(triangles)), vertexTransformator_(), node_(nullptr)
    {
    }

    static Mesh fromStlFile(const std::string& filename)
    {
        using Vector3 = math::Vector<float, 3>;
        using Matrix3 = math::Matrix<float, 3, 3>;
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

    const Hitable* hitableAt(std::size_t index) const noexcept override
    {
        return &triangles_[index];
    }

    std::size_t hitableCount() const noexcept override
    {
        return triangles_.size();
    }

    const VertexShader& vertexShader() const noexcept override
    {
        return vertexTransformator_;
    }

    void notifyNodeUpdated() override
    {
        vertexTransformator_ = VertexTransformator(node_->absPos(), node_->absScale(), node_->absRot());
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
    VertexTransformator vertexTransformator_;
    SceneNode* node_;
};
} // namespace toy_tracer

#endif
