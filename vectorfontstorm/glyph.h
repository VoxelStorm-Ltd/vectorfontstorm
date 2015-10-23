#ifndef VECTORFONTSTORM_GLYPH_H_INCLUDED
#define VECTORFONTSTORM_GLYPH_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath_forward_dec.h"

namespace vectorfontstorm {

class contour;

class glyph {
  friend class font;

  std::vector<contour> contours;

  std::vector<Vector3<GLfloat>> vbo_data;                                       // cached point vbo data values for rapid output
  std::vector<GLuint>           ibo_data;                                       // cached indices, need to be offset when outputting

  float advance = 0.0f;                                                         // how far the next glyph needs to advance

  bool whitespace = false;                                                      // whether this glyph is pure whitespace, i.e. nothing to draw
  char character = '\0';                                                        // what ascii character this glyph represents

public:
  glyph(char newchar, float newadvance);
  ~glyph();

  void cache_outline();

  float get_advance() const __attribute__((__pure__));
  void get_outline(std::vector<Vector3<GLfloat>> &vbo_data_out, std::vector<GLuint> &ibo_data_out);
};

}

#endif // VECTORFONTSTORM_GLYPH_H_INCLUDED
