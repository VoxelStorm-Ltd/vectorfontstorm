#ifndef VECTORFONTSTORM_BUFFER_DATA_H_INCLUDED
#define VECTORFONTSTORM_BUFFER_DATA_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath_forward_dec.h"

namespace vectorfontstorm {

template<typename T>
struct buffer_data {
  std::vector<T>      vbo;                                                      // point vbo data values
  std::vector<GLuint> ibo;                                                      // indices

  buffer_data();
  ~buffer_data();
};

template<typename T>
buffer_data<T>::buffer_data() {
  /// Default constructor
}

template<typename T>
buffer_data<T>::~buffer_data() {
  /// Default destructor
}

}

#endif // VECTORFONTSTORM_BUFFER_DATA_H_INCLUDED
