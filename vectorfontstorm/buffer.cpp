#include "buffer.h"
#include <iostream>
#include "vmath.h"
#include "cast_if_required.h"
#include "buffer_data.h"

namespace vectorfontstorm {

buffer::buffer() {
  /// Default constructor
}

buffer::~buffer() {
  /// Default destructor
}

void buffer::init() {
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);
}
void buffer::destroy() {
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  vbo = 0;
  ibo = 0;
  num_verts = 0;
}

void buffer::upload(buffer_data const &data) {
  num_verts = cast_if_required<GLuint>(data.ibo.size());
  if(num_verts == 0) {
    std::cout << "VectorFontStorm: WARNING: not uploading zero-sized buffer." << std::endl;
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.vbo.size() * sizeof(Vector3<GLfloat>), &data.vbo[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.ibo.size() * sizeof(GLuint), &data.ibo[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void buffer::render(GLenum mode) const {
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(mode, num_verts, GL_UNSIGNED_INT, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
