#pragma once

#include <arm_neon.h>
#include <cstddef>

namespace ns {

template <class ScalarType, size_t Width>
struct IntrinsicT {
  using Type = std::nullptr_t;
};

template <class ScalarType, size_t Width>
struct RegisterT {
  using Intrinsic = typename IntrinsicT<ScalarType, Width>::Type;

  Intrinsic v;

  explicit RegisterT(ScalarType initial_value);

  void Store(ScalarType* ptr) const;

  void Load(const ScalarType* ptr);
};

//------------------------------------------------------------------------------
/// uint32x4_t
//------------------------------------------------------------------------------

template <>
struct IntrinsicT<uint32_t, 4> {
  using Type = uint32x4_t;
};

template <>
inline RegisterT<uint32_t, 4>::RegisterT(uint32_t initial_value)
    : v(vdupq_n_u32(initial_value)) {}

template <>
inline void RegisterT<uint32_t, 4>::Store(uint32_t* ptr) const {
  vst1q_u32(ptr, v);
}

template <>
inline void RegisterT<uint32_t, 4>::Load(const uint32_t* ptr) {
  v = vld1q_u32(ptr);
}

using RegisterUInt32x4 = RegisterT<uint32_t, 4>;

}  // namespace ns
