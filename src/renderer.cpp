#include <optional>
#include <toy_tracer/camera.hpp>
#include <toy_tracer/ray.hpp>
#include <toy_tracer/renderer.hpp>

using toy_tracer::Renderer;
using Vector3 = toy_tracer::math::Vector<float, 3>;

void Renderer::render(void* buffer, size_t size)
{
    if (camera_ == nullptr) {
        return;
    }

    Vector3 origin          = camera_->origin();
    float viewportWidth     = camera_->viewportWidth();
    float viewportHeight    = camera_->viewportHeight();
    float focalLength       = camera_->focalLength();
    Vector3 lowerLeftCorner = origin - Vector3{ viewportWidth / 2.0f, viewportHeight / 2.0f, -focalLength };

    for (int w = 0; w < width_; ++w) {
        for (int h = height_ - 1; h >= 0; --h) {
            // normalize pixel coordinates
            float u            = static_cast<float>(w) / static_cast<float>(width_ - 1);
            float v            = static_cast<float>(h) / static_cast<float>(height_ - 1);
            Vector3 direction  = (lowerLeftCorner + Vector3{ u * viewportWidth, v * viewportHeight, 0.0f }) - origin;
            Vector3 normalized = direction / math::length(direction);
            Ray ray(origin, normalized);

            std::optional<HitRecord> hitRecord;
            for (Renderable* r : renderables_) {
                for (std::size_t count = 0; count < r->hitableCount(); ++count) {
                    HitRecord newHitRecord;
                    if (r->hitableAt(count)->hit(ray, r->vertexShader(), newHitRecord)) {
                        if (!hitRecord.has_value() || newHitRecord.distance < hitRecord->distance) {
                            hitRecord = newHitRecord;
                        }
                    }
                }
            }

            size_t index = (h * width_ + w) * 3;
            if (index >= size) {
                return;
            }
            if (!hitRecord.has_value()) {
                static_cast<std::uint8_t*>(buffer)[index + 0] = 0;
                static_cast<std::uint8_t*>(buffer)[index + 1] = 0;
                static_cast<std::uint8_t*>(buffer)[index + 2] = 0;
            } else {
                static_cast<std::uint8_t*>(buffer)[index + 0] = static_cast<std::uint8_t>(hitRecord->rgb[0]);
                static_cast<std::uint8_t*>(buffer)[index + 1] = static_cast<std::uint8_t>(hitRecord->rgb[1]);
                static_cast<std::uint8_t*>(buffer)[index + 2] = static_cast<std::uint8_t>(hitRecord->rgb[2]);
            }
        }
    }
}
