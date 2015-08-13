#ifndef VECTORFONTSTORM_STRING_H_INCLUDED
#define VECTORFONTSTORM_STRING_H_INCLUDED

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

namespace vectorfontstorm {

class font;

class string {
  enum class aligntype {
    TOPLEFT,
    TOP,
    TOPRIGHT,
    LEFT,
    RIGHT,
    CENTRE,
    BOTTOMLEFT,
    BOTTOM,
    BOTTOMRIGHT
  };
  aligntype align = aligntype::CENTRE;

  std::string contents;
  vectorfontstorm::font &thisfont;

  GLuint vbo = 0;                                                               // vertex buffer object for vertices
  GLuint ibo = 0;                                                               // index buffer object
  GLuint num_verts = 0;                                                         // number of vertices in the VBO

public:
  string(std::string const &newcontents,
         vectorfontstorm::font &newfont,
         Vector3f const &position = {0.0f, 0.0f, 0.0f},
         Quatf const &orientation = {1.0f, 0.0f, 0.0f, 0.0f},
         double scale = 1.0,
         aligntype alignment = aligntype::CENTRE);
  ~string();

  void render() const;
};

}

#endif // VECTORFONTSTORM_STRING_H_INCLUDED
