#include "string.h"
#include <vector>
#include <boost/range/join.hpp>
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
               float scale,
               float depth,
               aligntype alignment)
  : contents(newstring),
    thisfont(newfont),
    align(alignment) {
  /// Default constructor
  init(position, orientation, scale, depth);
}
string::string(string &&other) noexcept
  : contents(std::move(other.contents)),
    thisfont(other.thisfont),
    align(other.align) {
  /// Move constructor
  #ifndef NDEBUG
    //std::cout << "VectorFontStorm: WARNING: moving string \"" << contents << "\" - this is expensive." << std::endl;
  #endif // NDEBUG
  align = other.align;
  std::swap(contents, other.contents);
  thisfont = other.thisfont;
  std::swap(outline, other.outline);
  std::swap(fill,    other.fill);
  std::swap(back,    other.back);
  std::swap(edge,    other.edge);
  std::swap(bounds_left, other.bounds_left);
  std::swap(bounds_bottom, other.bounds_bottom);
  std::swap(bounds_right, other.bounds_right);
  std::swap(bounds_top, other.bounds_top);
}
string &string::operator=(string &&other) noexcept {
  /// Move assignment constructor
  #ifndef NDEBUG
    //std::cout << "VectorFontStorm: WARNING: moving string \"" << other.contents << "\" in assignment - this is expensive." << std::endl;
  #endif // NDEBUG
  align = other.align;
  std::swap(contents, other.contents);
  thisfont = other.thisfont;
  std::swap(outline, other.outline);
  std::swap(fill,    other.fill);
  std::swap(back,    other.back);
  std::swap(edge,    other.edge);
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
  back.destroy();
  edge.destroy();
}

