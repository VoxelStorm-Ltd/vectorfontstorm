#ifndef VECTORFONTSTORM_CONTOUR_H_INCLUDED
#define VECTORFONTSTORM_CONTOUR_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath_forward_dec.h"

namespace vectorfontstorm {

struct buffer_data;
class segment;

class contour {
  friend class font;

  std::vector<segment> segments;

public:
  contour();
  ~contour();

  segment &get_second_to_last_segment() __attribute__((__pure__));

  void get_outline(buffer_data &data_out) const;
};

}

#endif // VECTORFONTSTORM_CONTOUR_H_INCLUDED
