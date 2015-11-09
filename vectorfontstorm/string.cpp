#include "string.h"
#include <vector>
#include "vmath.h"
#include "cast_if_required.h"
#include "font.h"
#include "contour.h"
#include "segment.h"
#include "point.h"

namespace vectorfontstorm {

string::string(std::string const &newstring,
               vectorfontstorm::font &newfont,
               Vector3f const &position,
               Quatf const &orientation,
               double scale,
               aligntype alignment)
  : contents(newstring),
    thisfont(newfont),
    align(alignment) {
  /// Default constructor
  init(position, orientation, scale);
}
string::string(string &&other) noexcept
  : contents(std::move(other.contents)),
    thisfont(other.thisfont),
    align(other.align) {
  /// Move constructor
  #ifndef NDEBUG
    std::cout << "VectorFontStorm: WARNING: moving string \"" << contents << "\" - this is expensive." << std::endl;
  #endif // NDEBUG
  align = other.align;
  std::swap(contents, other.contents);
  thisfont = other.thisfont;
  std::swap(outline, other.outline);
  std::swap(fill,    other.fill);
  std::swap(bounds_left, other.bounds_left);
  std::swap(bounds_bottom, other.bounds_bottom);
  std::swap(bounds_right, other.bounds_right);
  std::swap(bounds_top, other.bounds_top);
}
string &string::operator=(string &&other) noexcept {
  /// Move assignment constructor
  #ifndef NDEBUG
    std::cout << "VectorFontStorm: WARNING: moving string \"" << contents << "\" in assignment - this is expensive." << std::endl;
  #endif // NDEBUG
  align = other.align;
  std::swap(contents, other.contents);
  thisfont = other.thisfont;
  std::swap(outline, other.outline);
  std::swap(fill,    other.fill);
  std::swap(bounds_left, other.bounds_left);
  std::swap(bounds_bottom, other.bounds_bottom);
  std::swap(bounds_right, other.bounds_right);
  std::swap(bounds_top, other.bounds_top);
  return *this;
}

string::~string() {
  /// Default destructor
  outline.destroy();
  fill.destroy();
}

void string::init(Vector3f const &position,
                  Quatf const &orientation,
                  double scale) {
  /// Initialise this string
  outline.init();
  fill.init();
  buffer_data data_outline;
  buffer_data data_fill;

  Vector2f advance;
  Vector2f advance_max;
  float const line_height = static_cast<float>(thisfont.get_height());
  struct line {
    float width = 0.0f;
    GLuint index_from = 0;
    GLuint index_to   = 0;
  };
  std::vector<line> lines(1);
  for(auto const &thischar : contents) {
    if(thischar == '\n') {                                                      // handle newlines
      lines.back().width = advance.x;
      lines.back().index_to = cast_if_required<GLuint>(data_outline.vbo.size());
      advance.y += line_height;
      advance.x = 0.0f;
      lines.emplace_back();
      lines.back().index_from = cast_if_required<GLuint>(data_outline.vbo.size());
    } else {
      GLuint vbo_start = cast_if_required<GLuint>(data_outline.vbo.size());
      float const new_advance = thisfont.get_outline(thischar, data_outline);
      GLuint vbo_end = cast_if_required<GLuint>(data_outline.vbo.size());
      for(GLuint p = vbo_start; p != vbo_end; ++p) {                            // apply the previous advance to every point in this character
        data_outline.vbo[p].x += advance.x;
        data_outline.vbo[p].y -= advance.y;
      }
      advance.x += new_advance;
    }
    advance_max = std::max(advance_max, advance);                               // track the widest line for alignment
  }
  lines.back().width = advance.x;
  lines.back().index_to = cast_if_required<GLuint>(data_outline.vbo.size());
  float align_offset_width, align_offset_height;
  switch(align) {                                                               // horizontal alignment
  // left alignment
  case aligntype::TOPLEFT:
  case aligntype::LEFT:
  case aligntype::BOTTOMLEFT:
  default:
    align_offset_width = 0.0f;
    break;
  // horizontal centre alignment
  case aligntype::TOP:
  case aligntype::CENTRE:
  case aligntype::BOTTOM:
    #ifdef DEBUG_VECTORFONTSTORM
      std::cout << "DEBUG: centred string start" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM
    for(line const &this_line : lines) {
      float const offset = (advance_max.x - this_line.width) / 2.0f;            // offset for centre align
      #ifdef DEBUG_VECTORFONTSTORM
        std::cout << "DEBUG: line advance " << advance_max.x << " width " << this_line.width << " offset " << offset << std::endl;
        std::cout << "DEBUG: this_line.index_from " << this_line.index_from << " this_line.index_to " << this_line.index_to << " data_outline.vbo.size() " << data_outline.vbo.size() << std::endl;
      #endif // DEBUG_VECTORFONTSTORM
      for(GLuint p = this_line.index_from; p != this_line.index_to; ++p) {      // slide every point in this line by the offset
        data_outline.vbo[p].x += offset;
      }
    }
    align_offset_width = advance_max.x * -0.5f;
    break;
  // right alignment
  case aligntype::TOPRIGHT:
  case aligntype::RIGHT:
  case aligntype::BOTTOMRIGHT:
    #ifdef DEBUG_VECTORFONTSTORM
      std::cout << "DEBUG: right-aligned string start" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM
    for(line const &this_line : lines) {
      float const offset = advance_max.x - this_line.width;                     // offset for right align
      #ifdef DEBUG_VECTORFONTSTORM
        std::cout << "DEBUG: line advance " << advance_max.x << " width " << this_line.width << " offset " << offset << std::endl;
        //std::cout << "DEBUG: this_line.index_from " << this_line.index_from << " this_line.index_to " << this_line.index_to << " data_outline.vbo.size() " << data_outline.vbo.size() << std::endl;
      #endif // DEBUG_VECTORFONTSTORM
      for(GLuint p = this_line.index_from; p != this_line.index_to; ++p) {      // slide every point in this line by the offset
        data_outline.vbo[p].x += offset;
      }
    }
    align_offset_width = -advance_max.x;
    break;
  }
  switch(align) {                                                               // vertical alignment
  // top alignment
  case aligntype::TOPLEFT:
  case aligntype::TOP:
  case aligntype::TOPRIGHT:
    align_offset_height = -line_height;
    break;
  // vertical centre alignment
  case aligntype::LEFT:
  case aligntype::RIGHT:
  case aligntype::CENTRE:
    align_offset_height = ((line_height + advance_max.y) * 0.5f) - line_height;
    #ifdef DEBUG_VECTORFONTSTORM
      //std::cout << "DEBUG: string " << contents << std::endl;
      std::cout << "DEBUG: string vertical advance " << advance_max.y << " align_offset_height " << align_offset_height << std::endl;
    #endif // DEBUG_VECTORFONTSTORM
    break;
  // bottom alignment
  case aligntype::BOTTOMLEFT:
  case aligntype::BOTTOM:
  case aligntype::BOTTOMRIGHT:
  default:
    align_offset_height = -advance_max.y;
    break;
  }

  for(auto &p : data_outline.vbo) {
    p.x += align_offset_width;
    p.y += align_offset_height;
    p *= static_cast<float>(scale);                                             // scale the model
    p *= orientation;                                                           // rotate the model
    p += position;                                                              // position the model in space
    bounds_left   = std::min(bounds_left,   p.x);                               // update the outer bounds
    bounds_right  = std::max(bounds_right,  p.x);
    bounds_top    = std::min(bounds_top,    p.y);
    bounds_bottom = std::max(bounds_bottom, p.y);
  }
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: String \"" << contents << "\" bounds " << get_bounds_2d() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: vbo size " << data_outline.vbo.size() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: ibo size " << data_outline.ibo.size() << std::endl;
  #endif // DEBUG_VECTORFONTSTORM

  outline.upload(data_outline);
  fill.upload(data_fill);
}

std::string const &string::get_contents() const {
  return contents;
}

float string::get_bounds_left() const {
  return bounds_left;
}
float string::get_bounds_right() const {
  return bounds_right;
}
float string::get_bounds_top() const {
  return bounds_top;
}
float string::get_bounds_bottom() const {
  return bounds_bottom;
}
Vector2f string::string::get_bounds_bottomleft_2d() const {
  return Vector2f(bounds_left, bounds_bottom);
}
Vector2f string::get_bounds_topright_2d() const {
  return Vector2f(bounds_right, bounds_top);
}
Vector3f string::get_bounds_bottomleft_3d() const {
  return Vector3f(bounds_left, bounds_bottom, 0.0f);
}
Vector3f string::get_bounds_topright_3d() const {
  return Vector3f(bounds_right, bounds_top, 0.0f);
}
Aabb2f string::get_bounds_2d() const {
  return Aabb2f(bounds_left, bounds_bottom, bounds_right, bounds_top);
}
Aabb3f string::get_bounds_3d() const {
  return Aabb3f(bounds_left, bounds_bottom, 0.0f, bounds_right, bounds_top, 0.0f);
}

void string::render_outline() const {
  /// Render this string in outline mode
  outline.render(GL_LINES);
}
void string::render_fill() const {
  /// Render this string in fill mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  // wireframe
  //glDisable(GL_FOG);
  fill.render(GL_TRIANGLES);
}

}
