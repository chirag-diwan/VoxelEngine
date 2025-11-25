//----------------------------------------------------------------------------------------
//
// siv::PerlinNoise
// Perlin noise library for modern C++
//
// Copyright (C) 2013-2021 Ryo Suzuki <reputeless@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//----------------------------------------------------------------------------------------
# pragma once
# include <cstdint>
# include <algorithm>
# include <array>
# include <iterator>
# include <numeric>
# include <random>
# include <type_traits>
# if __has_include(<concepts>) && defined(__cpp_concepts)
# include <concepts>
# endif
// Library major version
# define SIVPERLIN_VERSION_MAJOR 3
// Library minor version
# define SIVPERLIN_VERSION_MINOR 0
// Library revision version
# define SIVPERLIN_VERSION_REVISION 0
// Library version
# define SIVPERLIN_VERSION ((SIVPERLIN_VERSION_MAJOR * 100 * 100) + (SIVPERLIN_VERSION_MINOR * 100) + (SIVPERLIN_VERSION_REVISION))
// [[nodiscard]] for constructors
# if (201907L <= __has_cpp_attribute(nodiscard))
# define SIVPERLIN_NODISCARD_CXX20 [[nodiscard]]
# else
# define SIVPERLIN_NODISCARD_CXX20
# endif
// std::uniform_random_bit_generator concept
# if __cpp_lib_concepts
# define SIVPERLIN_CONCEPT_URBG template <std::uniform_random_bit_generator URBG>
# define SIVPERLIN_CONCEPT_URBG_ template <std::uniform_random_bit_generator URBG>
# else
# define SIVPERLIN_CONCEPT_URBG template <class URBG, std::enable_if_t<std::conjunction_v<std::is_invocable<URBG&>, std::is_unsigned<std::invoke_result_t<URBG&>>>>* = nullptr>
# define SIVPERLIN_CONCEPT_URBG_ template <class URBG, std::enable_if_t<std::conjunction_v<std::is_invocable<URBG&>, std::is_unsigned<std::invoke_result_t<URBG&>>>>*>
# endif
// arbitrary value for increasing entropy
# ifndef SIVPERLIN_DEFAULT_Y
# define SIVPERLIN_DEFAULT_Y (0.12345)
# endif
// arbitrary value for increasing entropy
# ifndef SIVPERLIN_DEFAULT_Z
# define SIVPERLIN_DEFAULT_Z (0.34567)
# endif
namespace siv
{
template <class Float>
class BasicPerlinNoise
{
public:
static_assert(std::is_floating_point_v<Float>);
///////////////////////////////////////
//
// Typedefs
//
using state_type = std::array<std::uint8_t, 256>;
using value_type = Float;
using default_random_engine = std::mt19937;
using seed_type = typename default_random_engine::result_type;
///////////////////////////////////////
//
// Constructors
//
SIVPERLIN_NODISCARD_CXX20
constexpr BasicPerlinNoise() noexcept;
SIVPERLIN_NODISCARD_CXX20
explicit BasicPerlinNoise(seed_type seed);
SIVPERLIN_CONCEPT_URBG
SIVPERLIN_NODISCARD_CXX20
explicit BasicPerlinNoise(URBG&& urbg);
///////////////////////////////////////
//
// Reseed
//
void reseed(seed_type seed);
SIVPERLIN_CONCEPT_URBG
void reseed(URBG&& urbg);
///////////////////////////////////////
//
// Serialization
//
[[nodiscard]]
constexpr const state_type& serialize() const noexcept;
constexpr void deserialize(const state_type& state) noexcept;
///////////////////////////////////////
//
// Noise (The result is in the range [-1, 1])
//
[[nodiscard]]
value_type noise1D(value_type x) const noexcept;
[[nodiscard]]
value_type noise2D(value_type x, value_type y) const noexcept;
[[nodiscard]]
value_type noise3D(value_type x, value_type y, value_type z) const noexcept;
///////////////////////////////////////
//
// Noise (The result is remapped to the range [0, 1])
//
[[nodiscard]]
value_type noise1D_01(value_type x) const noexcept;
[[nodiscard]]
value_type noise2D_01(value_type x, value_type y) const noexcept;
[[nodiscard]]
value_type noise3D_01(value_type x, value_type y, value_type z) const noexcept;
///////////////////////////////////////
//
// Octave noise (The result can be out of the range [-1, 1])
//
[[nodiscard]]
value_type octave1D(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type octave2D(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type octave3D(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
///////////////////////////////////////
//
// Octave noise (The result is clamped to the range [-1, 1])
//
[[nodiscard]]
value_type octave1D_11(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type octave2D_11(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type octave3D_11(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
///////////////////////////////////////
//
// Octave noise (The result is clamped and remapped to the range [0, 1])
//
[[nodiscard]]
value_type octave1D_01(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type octave2D_01(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type octave3D_01(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
///////////////////////////////////////
//
// Octave noise (The result is normalized to the range [-1, 1])
//
[[nodiscard]]
value_type normalizedOctave1D(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type normalizedOctave2D(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type normalizedOctave3D(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
///////////////////////////////////////
//
// Octave noise (The result is normalized and remapped to the range [0, 1])
//
[[nodiscard]]
value_type normalizedOctave1D_01(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type normalizedOctave2D_01(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
[[nodiscard]]
value_type normalizedOctave3D_01(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept;
private:
state_type m_permutation;
};
using PerlinNoise = BasicPerlinNoise<float>;

#ifdef __SSE4_1__
// SIMD-optimized version for float (requires SSE4.1)
# include <immintrin.h>

template<class Float>
class SimdPerlinNoise : public BasicPerlinNoise<Float>
{
public:
    static_assert(std::is_same_v<Float, float>, "SimdPerlinNoise is specialized for float only.");
    using Base = BasicPerlinNoise<Float>;
    using value_type = Float;
    using state_type = typename Base::state_type;
    using seed_type = typename Base::seed_type;

    SIVPERLIN_NODISCARD_CXX20
    constexpr SimdPerlinNoise() noexcept : Base() {}

    SIVPERLIN_NODISCARD_CXX20
    explicit SimdPerlinNoise(seed_type seed) : Base(seed) {}

    SIVPERLIN_CONCEPT_URBG
    SIVPERLIN_NODISCARD_CXX20
    explicit SimdPerlinNoise(URBG&& urbg) : Base(std::forward<URBG>(urbg)) {}

    void reseed(seed_type seed) { Base::reseed(seed); }

    SIVPERLIN_CONCEPT_URBG
    void reseed(URBG&& urbg) { Base::reseed(std::forward<URBG>(urbg)); }

    [[nodiscard]]
    constexpr const state_type& serialize() const noexcept { return Base::serialize(); }

    constexpr void deserialize(const state_type& state) noexcept { Base::deserialize(state); }

    // Noise functions forward to base
    [[nodiscard]] value_type noise1D(value_type x) const noexcept { return Base::noise1D(x); }
    [[nodiscard]] value_type noise2D(value_type x, value_type y) const noexcept { return Base::noise2D(x, y); }
    [[nodiscard]] value_type noise3D(value_type x, value_type y, value_type z) const noexcept { return Base::noise3D(x, y, z); }
    [[nodiscard]] value_type noise1D_01(value_type x) const noexcept { return Base::noise1D_01(x); }
    [[nodiscard]] value_type noise2D_01(value_type x, value_type y) const noexcept { return Base::noise2D_01(x, y); }
    [[nodiscard]] value_type noise3D_01(value_type x, value_type y, value_type z) const noexcept { return Base::noise3D_01(x, y, z); }
    [[nodiscard]] value_type octave1D(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave1D(x, octaves, persistence); }
    [[nodiscard]] value_type octave2D(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave2D(x, y, octaves, persistence); }
    [[nodiscard]] value_type octave3D(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave3D(x, y, z, octaves, persistence); }
    [[nodiscard]] value_type octave1D_11(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave1D_11(x, octaves, persistence); }
    [[nodiscard]] value_type octave2D_11(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave2D_11(x, y, octaves, persistence); }
    [[nodiscard]] value_type octave3D_11(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave3D_11(x, y, z, octaves, persistence); }
    [[nodiscard]] value_type octave1D_01(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave1D_01(x, octaves, persistence); }
    [[nodiscard]] value_type octave2D_01(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave2D_01(x, y, octaves, persistence); }
    [[nodiscard]] value_type octave3D_01(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::octave3D_01(x, y, z, octaves, persistence); }
    [[nodiscard]] value_type normalizedOctave1D(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::normalizedOctave1D(x, octaves, persistence); }
    [[nodiscard]] value_type normalizedOctave2D(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::normalizedOctave2D(x, y, octaves, persistence); }
    [[nodiscard]] value_type normalizedOctave3D(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::normalizedOctave3D(x, y, z, octaves, persistence); }
    [[nodiscard]] value_type normalizedOctave1D_01(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::normalizedOctave1D_01(x, octaves, persistence); }
    [[nodiscard]] value_type normalizedOctave2D_01(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::normalizedOctave2D_01(x, y, octaves, persistence); }
    [[nodiscard]] value_type normalizedOctave3D_01(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const noexcept { return Base::normalizedOctave3D_01(x, y, z, octaves, persistence); }

    // Fill noise for multiple points using SIMD (processes in groups of 4)
    void fillNoise3D(const float* xs, const float* ys, const float* zs, float* out, size_t count) const noexcept;
};

using SimdPerlinNoise = SimdPerlinNoise<float>;

template<class Float>
void SimdPerlinNoise<Float>::fillNoise3D(const float* xs, const float* ys, const float* zs, float* out, size_t count) const noexcept
{
    static_assert(std::is_same_v<Float, float>);
    const auto& perm = this->m_permutation;
    auto FadeSIMD = [](__m128 t) -> __m128 {
        __m128 t2 = _mm_mul_ps(t, t);
        __m128 t3 = _mm_mul_ps(t2, t);
        __m128 tmp = _mm_sub_ps(_mm_mul_ps(t, _mm_set1_ps(6.0f)), _mm_set1_ps(15.0f));
        tmp = _mm_mul_ps(t, tmp);
        tmp = _mm_add_ps(tmp, _mm_set1_ps(10.0f));
        return _mm_mul_ps(t3, tmp);
    };
    auto LerpSIMD = [](__m128 a, __m128 b, __m128 t) -> __m128 {
        return _mm_add_ps(a, _mm_mul_ps(_mm_sub_ps(b, a), t));
    };
    auto GradSIMD = [&](__m128i H, __m128 dx, __m128 dy, __m128 dz) -> __m128 {
        __m128i hh = _mm_and_si128(H, _mm_set1_epi32(15));
        // u = h < 8 ? x : y
        __m128i lt8 = _mm_cmplt_epi32(hh, _mm_set1_epi32(8));
        __m128 u = _mm_blendv_ps(dy, dx, _mm_castsi128_ps(lt8));
        // sign u: (h & 1) == 0 ? u : -u
        __m128i h1 = _mm_and_si128(hh, _mm_set1_epi32(1));
        __m128 sign_mask_u = _mm_castsi128_ps(_mm_slli_epi32(h1, 31));
        __m128 u_signed = _mm_xor_ps(u, sign_mask_u);
        // v
        __m128i lt4 = _mm_cmplt_epi32(hh, _mm_set1_epi32(4));
        __m128i eq12 = _mm_cmpeq_epi32(hh, _mm_set1_epi32(12));
        __m128i eq14 = _mm_cmpeq_epi32(hh, _mm_set1_epi32(14));
        __m128i eq12or14 = _mm_or_si128(eq12, eq14);
        __m128 temp_v = _mm_blendv_ps(dz, dx, _mm_castsi128_ps(eq12or14));
        __m128 v = _mm_blendv_ps(temp_v, dy, _mm_castsi128_ps(lt4));
        // sign v: (h & 2) == 0 ? v : -v
        __m128i h2 = _mm_and_si128(hh, _mm_set1_epi32(2));
        __m128i odd_v = _mm_srli_epi32(h2, 1);
        __m128 sign_mask_v = _mm_castsi128_ps(_mm_slli_epi32(odd_v, 31));
        __m128 v_signed = _mm_xor_ps(v, sign_mask_v);
        return _mm_add_ps(u_signed, v_signed);
    };

    for (size_t i = 0; i < count; i += 4) {
        size_t nn = std::min(size_t(4), count - i);
        if (nn < 4) {
            // Fallback for remainder
            for (size_t j = 0; j < nn; ++j) {
                out[i + j] = this->noise3D(xs[i + j], ys[i + j], zs[i + j]);
            }
            continue;
        }
        // SIMD for 4 points
        __m128 X = _mm_loadu_ps(xs + i);
        __m128 Y = _mm_loadu_ps(ys + i);
        __m128 Z = _mm_loadu_ps(zs + i);
        __m128 X0 = _mm_floor_ps(X);
        __m128 Y0 = _mm_floor_ps(Y);
        __m128 Z0 = _mm_floor_ps(Z);
        __m128 X1 = _mm_sub_ps(X, X0);
        __m128 Y1 = _mm_sub_ps(Y, Y0);
        __m128 Z1 = _mm_sub_ps(Z, Z0);
        __m128 u = FadeSIMD(X1);
        __m128 v = FadeSIMD(Y1);
        __m128 w = FadeSIMD(Z1);
        __m128i IX = _mm_cvttps_epi32(X0);  // Use cvttps for floor semantics with positive numbers
        __m128i IY = _mm_cvttps_epi32(Y0);
        __m128i IZ = _mm_cvttps_epi32(Z0);
        __m128i mask = _mm_set1_epi32(255);
        IX = _mm_and_si128(IX, mask);
        IY = _mm_and_si128(IY, mask);
        IZ = _mm_and_si128(IZ, mask);
        alignas(16) int ix[4], iy[4], iz[4];
        _mm_store_si128(reinterpret_cast<__m128i*>(ix), IX);
        _mm_store_si128(reinterpret_cast<__m128i*>(iy), IY);
        _mm_store_si128(reinterpret_cast<__m128i*>(iz), IZ);
        alignas(16) uint8_t h0[4], h1[4], h2[4], h3[4], h4[4], h5[4], h6[4], h7[4];
        for (int j = 0; j < 4; ++j) {
            uint8_t A = (perm[ix[j]] + static_cast<uint8_t>(iy[j])) & 255;
            uint8_t B = (perm[(ix[j] + 1) & 255] + static_cast<uint8_t>(iy[j])) & 255;
            uint8_t AA = (perm[A] + static_cast<uint8_t>(iz[j])) & 255;
            uint8_t AB = (perm[(A + 1) & 255] + static_cast<uint8_t>(iz[j])) & 255;
            uint8_t BA = (perm[B] + static_cast<uint8_t>(iz[j])) & 255;
            uint8_t BB = (perm[(B + 1) & 255] + static_cast<uint8_t>(iz[j])) & 255;
            h0[j] = perm[AA];
            h1[j] = perm[BA];
            h2[j] = perm[AB];
            h3[j] = perm[BB];
            h4[j] = perm[(AA + 1) & 255];
            h5[j] = perm[(BA + 1) & 255];
            h6[j] = perm[(AB + 1) & 255];
            h7[j] = perm[(BB + 1) & 255];
        }
        __m128i H0 = _mm_set_epi32(static_cast<int>(h0[3]), static_cast<int>(h0[2]), static_cast<int>(h0[1]), static_cast<int>(h0[0]));
        __m128i H1 = _mm_set_epi32(static_cast<int>(h1[3]), static_cast<int>(h1[2]), static_cast<int>(h1[1]), static_cast<int>(h1[0]));
        __m128i H2 = _mm_set_epi32(static_cast<int>(h2[3]), static_cast<int>(h2[2]), static_cast<int>(h2[1]), static_cast<int>(h2[0]));
        __m128i H3 = _mm_set_epi32(static_cast<int>(h3[3]), static_cast<int>(h3[2]), static_cast<int>(h3[1]), static_cast<int>(h3[0]));
        __m128i H4 = _mm_set_epi32(static_cast<int>(h4[3]), static_cast<int>(h4[2]), static_cast<int>(h4[1]), static_cast<int>(h4[0]));
        __m128i H5 = _mm_set_epi32(static_cast<int>(h5[3]), static_cast<int>(h5[2]), static_cast<int>(h5[1]), static_cast<int>(h5[0]));
        __m128i H6 = _mm_set_epi32(static_cast<int>(h6[3]), static_cast<int>(h6[2]), static_cast<int>(h6[1]), static_cast<int>(h6[0]));
        __m128i H7 = _mm_set_epi32(static_cast<int>(h7[3]), static_cast<int>(h7[2]), static_cast<int>(h7[1]), static_cast<int>(h7[0]));
        __m128 dx0 = X1, dy0 = Y1, dz0 = Z1;
        __m128 dx1 = _mm_sub_ps(X1, _mm_set1_ps(1.0f));
        __m128 dy1 = Y1, dz1 = Z1;
        __m128 dx2 = X1;
        __m128 dy2 = _mm_sub_ps(Y1, _mm_set1_ps(1.0f));
        __m128 dz2 = Z1;
        __m128 dx3 = _mm_sub_ps(X1, _mm_set1_ps(1.0f));
        __m128 dy3 = _mm_sub_ps(Y1, _mm_set1_ps(1.0f));
        __m128 dz3 = Z1;
        __m128 dx4 = X1, dy4 = Y1;
        __m128 dz4 = _mm_sub_ps(Z1, _mm_set1_ps(1.0f));
        __m128 dx5 = _mm_sub_ps(X1, _mm_set1_ps(1.0f));
        __m128 dy5 = Y1;
        __m128 dz5 = _mm_sub_ps(Z1, _mm_set1_ps(1.0f));
        __m128 dx6 = X1;
        __m128 dy6 = _mm_sub_ps(Y1, _mm_set1_ps(1.0f));
        __m128 dz6 = _mm_sub_ps(Z1, _mm_set1_ps(1.0f));
        __m128 dx7 = _mm_sub_ps(X1, _mm_set1_ps(1.0f));
        __m128 dy7 = _mm_sub_ps(Y1, _mm_set1_ps(1.0f));
        __m128 dz7 = _mm_sub_ps(Z1, _mm_set1_ps(1.0f));
        __m128 p0 = GradSIMD(H0, dx0, dy0, dz0);
        __m128 p1 = GradSIMD(H1, dx1, dy1, dz1);
        __m128 p2 = GradSIMD(H2, dx2, dy2, dz2);
        __m128 p3 = GradSIMD(H3, dx3, dy3, dz3);
        __m128 p4 = GradSIMD(H4, dx4, dy4, dz4);
        __m128 p5 = GradSIMD(H5, dx5, dy5, dz5);
        __m128 p6 = GradSIMD(H6, dx6, dy6, dz6);
        __m128 p7 = GradSIMD(H7, dx7, dy7, dz7);
        __m128 q0 = LerpSIMD(p0, p1, u);
        __m128 q1 = LerpSIMD(p2, p3, u);
        __m128 q2 = LerpSIMD(p4, p5, u);
        __m128 q3 = LerpSIMD(p6, p7, u);
        __m128 r0 = LerpSIMD(q0, q1, v);
        __m128 r1 = LerpSIMD(q2, q3, v);
        __m128 result = LerpSIMD(r0, r1, w);
        _mm_storeu_ps(out + i, result);
    }
}

#endif  // __SSE4_1__

namespace perlin_detail
{
////////////////////////////////////////////////
//
// These functions are provided for consistency.
// You may get different results from std::shuffle() with different standard library implementations.
//
SIVPERLIN_CONCEPT_URBG
[[nodiscard]]
inline std::uint64_t Random(const std::uint64_t max, URBG&& urbg)
{
return (urbg() % (max + 1));
}
template <class RandomIt, class URBG>
inline void Shuffle(RandomIt first, RandomIt last, URBG&& urbg)
{
if (first == last)
{
return;
}
using difference_type = typename std::iterator_traits<RandomIt>::difference_type;
for (RandomIt it = first + 1; it < last; ++it)
{
const std::uint64_t n = static_cast<std::uint64_t>(it - first);
std::iter_swap(it, first + static_cast<difference_type>(Random(n, std::forward<URBG>(urbg))));
}
}
//
////////////////////////////////////////////////
template <class Float>
[[nodiscard]]
inline constexpr Float Fade(const Float t) noexcept
{
return t * t * t * (t * (t * 6 - 15) + 10);
}
template <class Float>
[[nodiscard]]
inline constexpr Float Lerp(const Float a, const Float b, const Float t) noexcept
{
return (a + (b - a) * t);
}
template <class Float>
[[nodiscard]]
inline constexpr Float Grad(const std::uint8_t hash, const Float x, const Float y, const Float z) noexcept
{
const std::uint8_t h = hash & 15;
const Float u = h < 8 ? x : y;
const Float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
template <class Float>
[[nodiscard]]
inline constexpr Float Remap_01(const Float x) noexcept
{
return (x * Float(0.5) + Float(0.5));
}
template <class Float>
[[nodiscard]]
inline constexpr Float Clamp_11(const Float x) noexcept
{
return std::clamp(x, Float(-1.0), Float(1.0));
}
template <class Float>
[[nodiscard]]
inline constexpr Float RemapClamp_01(const Float x) noexcept
{
if (x <= Float(-1.0))
{
return Float(0.0);
}
else if (Float(1.0) <= x)
{
return Float(1.0);
}
return (x * Float(0.5) + Float(0.5));
}
template <class Noise, class Float>
[[nodiscard]]
inline auto Octave1D(const Noise& noise, Float x, const std::int32_t octaves, const Float persistence) noexcept
{
using value_type = Float;
value_type result = 0;
value_type amplitude = 1;
for (std::int32_t i = 0; i < octaves; ++i)
{
result += (noise.noise1D(x) * amplitude);
x *= 2;
amplitude *= persistence;
}
return result;
}
template <class Noise, class Float>
[[nodiscard]]
inline auto Octave2D(const Noise& noise, Float x, Float y, const std::int32_t octaves, const Float persistence) noexcept
{
using value_type = Float;
value_type result = 0;
value_type amplitude = 1;
for (std::int32_t i = 0; i < octaves; ++i)
{
result += (noise.noise2D(x, y) * amplitude);
x *= 2;
y *= 2;
amplitude *= persistence;
}
return result;
}
template <class Noise, class Float>
[[nodiscard]]
inline auto Octave3D(const Noise& noise, Float x, Float y, Float z, const std::int32_t octaves, const Float persistence) noexcept
{
using value_type = Float;
value_type result = 0;
value_type amplitude = 1;
for (std::int32_t i = 0; i < octaves; ++i)
{
result += (noise.noise3D(x, y, z) * amplitude);
x *= 2;
y *= 2;
z *= 2;
amplitude *= persistence;
}
return result;
}
template <class Float>
[[nodiscard]]
inline constexpr Float MaxAmplitude(const std::int32_t octaves, const Float persistence) noexcept
{
using value_type = Float;
value_type result = 0;
value_type amplitude = 1;
for (std::int32_t i = 0; i < octaves; ++i)
{
result += amplitude;
amplitude *= persistence;
}
return result;
}
}
///////////////////////////////////////
template <class Float>
inline constexpr BasicPerlinNoise<Float>::BasicPerlinNoise() noexcept
: m_permutation{ 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 } {}
template <class Float>
inline BasicPerlinNoise<Float>::BasicPerlinNoise(const seed_type seed)
{
reseed(seed);
}
template <class Float>
SIVPERLIN_CONCEPT_URBG_
inline BasicPerlinNoise<Float>::BasicPerlinNoise(URBG&& urbg)
{
reseed(std::forward<URBG>(urbg));
}
///////////////////////////////////////
template <class Float>
inline void BasicPerlinNoise<Float>::reseed(const seed_type seed)
{
reseed(default_random_engine{ seed });
}
template <class Float>
SIVPERLIN_CONCEPT_URBG_
inline void BasicPerlinNoise<Float>::reseed(URBG&& urbg)
{
std::iota(m_permutation.begin(), m_permutation.end(), uint8_t{ 0 });
perlin_detail::Shuffle(m_permutation.begin(), m_permutation.end(), std::forward<URBG>(urbg));
}
///////////////////////////////////////
template <class Float>
inline constexpr const typename BasicPerlinNoise<Float>::state_type& BasicPerlinNoise<Float>::serialize() const noexcept
{
return m_permutation;
}
template <class Float>
inline constexpr void BasicPerlinNoise<Float>::deserialize(const state_type& state) noexcept
{
m_permutation = state;
}
///////////////////////////////////////
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::noise1D(const value_type x) const noexcept
{
return noise3D(x,
static_cast<value_type>(SIVPERLIN_DEFAULT_Y),
static_cast<value_type>(SIVPERLIN_DEFAULT_Z));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::noise2D(const value_type x, const value_type y) const noexcept
{
return noise3D(x,
y,
static_cast<value_type>(SIVPERLIN_DEFAULT_Z));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::noise3D(const value_type x, const value_type y, const value_type z) const noexcept
{
const value_type _x = std::floor(x);
const value_type _y = std::floor(y);
const value_type _z = std::floor(z);
const std::int32_t ix = static_cast<std::int32_t>(_x) & 255;
const std::int32_t iy = static_cast<std::int32_t>(_y) & 255;
const std::int32_t iz = static_cast<std::int32_t>(_z) & 255;
const value_type fx = (x - _x);
const value_type fy = (y - _y);
const value_type fz = (z - _z);
const value_type u = perlin_detail::Fade(fx);
const value_type v = perlin_detail::Fade(fy);
const value_type w = perlin_detail::Fade(fz);
const std::uint8_t A = (m_permutation[ix & 255] + iy) & 255;
const std::uint8_t B = (m_permutation[(ix + 1) & 255] + iy) & 255;
const std::uint8_t AA = (m_permutation[A] + iz) & 255;
const std::uint8_t AB = (m_permutation[(A + 1) & 255] + iz) & 255;
const std::uint8_t BA = (m_permutation[B] + iz) & 255;
const std::uint8_t BB = (m_permutation[(B + 1) & 255] + iz) & 255;
const value_type p0 = perlin_detail::Grad(m_permutation[AA], fx, fy, fz);
const value_type p1 = perlin_detail::Grad(m_permutation[BA], fx - 1, fy, fz);
const value_type p2 = perlin_detail::Grad(m_permutation[AB], fx, fy - 1, fz);
const value_type p3 = perlin_detail::Grad(m_permutation[BB], fx - 1, fy - 1, fz);
const value_type p4 = perlin_detail::Grad(m_permutation[(AA + 1) & 255], fx, fy, fz - 1);
const value_type p5 = perlin_detail::Grad(m_permutation[(BA + 1) & 255], fx - 1, fy, fz - 1);
const value_type p6 = perlin_detail::Grad(m_permutation[(AB + 1) & 255], fx, fy - 1, fz - 1);
const value_type p7 = perlin_detail::Grad(m_permutation[(BB + 1) & 255], fx - 1, fy - 1, fz - 1);
const value_type q0 = perlin_detail::Lerp(p0, p1, u);
const value_type q1 = perlin_detail::Lerp(p2, p3, u);
const value_type q2 = perlin_detail::Lerp(p4, p5, u);
const value_type q3 = perlin_detail::Lerp(p6, p7, u);
const value_type r0 = perlin_detail::Lerp(q0, q1, v);
const value_type r1 = perlin_detail::Lerp(q2, q3, v);
return perlin_detail::Lerp(r0, r1, w);
}
///////////////////////////////////////
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::noise1D_01(const value_type x) const noexcept
{
return perlin_detail::Remap_01(noise1D(x));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::noise2D_01(const value_type x, const value_type y) const noexcept
{
return perlin_detail::Remap_01(noise2D(x, y));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::noise3D_01(const value_type x, const value_type y, const value_type z) const noexcept
{
return perlin_detail::Remap_01(noise3D(x, y, z));
}
///////////////////////////////////////
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave1D(const value_type x, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Octave1D(*this, x, octaves, persistence);
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave2D(const value_type x, const value_type y, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Octave2D(*this, x, y, octaves, persistence);
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave3D(const value_type x, const value_type y, const value_type z, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Octave3D(*this, x, y, z, octaves, persistence);
}
///////////////////////////////////////
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave1D_11(const value_type x, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Clamp_11(octave1D(x, octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave2D_11(const value_type x, const value_type y, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Clamp_11(octave2D(x, y, octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave3D_11(const value_type x, const value_type y, const value_type z, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Clamp_11(octave3D(x, y, z, octaves, persistence));
}
///////////////////////////////////////
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave1D_01(const value_type x, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::RemapClamp_01(octave1D(x, octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave2D_01(const value_type x, const value_type y, std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::RemapClamp_01(octave2D(x, y, octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::octave3D_01(const value_type x, const value_type y, const value_type z, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::RemapClamp_01(octave3D(x, y, z, octaves, persistence));
}
///////////////////////////////////////
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::normalizedOctave1D(const value_type x, const std::int32_t octaves, const value_type persistence) const noexcept
{
return (octave1D(x, octaves, persistence) / perlin_detail::MaxAmplitude(octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::normalizedOctave2D(const value_type x, const value_type y, const std::int32_t octaves, const value_type persistence) const noexcept
{
return (octave2D(x, y, octaves, persistence) / perlin_detail::MaxAmplitude(octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::normalizedOctave3D(const value_type x, const value_type y, const value_type z, const std::int32_t octaves, const value_type persistence) const noexcept
{
return (octave3D(x, y, z, octaves, persistence) / perlin_detail::MaxAmplitude(octaves, persistence));
}
///////////////////////////////////////
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::normalizedOctave1D_01(const value_type x, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Remap_01(normalizedOctave1D(x, octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::normalizedOctave2D_01(const value_type x, const value_type y, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Remap_01(normalizedOctave2D(x, y, octaves, persistence));
}
template <class Float>
inline typename BasicPerlinNoise<Float>::value_type BasicPerlinNoise<Float>::normalizedOctave3D_01(const value_type x, const value_type y, const value_type z, const std::int32_t octaves, const value_type persistence) const noexcept
{
return perlin_detail::Remap_01(normalizedOctave3D(x, y, z, octaves, persistence));
}
}
# undef SIVPERLIN_NODISCARD_CXX20
# undef SIVPERLIN_CONCEPT_URBG
# undef SIVPERLIN_CONCEPT_URBG_
