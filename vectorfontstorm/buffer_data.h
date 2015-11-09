#ifndef VECTORFONTSTORM_BUFFER_DATA_H_INCLUDED
#define VECTORFONTSTORM_BUFFER_DATA_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath_forward_dec.h"

namespace vectorfontstorm {

struct buffer_data {
  std::vector<Vector3<GLfloat>> vbo;                                            // point vbo data values
  std::vector<GLuint>           ibo;                                            // indices

  buffer_data();
  ~buffer_data();
};

}

#endif // VECTORFONTSTORM_BUFFER_DATA_H_INCLUDED
