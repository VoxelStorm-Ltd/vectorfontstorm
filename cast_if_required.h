#pragma once
template<typename To, typename From>
inline To cast_if_required(From const value) {
  return static_cast<To>(value);
}
