#ifndef VECTORFONTSTORM_BUFFER_H_INCLUDED
#define VECTORFONTSTORM_BUFFER_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace vectorfontstorm {

struct buffer_data;

struct buffer {
  GLuint vbo = 0;                                                               // vertex buffer object for vertices
  GLuint ibo = 0;                                                               // index buffer object
  GLuint num_verts = 0;                                                         // number of vertices in the VBO

  buffer();
  ~buffer() __attribute__((__const__));

  void init();
  void destroy();
  void upload(buffer_data const &data);
  void render(GLenum mode) const;
};

}

#endif // VECTORFONTSTORM_BUFFER_H_INCLUDED
