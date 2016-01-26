#ifndef VECTORFONTSTORM_BUFFER_H_INCLUDED
#define VECTORFONTSTORM_BUFFER_H_INCLUDED

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "cast_if_required.h"

namespace vectorfontstorm {

template<typename T> struct buffer_data;

template<typename T>
struct buffer {
  GLuint vbo = 0;                                                               // vertex buffer object for vertices
  GLuint ibo = 0;                                                               // index buffer object
  GLuint num_verts = 0;                                                         // number of vertices in the VBO

  buffer();
  ~buffer() __attribute__((__const__));

  void init();
  void destroy();
  void upload(buffer_data<T> const &data);
};

template<typename T>
buffer<T>::buffer() {
  /// Default constructor
}

template<typename T>
buffer<T>::~buffer() {
  /// Default destructor
}

template<typename T>
void buffer<T>::init() {
  /// Initialise the buffer
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);
}
template<typename T>
void buffer<T>::destroy() {
  /// Release this buffer's resources
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  vbo = 0;
  ibo = 0;
  num_verts = 0;
}

template<typename T>
void buffer<T>::upload(buffer_data<T> const &data) {
  /// Upload this buffer to opengl
  num_verts = cast_if_required<GLuint>(data.ibo.size());
  if(num_verts == 0) {
    std::cout << "VectorFontStorm: WARNING: not uploading zero-sized buffer." << std::endl;
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.vbo.size() * sizeof(T), &data.vbo[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.ibo.size() * sizeof(GLuint), &data.ibo[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}

#endif // VECTORFONTSTORM_BUFFER_H_INCLUDED
