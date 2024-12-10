#pragma once

#include "font.h"

namespace vectorfontstorm {

template<typename T>
class manager {
  std::vector<font<T>> fonts;

public:
  manager();
  ~manager();
};

}
