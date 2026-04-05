#pragma once

/// Safe numeric cast that suppresses narrowing-conversion warnings when the
/// conversion is intentional.  Equivalent to static_cast<To>(value) but
/// self-documenting at the call site.
template<typename To, typename From>
inline To cast_if_required(From const &value) {
  return static_cast<To>(value);
}
