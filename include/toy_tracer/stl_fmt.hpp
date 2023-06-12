#ifndef TOY_TRACER_STL_FMT_HPP
#define TOY_TRACER_STL_FMT_HPP

#include <array>
#include <fstream>
#include <vector>
#include "triangle.hpp"

namespace toy_tracer
{
namespace stl_fmt
{
using Vector3 = std::array<float, 3>;

struct Header {
    char header[80];
    std::uint32_t num_triangles;
};

struct Triangle {
    Vector3 normal;
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;
};

class FileReader {
  public:
    FileReader(const char* filename)
            : file_(filename, std::ios::in | std::ios::binary)
    {
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
    }

    ~FileReader();

    void read(Header& header, std::vector<Triangle>& triangles)
    {
        // parse STL mesh format
        file_.read(reinterpret_cast<char*>(&header), sizeof(Header));
        triangles.resize(header.num_triangles);
        for (auto& triangle : triangles) {
            file_.read(reinterpret_cast<char*>(&triangle), sizeof(Triangle));
        }
    }

  private:
    std::ifstream file_;
};

} // namespace stl_fmt
} // namespace toy_tracer

#endif
