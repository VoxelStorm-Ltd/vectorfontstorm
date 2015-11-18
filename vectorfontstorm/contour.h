#ifndef VECTORFONTSTORM_CONTOUR_H_INCLUDED
#define VECTORFONTSTORM_CONTOUR_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath_forward_dec.h"
#include "poly2tri/poly2tri_forward.h"

namespace vectorfontstorm {

struct buffer_data;
class segment;

class contour {
  friend class font;
public:
  enum class windingtype : char {                                               // polygon winding types
    UNTESTED,                                                                   // the winding has not yet been checked
    INDETERMINATE,                                                              // winding has been checked but cannot be determined (too few points?)
    CLOCKWISE,                                                                  // the polygon is wound clockwise
    COUNTERCLOCKWISE,                                                           // the polygon is wound counterclockwise
    EXTERNAL = CLOCKWISE,                                                       // which winding is used for external polygons (outlines)
    INTERNAL = COUNTERCLOCKWISE                                                 // which winding is used for internal polygons (holes)
  };

  windingtype winding = windingtype::UNTESTED;                                  // what sort of winding this polygon has - cached

private:
  std::vector<segment> segments;

public:
  contour();
  ~contour();

  segment &get_second_to_last_segment() __attribute__((__pure__));

  void get_outline(buffer_data &data_out) const;
  void get_outline(std::vector<p2t::Point*> &polyline_out) const;
  void get_extrude(buffer_data &data_out) const;

  windingtype get_winding();
  void set_winding_clockwise();
  void set_winding_counterclockwise();

  void reverse();
};

}

#endif // VECTORFONTSTORM_CONTOUR_H_INCLUDED
