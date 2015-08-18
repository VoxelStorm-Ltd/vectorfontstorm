#ifndef VECTORFONTSTORM_CONTOUR_H_INCLUDED
#define VECTORFONTSTORM_CONTOUR_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

namespace vectorfontstorm {

class segment;

class contour {
  friend class font;

  std::vector<segment> segments;

public:
  contour();
  ~contour();

  segment &get_second_to_last_segment();

  void get_outline(std::vector<Vector3<GLfloat>> &vbo_data, std::vector<GLuint> &ibo_data) const;
};

}

#endif // VECTORFONTSTORM_CONTOUR_H_INCLUDED
