#ifndef VECTORFONTSTORM_SEGMENT_H_INCLUDED
#define VECTORFONTSTORM_SEGMENT_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath_forward_dec.h"

namespace vectorfontstorm {

class point;

class segment {
  friend class font;

  enum class types {
    UNKNOWN,                                                                    // not yet defined
    LINE,                                                                       // two successive ON points form a line segment
    CONIC,                                                                      // a conic OFF point between two successive ON points forms a conic bezier arc
    CUBIC                                                                       // two cubic OFF poitns between two ON points forms a cubic bezier arc
  };
  types type = types::UNKNOWN;

  std::vector<point> points;

public:
  segment();
  ~segment();

  point &get_second_to_last_point();

  void get_outline(std::vector<Vector3<GLfloat>> &vbo_data, std::vector<GLuint> &ibo_data) const;

  void subdivide_conic(std::vector<segment> &new_segments) const;
  void subdivide_cubic(std::vector<segment> &new_segments) const;
};

}

#endif // VECTORFONTSTORM_SEGMENT_H_INCLUDED
