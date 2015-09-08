#ifndef VECTORFONTSTORM_FONT_H_INCLUDED
#define VECTORFONTSTORM_FONT_H_INCLUDED

#include <cstddef>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "vmath_forward_dec.h"
#include "glyph.h"

namespace vectorfontstorm {

class font {
  FT_Library library;                                                           // the freetype library
  FT_Face face;                                                                 // the freetype font

  double height = 1.0;                                                          // desired base height for this font
  double scale = 1.0;                                                           // internal scale factor to normalise height to 1.0

  std::unordered_map<char, glyph> glyphs;

public:
  font(unsigned char const *const memory_offset, size_t const memory_size, double desired_height = 1.0);
  ~font();

  double get_height() const;

  float get_outline(char const thischar, std::vector<Vector3<GLfloat>> &vbo_data, std::vector<GLuint> &ibo_data);

  glyph &load_glyph_from_freetype(char const thischar, std::unordered_map<char, glyph>::iterator &it);
};

}

#endif // VECTORFONTSTORM_FONT_H_INCLUDED
