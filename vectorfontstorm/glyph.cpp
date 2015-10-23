#include "glyph.h"
#include "vmath.h"
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

void glyph::cache_outline() {
  /// Walk through the glyph's components and cache the outline it makes
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Glyph \"" << character << "\" with " << contours.size() << " contours" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  for(contour const &c : contours) {
    c.get_outline(vbo_data, ibo_data);
  }
}

float glyph::get_advance() const {
  return advance;
}

void glyph::get_outline(std::vector<Vector3<GLfloat>> &vbo_data_out,
                        std::vector<GLuint>           &ibo_data_out) {
  /// Output the vertices for this character to an indexed buffer
  if(whitespace) {
    return;
  } else {
    if(__builtin_expect(vbo_data.empty(), false)) {
      cache_outline();
      if(__builtin_expect(vbo_data.empty(), false)) {
        // std::cout << "VectorFontStorm: WARNING: Requested outline from glyph with empty vbo_data!" << std::endl;
        whitespace = true;
        return;
      }
      // whitespace glyphs have no outlines so this is normal
    }
  }
  GLuint const vertex_offset = static_cast<GLuint>(vbo_data_out.size());
  vbo_data_out.reserve(vbo_data_out.size() + vbo_data.size());
  ibo_data_out.reserve(ibo_data_out.size() + ibo_data.size());
  auto const newrange_start  = ibo_data_out.end();                              // this NEEDS the reserve first, or the iterator will be invalidated
  // append the vectors to our target
  vbo_data_out.insert(vbo_data_out.end(), vbo_data.begin(), vbo_data.end());
  ibo_data_out.insert(ibo_data_out.end(), ibo_data.begin(), ibo_data.end());
  // transform the indices to the correct offset
  for(auto &it : boost::make_iterator_range(newrange_start, ibo_data_out.end())) {
    it += vertex_offset;
  }
}

}
