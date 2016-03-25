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
  void swap(buffer<T> &other);

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
void buffer<T>::swap(buffer<T> &other) {
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Buffer vbo " << vbo << " ibo " << ibo << " member swapping with vbo " << other.vbo << " ibo " << other.ibo << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  using std::swap;                                                              // needed to allow our own swap to be found
  swap(vbo, other.vbo);
  swap(ibo, other.ibo);
  swap(num_verts, other.num_verts);
}
template<typename T>
void swap(buffer<T> &lhs, buffer<T> &rhs) {
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Buffer vbo " << lhs.vbo << " ibo " << lhs.ibo << " swapping with vbo " << rhs.vbo << " ibo " << rhs.ibo << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  using std::swap;                                                              // needed to allow our own swap to be found
  swap(lhs.vbo, rhs.vbo);
  swap(lhs.ibo, rhs.ibo);
  swap(lhs.num_verts, rhs.num_verts);
}

template<typename T>
void buffer<T>::init() {
  /// Initialise the buffer
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Initialised buffers vbo " << vbo << " ibo " << ibo << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
}
template<typename T>
void buffer<T>::destroy() {
  /// Release this buffer's resources
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Deleting buffers vbo " << vbo << " ibo " << ibo << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
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
    std::cout << "VectorFontStorm: WARNING: Not uploading zero-sized buffer to vbo " << vbo << " ibo " << ibo << "." << std::endl;
    return;
  }
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Uploading to vbo " << vbo << " ibo " << ibo << std::endl;
  #endif // DEBUG_VECTORFONTSTORM

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.vbo.size() * sizeof(T), &data.vbo[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.ibo.size() * sizeof(GLuint), &data.ibo[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}

#endif // VECTORFONTSTORM_BUFFER_H_INCLUDED
