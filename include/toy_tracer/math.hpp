#ifndef TOY_TRACER_MATH_H
#define TOY_TRACER_MATH_H

#include <algorithm>
#include <array>
#include <cmath>

namespace toy_tracer
{
namespace math
{
constexpr float pi          = 3.14159265358979323846f;
constexpr float degRadScale = 180.f / pi;

inline float degToRad(float deg)
{
    return deg / degRadScale;
}

inline float radToDeg(float rad)
{
    return rad * degRadScale;
}

template<typename T, std::size_t N>
using Vector = std::array<T, N>;

template<typename T, std::size_t N, std::size_t M>
using Matrix = std::array<std::array<T, M>, N>;

template<typename T, std::size_t N>
T std_norm(const toy_tracer::math::Vector<T, N>& v);

template<typename T, std::size_t N>
T length(const toy_tracer::math::Vector<T, N>& v);

template<typename T, std::size_t N>
T max_norm(const toy_tracer::math::Vector<T, N>& v);

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N>
cross(const toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w);

template<typename T, std::size_t N>
T dot(const toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w);

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N> normalize(const toy_tracer::math::Vector<T, N>& v);

} // namespace math
} // namespace toy_tracer

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N> operator+(const toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w)
{
    toy_tracer::math::Vector<T, N> r;
    for (std::size_t i = 0; i < N; ++i)
        r[i] = v[i] + w[i];
    return r;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N>& operator+=(toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w)
{
    for (std::size_t i = 0; i < N; ++i)
        v[i] += w[i];
    return v;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N>& operator-=(toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w)
{
    for (std::size_t i = 0; i < N; ++i)
        v[i] -= w[i];
    return v;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N> operator-(const toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w)
{
    toy_tracer::math::Vector<T, N> r;
    for (std::size_t i = 0; i < N; ++i)
        r[i] = v[i] - w[i];
    return r;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N> operator-(const toy_tracer::math::Vector<T, N>& v)
{
    toy_tracer::math::Vector<T, N> r;
    for (std::size_t i = 0; i < N; ++i)
        r[i] = -v[i];
    return r;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N> operator*(float l, toy_tracer::math::Vector<T, N> w)
{
    for (std::size_t i = 0; i < N; ++i)
        w[i] *= l;
    return w;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N>& operator*=(toy_tracer::math::Vector<T, N> w, float l)
{
    for (std::size_t i = 0; i < N; ++i)
        w[i] *= l;
    return w;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N> operator*(toy_tracer::math::Vector<T, N> w, float l)
{
    toy_tracer::math::Vector<T, N> v;
    for (std::size_t i = 0; i < N; ++i)
        v[i] = w[i] * l;
    return v;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N>& operator/=(toy_tracer::math::Vector<T, N> w, float l)
{
    for (std::size_t i = 0; i < N; ++i)
        w[i] /= l;
    return w;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N> operator/(const toy_tracer::math::Vector<T, N> w, float l)
{
    toy_tracer::math::Vector<T, N> v;
    for (std::size_t i = 0; i < N; ++i)
        v[i] = w[i] / l;
    return v;
}

template<typename T, std::size_t N, std::size_t M>
toy_tracer::math::Vector<T, N> operator*(const toy_tracer::math::Matrix<T, N, M>& m, const toy_tracer::math::Vector<T, M> v)
{
    toy_tracer::math::Vector<T, N> r;
    for (std::size_t i = 0; i < N; ++i)
        r[i] = toy_tracer::math::dot(m[i], v);
    return r;
}

template<typename T, std::size_t N>
T toy_tracer::math::std_norm(const toy_tracer::math::Vector<T, N>& v)
{
    return toy_tracer::math::dot(v, v);
}

template<typename T, std::size_t N>
T toy_tracer::math::length(const toy_tracer::math::Vector<T, N>& v)
{
    return std::sqrt(std_norm(v));
}

template<typename T, std::size_t N>
T toy_tracer::math::max_norm(const toy_tracer::math::Vector<T, N>& v)
{
    return abs(*std::max_element(v.cbegin(), v.cend()));
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N>
toy_tracer::math::cross(const toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w)
{
    toy_tracer::math::Vector<T, N> r;
    r[0] = v[1] * w[2] - v[2] * w[1];
    r[1] = v[2] * w[0] - v[0] * w[2];
    r[2] = v[0] * w[1] - v[1] * w[0];
    return r;
}

template<typename T, std::size_t N>
T toy_tracer::math::dot(const toy_tracer::math::Vector<T, N>& v, const toy_tracer::math::Vector<T, N>& w)
{
    typename toy_tracer::math::Vector<T, N>::value_type r = 0;
    for (std::size_t i = 0; i < N; ++i)
        r += v[i] * w[i];
    return r;
}

template<typename T, std::size_t N>
toy_tracer::math::Vector<T, N>
toy_tracer::math::normalize(const toy_tracer::math::Vector<T, N>& v)
{
    return v / length(v);
}

template<typename T, std::size_t N, std::size_t M, std::size_t P>
toy_tracer::math::Matrix<T, N, P> operator*(const toy_tracer::math::Matrix<T, N, M>& m, const toy_tracer::math::Matrix<T, M, P>& n)
{
    toy_tracer::math::Matrix<T, N, P> r = {};
    for (std::size_t i = 0; i < N; ++i)
        for (std::size_t j = 0; j < P; ++j)
            for (std::size_t k = 0; k < M; ++k)
                r[i][j] += m[i][k] * n[k][j];
    return r;
}

template<typename T, std::size_t N, std::size_t M, std::size_t P>
toy_tracer::math::Matrix<T, N, P> operator*=(toy_tracer::math::Matrix<T, N, M>& m, const toy_tracer::math::Matrix<T, M, P>& n)
{
    return m = m * n;
}

#endif
