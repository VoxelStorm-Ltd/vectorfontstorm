#include "glyph.h"
#include "vmath.h"
#include "cast_if_required.h"
#include "contour.h"

namespace vectorfontstorm {

glyph::glyph(char newchar, float newadvance)
  : advance(newadvance),
    character(newchar) {
  /// Default constructor
}

glyph::~glyph() {
  /// Default destructor
}

void glyph::cache_buffer() {
  /// Walk through the glyph's components and cache the outline it makes
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Glyph \"" << character << "\" with " << contours.size() << " contours" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  for(contour const &c : contours) {
    c.get_outline(outline);
    // generate fill from outline
    // TODO
  }
}

float glyph::get_advance() const {
  return advance;
}

void glyph::get_buffer(buffer_data const &data_in, buffer_data &data_out) {
  /// Output the vertices from the specified buffer to an indexed buffer, if non-empty
  if(whitespace) {
    return;
  } else {
    if(__builtin_expect(data_in.vbo.empty(), false)) {
      cache_buffer();
      if(__builtin_expect(data_in.vbo.empty(), false)) {
        // std::cout << "VectorFontStorm: WARNING: Requested buffer from glyph with empty data_in.vbo!" << std::endl;
        whitespace = true;
        return;
      }
      // whitespace glyphs have no outlines so this is normal
    }
  }
  GLuint const vertex_offset = cast_if_required<GLuint>(data_out.vbo.size());
  data_out.vbo.reserve(data_out.vbo.size() + data_in.vbo.size());
  data_out.ibo.reserve(data_out.ibo.size() + data_in.ibo.size());
  auto const newrange_start  = data_out.ibo.end();                              // this NEEDS the reserve first, or the iterator will be invalidated
  // append the vectors to our target
  data_out.vbo.insert(data_out.vbo.end(), data_in.vbo.begin(), data_in.vbo.end());
  data_out.ibo.insert(data_out.ibo.end(), data_in.ibo.begin(), data_in.ibo.end());
  // transform the indices to the correct offset
  for(auto &it : boost::make_iterator_range(newrange_start, data_out.ibo.end())) {
    it += vertex_offset;
  }
}

void glyph::get_outline(buffer_data &data_out) {
  /// Output the vertices for this character to an indexed buffer
  get_buffer(outline, data_out);
}

void glyph::get_fill(buffer_data &data_out) {
  /// Output the vertices for this character to an indexed buffer
  get_buffer(fill, data_out);
}

}
