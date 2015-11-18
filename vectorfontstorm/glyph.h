#ifndef VECTORFONTSTORM_GLYPH_H_INCLUDED
#define VECTORFONTSTORM_GLYPH_H_INCLUDED

#include "buffer_data.h"

namespace vectorfontstorm {

class contour;

class glyph {
  friend class font;

  std::vector<contour> contours;

  buffer_data outline;                                                          // cached point vbo data values, indices need to be offset when outputting
  buffer_data fill;                                                             // as above but for filled rendering rather than outlines
  buffer_data back;                                                             // mirrored version of normal fill, offset to the back of the 3D glyph
  buffer_data edge;                                                             // the polygon ribbon forming the extruded edge of the 3D glyph

  float advance = 0.0f;                                                         // how far the next glyph needs to advance

  bool whitespace = false;                                                      // whether this glyph is pure whitespace, i.e. nothing to draw
  char character = '\0';                                                        // what ascii character this glyph represents

public:
  glyph(char newchar, float newadvance);
  ~glyph();

  void correct_winding();

  void cache_buffer();

  float get_advance() const __attribute__((__pure__));

  void get_buffer(buffer_data const &data_in, buffer_data &data_out);
  void get_outline(                           buffer_data &data_out);
  void get_fill(                              buffer_data &data_out);
  void get_back(                              buffer_data &data_out);
  void get_edge(                              buffer_data &data_out);
};

}

#endif // VECTORFONTSTORM_GLYPH_H_INCLUDED
