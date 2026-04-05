/// Minimal stub for vectorstorm/vector/vector2.h
/// Provides the vector2f type needed by vectorfontstorm without pulling in
/// the full vectorstorm library (which has GCC 13 / C++17 incompatibilities
/// in headers that are not required for the geometry pipeline under test).
#pragma once

#include <ostream>

template<typename T>
struct vector2 {
  T x = T(0);
  T y = T(0);

  vector2() = default;
  vector2(T x_, T y_) : x(x_), y(y_) {}

  /// Linear interpolation: returns a point between *this and other
  vector2 lerp(T t, vector2 const &other) const {
    return {x + t * (other.x - x), y + t * (other.y - y)};
  }
};

template<typename T>
inline std::ostream &operator<<(std::ostream &os, vector2<T> const &v) {
  return os << v.x << ' ' << v.y;
}

using vector2f = vector2<float>;
using vector2i = vector2<int>;
using vector2d = vector2<double>;
