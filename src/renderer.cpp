#include <optional>
#include <random>
#include <thread>
#include <toy_tracer/camera.hpp>
#include <toy_tracer/ray.hpp>
#include <toy_tracer/renderer.hpp>

using toy_tracer::Renderer;
using Vector3     = toy_tracer::math::Vector<float, 3>;
using ColorVector = toy_tracer::math::Vector<float, 3>;

inline float random_float()
{
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    static std::mt19937 gen;
    return dist(gen);
}

void Renderer::render(void* buffer, size_t size, const World& world) const noexcept
{
    if (camera_ == nullptr) {
        return;
    }

    Vector3 origin          = camera_->origin();
    float viewportWidth     = camera_->viewportWidth();
    float viewportHeight    = camera_->viewportHeight();
    float focalLength       = camera_->focalLength();
    Vector3 lowerLeftCorner = origin - Vector3{ viewportWidth / 2.0f, viewportHeight / 2.0f, -focalLength };
    const int samples       = 100;

    auto task = [&](int thread, int thread_count) {
        for (int w = thread; w < width_; w += thread_count) {
            for (int h = height_ - 1; h >= 0; --h) {
                ColorVector color = { 0, 0, 0 };
                for (int s = 0; s < samples; ++s) {
                    // normalize pixel coordinates
                    float u            = (static_cast<float>(w) + random_float()) / static_cast<float>(width_ - 1);
                    float v            = (static_cast<float>(h) + random_float()) / static_cast<float>(height_ - 1);
                    Vector3 direction  = (lowerLeftCorner + Vector3{ u * viewportWidth, v * viewportHeight, 0.0f }) - origin;
                    Vector3 normalized = direction / math::length(direction);
                    Ray ray(origin, direction);

                    std::optional<HitRecord> hitRecord;

                    color += world.hit(ray);
                }
                color /= static_cast<float>(samples);

                size_t index = (h * width_ + w) * 3;
                if (index >= size) {
                    return;
                }

                static_cast<std::uint8_t*>(buffer)[index + 0] = color[0];
                static_cast<std::uint8_t*>(buffer)[index + 1] = color[1];
                static_cast<std::uint8_t*>(buffer)[index + 2] = color[2];
            }
        }
    };
    std::thread t[8];
    for (int i = 0; i < 8; ++i) {
        t[i] = std::thread(task, i, 8);
    }
    for (int i = 0; i < 8; ++i) {
        t[i].join();
    }
}
