#ifndef VECTORFONTSTORM_MANAGER_H_INCLUDED
#define VECTORFONTSTORM_MANAGER_H_INCLUDED

#include "font.h"

namespace vectorfontstorm {

class manager {
  std::vector<font> fonts;

public:
  manager();
  ~manager();
};

}

#endif // VECTORFONTSTORM_MANAGER_H_INCLUDED
