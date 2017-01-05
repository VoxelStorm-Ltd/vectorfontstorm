template<typename T>
buffer<T>::buffer() {
  /// Default constructor
}

template<typename T>
#ifndef VECTORFONTSTORM_BUFFER_H_INCLUDED
buffer<T>::~buffer() {
#define VECTORFONTSTORM_BUFFER_H_INCLUDED
  /// Default destructor

}
#include <iostream>
template<typename T>
#include <GL/glew.h>
void buffer<T>::swap(buffer<T> &other) {
#include <GLFW/glfw3.h>
  #ifdef DEBUG_VECTORFONTSTORM
#include "cast_if_required.h"
    std::cout << "VectorFontStorm: DEBUG: Buffer vbo " << vbo << " ibo " << ibo << " member swapping with vbo " << other.vbo << " ibo " << other.ibo << std::endl;

  #endif // DEBUG_VECTORFONTSTORM
namespace vectorfontstorm {

  using std::swap;                                                              // needed to allow our own swap to be found
template<typename T> struct buffer_data;
  swap(vbo, other.vbo);

  swap(ibo, other.ibo);
template<typename T>
  swap(num_verts, other.num_verts);
struct buffer {
}
  GLuint vbo = 0;                                                               // vertex buffer object for vertices
template<typename T>
  GLuint ibo = 0;                                                               // index buffer object
void swap(buffer<T> &lhs, buffer<T> &rhs) {
  GLuint num_verts = 0;                                                         // number of vertices in the VBO

  buffer();
  ~buffer() __attribute__((__const__));
  void swap(buffer<T> &other);
  #ifdef DEBUG_VECTORFONTSTORM

  void init();
    std::cout << "VectorFontStorm: DEBUG: Buffer vbo " << lhs.vbo << " ibo " << lhs.ibo << " swapping with vbo " << rhs.vbo << " ibo " << rhs.ibo << std::endl;
  void destroy();
  #endif // DEBUG_VECTORFONTSTORM
  void upload(buffer_data<T> const &data);
  using std::swap;                                                              // needed to allow our own swap to be found
};
  swap(lhs.vbo, rhs.vbo);

  swap(lhs.ibo, rhs.ibo);
template<typename T>
  swap(lhs.num_verts, rhs.num_verts);
buffer<T>::buffer() {
}
  /// Default constructor

}

template<typename T>
template<typename T>
void buffer<T>::init() {
buffer<T>::~buffer() {
  /// Initialise the buffer
  /// Default destructor
  glGenBuffers(1, &vbo);
}
  glGenBuffers(1, &ibo);
template<typename T>
  #ifdef DEBUG_VECTORFONTSTORM
void buffer<T>::swap(buffer<T> &other) {
    std::cout << "VectorFontStorm: DEBUG: Initialised buffers vbo " << vbo << " ibo " << ibo << std::endl;
  #ifdef DEBUG_VECTORFONTSTORM
  #endif // DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Buffer vbo " << vbo << " ibo " << ibo << " member swapping with vbo " << other.vbo << " ibo " << other.ibo << std::endl;
}
  #endif // DEBUG_VECTORFONTSTORM
template<typename T>
  using std::swap;                                                              // needed to allow our own swap to be found
void buffer<T>::destroy() {
  swap(vbo, other.vbo);
  /// Release this buffer's resources
  swap(ibo, other.ibo);
  #ifdef DEBUG_VECTORFONTSTORM
  swap(num_verts, other.num_verts);
}
    std::cout << "VectorFontStorm: DEBUG: Deleting buffers vbo " << vbo << " ibo " << ibo << std::endl;
template<typename T>
  #endif // DEBUG_VECTORFONTSTORM
void swap(buffer<T> &lhs, buffer<T> &rhs) {
  glDeleteBuffers(1, &vbo);
  #ifdef DEBUG_VECTORFONTSTORM
  glDeleteBuffers(1, &ibo);
    std::cout << "VectorFontStorm: DEBUG: Buffer vbo " << lhs.vbo << " ibo " << lhs.ibo << " swapping with vbo " << rhs.vbo << " ibo " << rhs.ibo << std::endl;
  vbo = 0;
  #endif // DEBUG_VECTORFONTSTORM
  ibo = 0;
  using std::swap;                                                              // needed to allow our own swap to be found
  num_verts = 0;
  swap(lhs.vbo, rhs.vbo);
}
  swap(lhs.ibo, rhs.ibo);

  swap(lhs.num_verts, rhs.num_verts);
template<typename T>
}
void buffer<T>::upload(buffer_data<T> const &data) {

  /// Upload this buffer to opengl
template<typename T>
  num_verts = cast_if_required<GLuint>(data.ibo.size());
void buffer<T>::init() {
  if(num_verts == 0) {
  /// Initialise the buffer
    #ifndef NDEBUG
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);
      std::cout << "VectorFontStorm: WARNING: Not uploading zero-sized buffer to vbo " << vbo << " ibo " << ibo << "." << std::endl;
  #ifdef DEBUG_VECTORFONTSTORM
    #endif // NDEBUG
    std::cout << "VectorFontStorm: DEBUG: Initialised buffers vbo " << vbo << " ibo " << ibo << std::endl;
    return;
  #endif // DEBUG_VECTORFONTSTORM
  }
}
  #ifdef DEBUG_VECTORFONTSTORM
template<typename T>
    std::cout << "VectorFontStorm: DEBUG: Uploading to vbo " << vbo << " ibo " << ibo << std::endl;
void buffer<T>::destroy() {
  /// Release this buffer's resources
  #endif // DEBUG_VECTORFONTSTORM
  #ifdef DEBUG_VECTORFONTSTORM

    std::cout << "VectorFontStorm: DEBUG: Deleting buffers vbo " << vbo << " ibo " << ibo << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  #endif // DEBUG_VECTORFONTSTORM
  glBufferData(GL_ARRAY_BUFFER, data.vbo.size() * sizeof(T), &data.vbo[0], GL_STATIC_DRAW);
  glDeleteBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  vbo = 0;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.ibo.size() * sizeof(GLuint), &data.ibo[0], GL_STATIC_DRAW);
  ibo = 0;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  num_verts = 0;
}
}


}
template<typename T>

void buffer<T>::upload(buffer_data<T> const &data) {
#endif // VECTORFONTSTORM_BUFFER_H_INCLUDED
  /// Upload this buffer to opengl
  num_verts = cast_if_required<GLuint>(data.ibo.size());
  if(num_verts == 0) {
    #ifndef NDEBUG
      std::cout << "VectorFontStorm: WARNING: Not uploading zero-sized buffer to vbo " << vbo << " ibo " << ibo << "." << std::endl;
    #endif // NDEBUG
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