void string::init(Vector3f const &position,
                  Quatf const &orientation,
                  float scale,
                  float depth) {
  /// Initialise this string
  outline.init();
  fill.init();
  back.init();
  edge.init();
  buffer_data data_outline;
  buffer_data data_fill;
  buffer_data data_back;
  buffer_data data_edge;

  Vector2f advance;
  Vector2f advance_max;
  float const line_height = static_cast<float>(thisfont.get_height());
  struct line {
    float width = 0.0f;
    GLuint index_from_outline = 0;
    GLuint index_from_fill    = 0;
    GLuint index_from_back    = 0;
    GLuint index_from_edge    = 0;
    GLuint index_to_outline   = 0;
    GLuint index_to_fill      = 0;
    GLuint index_to_back      = 0;
    GLuint index_to_edge      = 0;
  };
  std::vector<line> lines(1);
  for(auto const &thischar : contents) {
    if(thischar == '\n') {                                                      // handle newlines
      lines.back().width = advance.x;
      lines.back().index_to_outline = cast_if_required<GLuint>(data_outline.vbo.size());
      lines.back().index_to_fill    = cast_if_required<GLuint>(data_fill.vbo.size());
      lines.back().index_to_back    = cast_if_required<GLuint>(data_back.vbo.size());
      lines.back().index_to_edge    = cast_if_required<GLuint>(data_edge.vbo.size());
      advance.y += line_height;
      advance.x = 0.0f;
      lines.emplace_back();
      lines.back().index_from_outline = cast_if_required<GLuint>(data_outline.vbo.size());
      lines.back().index_from_fill    = cast_if_required<GLuint>(data_fill.vbo.size());
      lines.back().index_from_back    = cast_if_required<GLuint>(data_back.vbo.size());
      lines.back().index_from_edge    = cast_if_required<GLuint>(data_edge.vbo.size());
    } else {
      GLuint vbo_start_outline = cast_if_required<GLuint>(data_outline.vbo.size());
      GLuint vbo_start_fill    = cast_if_required<GLuint>(data_fill.vbo.size());
      GLuint vbo_start_back    = cast_if_required<GLuint>(data_back.vbo.size());
      GLuint vbo_start_edge    = cast_if_required<GLuint>(data_edge.vbo.size());
      float const new_advance  = thisfont.get_outline_and_fill(thischar, data_outline, data_fill, data_back, data_edge);
      GLuint vbo_end_outline   = cast_if_required<GLuint>(data_outline.vbo.size());
      GLuint vbo_end_fill      = cast_if_required<GLuint>(data_fill.vbo.size());
      GLuint vbo_end_back      = cast_if_required<GLuint>(data_back.vbo.size());
      GLuint vbo_end_edge      = cast_if_required<GLuint>(data_edge.vbo.size());
      for(GLuint p = vbo_start_outline; p != vbo_end_outline; ++p) {            // apply the previous advance to every point in this character, outline
        data_outline.vbo[p].x += advance.x;
        data_outline.vbo[p].y -= advance.y;
      }
      for(GLuint p = vbo_start_fill; p != vbo_end_fill; ++p) {                  // apply the previous advance to every point in this character, filled front
        data_fill.vbo[p].x += advance.x;
        data_fill.vbo[p].y -= advance.y;
      }
      for(GLuint p = vbo_start_back; p != vbo_end_back; ++p) {                  // apply the previous advance to every point in this character, filled back
        data_back.vbo[p].x += advance.x;
        data_back.vbo[p].y -= advance.y;
      }
      for(GLuint p = vbo_start_edge; p != vbo_end_edge; ++p) {                  // apply the previous advance to every point in this character, filled edge
        data_edge.vbo[p].x += advance.x;
        data_edge.vbo[p].y -= advance.y;
      }
      advance.x += new_advance;
    }
    advance_max = std::max(advance_max, advance);                               // track the widest line for alignment
  }
  lines.back().width = advance.x;
  lines.back().index_to_outline = cast_if_required<GLuint>(data_outline.vbo.size());
  lines.back().index_to_fill    = cast_if_required<GLuint>(data_fill.vbo.size());
  lines.back().index_to_back    = cast_if_required<GLuint>(data_back.vbo.size());
  lines.back().index_to_edge    = cast_if_required<GLuint>(data_edge.vbo.size());
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
      std::cout << "VectorFontStorm: DEBUG: centred string start" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM
    for(line const &this_line : lines) {
      float const offset = (advance_max.x - this_line.width) / 2.0f;            // offset for centre align
      #ifdef DEBUG_VECTORFONTSTORM
        std::cout << "VectorFontStorm: DEBUG: line advance " << advance_max.x << " width " << this_line.width << " offset " << offset << std::endl;
      #endif // DEBUG_VECTORFONTSTORM
      #ifdef DEBUG_VECTORFONTSTORM_DETAILED
        std::cout << "VectorFontStorm: DEBUG: this_line.index_from " << this_line.index_from << " this_line.index_to " << this_line.index_to << " data_outline.vbo.size() " << data_outline.vbo.size() << std::endl;
      #endif // DEBUG_VECTORFONTSTORM_DETAILED
      for(GLuint p = this_line.index_from_outline; p != this_line.index_to_outline; ++p) {  // slide every point in this line's outline by the offset
        data_outline.vbo[p].x += offset;
      }
      for(GLuint p = this_line.index_from_fill; p != this_line.index_to_fill; ++p) {  // slide every point in this line's front fill by the offset
        data_fill.vbo[p].x += offset;
      }
      for(GLuint p = this_line.index_from_back; p != this_line.index_to_back; ++p) {  // slide every point in this line's back fill by the offset
        data_back.vbo[p].x += offset;
      }
      for(GLuint p = this_line.index_from_edge; p != this_line.index_to_edge; ++p) {  // slide every point in this line's edge fill by the offset
        data_edge.vbo[p].x += offset;
      }
    }
    align_offset_width = advance_max.x * -0.5f;
    break;
  // right alignment
  case aligntype::TOPRIGHT:
  case aligntype::RIGHT:
  case aligntype::BOTTOMRIGHT:
    #ifdef DEBUG_VECTORFONTSTORM
      std::cout << "VectorFontStorm: DEBUG: right-aligned string start" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM
    for(line const &this_line : lines) {
      float const offset = advance_max.x - this_line.width;                     // offset for right align
      #ifdef DEBUG_VECTORFONTSTORM
        std::cout << "VectorFontStorm: DEBUG: line advance " << advance_max.x << " width " << this_line.width << " offset " << offset << std::endl;
      #endif // DEBUG_VECTORFONTSTORM
      #ifdef DEBUG_VECTORFONTSTORM_DETAILED
        std::cout << "VectorFontStorm: DEBUG: this_line.index_from " << this_line.index_from << " this_line.index_to " << this_line.index_to << " data_outline.vbo.size() " << data_outline.vbo.size() << std::endl;
      #endif // DEBUG_VECTORFONTSTORM_DETAILED
      for(GLuint p = this_line.index_from_outline; p != this_line.index_to_outline; ++p) {  // slide every point in this line's outline by the offset
        data_outline.vbo[p].x += offset;
      }
      for(GLuint p = this_line.index_from_fill; p != this_line.index_to_fill; ++p) {  // slide every point in this line's front fill by the offset
        data_fill.vbo[p].x += offset;
      }
      for(GLuint p = this_line.index_from_back; p != this_line.index_to_back; ++p) {  // slide every point in this line's back fill by the offset
        data_back.vbo[p].x += offset;
      }
      for(GLuint p = this_line.index_from_edge; p != this_line.index_to_edge; ++p) {  // slide every point in this line's edge fill by the offset
        data_edge.vbo[p].x += offset;
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
      //std::cout << "VectorFontStorm: DEBUG: string " << contents << std::endl;
      std::cout << "VectorFontStorm: DEBUG: string vertical advance " << advance_max.y << " align_offset_height " << align_offset_height << std::endl;
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

  for(auto &range : {&data_outline, &data_fill, &data_back, &data_edge}) {
    for(auto &p : range->vbo) {
      p.x += align_offset_width;
      p.y += align_offset_height;
      p.x *= scale;                                                             // scale the model, x
      p.y *= scale;                                                             // scale the model, y
      p.z *= depth;                                                             // apply depth
      p *= orientation;                                                         // rotate the model
      p += position;                                                            // position the model in space
      bounds_left   = std::min(bounds_left,   p.x);                             // update the outer bounds
      bounds_right  = std::max(bounds_right,  p.x);
      bounds_top    = std::min(bounds_top,    p.y);
      bounds_bottom = std::max(bounds_bottom, p.y);
    }
  }
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: String \"" << contents << "\" bounds " << get_bounds_2d() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: outline vbo size " << data_outline.vbo.size() << " ibo size " << data_outline.ibo.size() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: fill vbo size " << data_fill.vbo.size() << " ibo size " << data_fill.ibo.size() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: back vbo size " << data_back.vbo.size() << " ibo size " << data_back.ibo.size() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: edge vbo size " << data_edge.vbo.size() << " ibo size " << data_edge.ibo.size() << std::endl;
  #endif // DEBUG_VECTORFONTSTORM

  outline.upload(data_outline);
  fill.upload(data_fill);
  back.upload(data_back);
  edge.upload(data_edge);
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
  /// Render the front of string in fill mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  // wireframe
  //glDisable(GL_FOG);
  fill.render(GL_TRIANGLES);
}
void string::render_back() const {
  /// Render the back of this string in fill mode
  back.render(GL_TRIANGLES);
}
void string::render_edge() const {
  /// Render the extruded edge of this string in fill mode
  edge.render(GL_TRIANGLES);
}

}
