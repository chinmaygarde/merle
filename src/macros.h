#pragma once

#include <iostream>

#define MERLE_ASSERT(x)                                         \
  {                                                             \
    if (!(x)) {                                                 \
      std::cout << "NS: Assertion " #x " failed." << std::endl; \
      std::abort();                                             \
    }                                                           \
  }

#define MERLE_DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete

#define MERLE_DISALLOW_ASSIGN(TypeName) \
  TypeName& operator=(const TypeName&) = delete

#define MERLE_DISALLOW_MOVE(TypeName) \
  TypeName(TypeName&&) = delete;      \
  TypeName& operator=(TypeName&&) = delete

#define MERLE_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;            \
  TypeName& operator=(const TypeName&) = delete

#define MERLE_DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName) \
  TypeName(const TypeName&) = delete;                 \
  TypeName(TypeName&&) = delete;                      \
  TypeName& operator=(const TypeName&) = delete;      \
  TypeName& operator=(TypeName&&) = delete

#define MERLE_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                                 \
  MERLE_DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName)

#define NS_ALWAYS_INLINE inline __attribute__((always_inline))
