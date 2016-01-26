#ifndef VECTORFONTSTORM_MANAGER_H_INCLUDED
#define VECTORFONTSTORM_MANAGER_H_INCLUDED

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

#endif // VECTORFONTSTORM_MANAGER_H_INCLUDED
