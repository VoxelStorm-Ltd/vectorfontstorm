#ifndef VECTORFONTSTORM_STRING_H_INCLUDED
#define VECTORFONTSTORM_STRING_H_INCLUDED

#include <string>
#include <limits>
#include <vector>
#include "vectorstorm/vector/vector2.h"
#include "vectorstorm/vector/vector3.h"
#include "vectorstorm/quat/quat.h"
#include "vectorstorm/aabb/aabb2.h"
#include "vectorstorm/aabb/aabb3.h"
#include "buffer.h"
#include "aligntype.h"

namespace vectorfontstorm {

template<typename T> class font;

template<typename T>
class string {
private:
  std::string contents;
  vectorfontstorm::font<T> &thisfont;

  vector3f position;
  quatf orientation;
  float scale = 1.0f;
  float depth = 0.0f;

  aligntype align = aligntype::CENTRE;

public:
  buffer<T> outline;                                                            // the opengl buffer for the outline of the string
  buffer<T> fill;                                                               // the opengl buffer for the polygonal front fill of the string
  buffer<T> back;                                                               // the opengl buffer for the polygonal back fill of the string
  buffer<T> edge;                                                               // the opengl buffer for the polygonal extruded edge of the string

private:
  float bounds_left   = std::numeric_limits<float>::max();                      // outer-most points of the rendered model
  float bounds_bottom = std::numeric_limits<float>::max();
  float bounds_right  = std::numeric_limits<float>::lowest();
  float bounds_top    = std::numeric_limits<float>::lowest();

public:
  string(std::string const &newcontents,
         vectorfontstorm::font<T> &newfont,
         vector3f const &newpositnewion = {0.0f, 0.0f, 0.0f},
         quatf const &neworientation = {1.0f, 0.0f, 0.0f, 0.0f},
         float newscale = 1.0f,
         float newdepth = 1.0f,
         aligntype alignment = aligntype::CENTRE);
  string(string const &other) = delete;                                         // disallow copy
  string &operator=(string const &other) = delete;
  //string(string &&other) noexcept;                                              // allow move
  //string &operator=(string &&other) noexcept;
  string(string const &&other) = delete;                                        // disallow move
  string &operator=(string const &&other) = delete;
  ~string();
  //void swap(string<T> &other);

private:
  void init();

public:
  std::string const &get_contents() const __attribute__((__const__));

  //void update_contents(std::string const &new_contents);

  float get_bounds_left()   const __attribute__((__const__));
  float get_bounds_right()  const __attribute__((__const__));
  float get_bounds_top()    const __attribute__((__const__));
  float get_bounds_bottom() const __attribute__((__const__));
  vector2f get_bounds_bottomleft_2d() const;
  vector2f get_bounds_bottomright_2d() const;
  vector2f get_bounds_topleft_2d() const;
  vector2f get_bounds_topright_2d() const;
  vector3f get_bounds_bottomleft_3d() const;
  vector3f get_bounds_bottomright_3d() const;
  vector3f get_bounds_topleft_3d() const;
  vector3f get_bounds_topright_3d() const;
  aabb2f get_bounds_2d() const;
  aabb3f get_bounds_3d() const;
};

#include "cast_if_required.h"

template<typename T>
string<T>::string(std::string const &newstring,
                  vectorfontstorm::font<T> &newfont,
                  vector3f const &newposition,
                  quatf const &neworientation,
                  float newscale,
                  float newdepth,
                  aligntype alignment)
  : contents(newstring),
    thisfont(newfont),
    position(newposition),
    orientation(neworientation),
    scale(newscale),
    depth(newdepth),
    align(alignment) {
  /// Default constructor
  init();
}
/*
template<typename T>
string<T>::string(string &&other) noexcept
  : contents(std::move(other.contents)),
    thisfont(other.thisfont),
    position(other.position),
    orientation(other.orientation),
    scale(other.scale),
    depth(other.depth),
    align(other.align),
    bounds_left(  other.bounds_left),
    bounds_bottom(other.bounds_bottom),
    bounds_right( other.bounds_right),
    bounds_top(   other.bounds_top) {
  /// Move constructor
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: String \"" << contents << "\" moving from \"" << other.contents << "\"" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  #ifndef NDEBUG
    //std::cout << "VectorFontStorm: WARNING: moving string \"" << contents << "\" - this is expensive." << std::endl;
  #endif // NDEBUG
  using std::swap;                                                              // needed to allow our own swap to be found
  swap(outline, other.outline);                                                 // swap these rather than just move, to make destruction safe
  swap(fill,    other.fill);
  swap(back,    other.back);
  swap(edge,    other.edge);
}
template<typename T>
string<T> &string<T>::operator=(string<T> &&other) noexcept {
  /// Move assignment constructor
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: String \"" << contents << "\" moving from \"" << other.contents << "\" in assignment" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  #ifndef NDEBUG
    //std::cout << "VectorFontStorm: WARNING: moving string \"" << other.contents << "\" in assignment - this is expensive." << std::endl;
  #endif // NDEBUG
  using std::swap;                                                              // needed to allow our own swap to be found
  swap(position,    other.position);
  swap(orientation, other.orientation);
  scale = other.scale;
  depth = other.depth;
  align = other.align;
  swap(contents, other.contents);
  //swap(thisfont, other.thisfont);
  thisfont = other.thisfont;
  swap(outline,  other.outline);
  swap(fill,     other.fill);
  swap(back,     other.back);
  swap(edge,     other.edge);
  swap(bounds_left, other.bounds_left);
  swap(bounds_bottom, other.bounds_bottom);
  swap(bounds_right, other.bounds_right);
  swap(bounds_top, other.bounds_top);
  return *this;
}
template<typename T>
void string<T>::swap(string<T> &other) {
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: String \"" << contents << "\" member swapping with string \"" << other.contents << "\"" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  using std::swap;                                                              // needed to allow our own swap to be found
  swap(contents,      other.contents);
  //swap(thisfont,      other.thisfont);
  thisfont = other.thisfont;
  swap(position,      other.position);
  swap(orientation,   other.orientation);
  swap(scale,         other.scale);
  swap(depth,         other.depth);
  swap(align,         other.align);
  swap(outline,       other.outline);
  swap(fill,          other.fill);
  swap(back,          other.back);
  swap(edge,          other.edge);
  swap(bounds_left,   other.bounds_left);
  swap(bounds_bottom, other.bounds_bottom);
  swap(bounds_right,  other.bounds_right);
  swap(bounds_top,    other.bounds_top);
}
template<typename T>
void swap(string<T> &lhs, string<T> &rhs) {
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: String \"" << lhs.get_contents() << "\" swapping with string \"" << rhs.get_contents() << "\"" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  lhs.swap(rhs);
}
*/
template<typename T>
string<T>::~string() {
  /// Default destructor
  outline.destroy();
  fill.destroy();
  back.destroy();
  edge.destroy();
}

template<typename T>
void string<T>::init() {
  /// Initialise this string
  outline.init();
  fill.init();
  back.init();
  edge.init();
  buffer_data<T> data_outline;
  buffer_data<T> data_fill;
  buffer_data<T> data_back;
  buffer_data<T> data_edge;

  vector2f advance;
  vector2f advance_max;
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
      if(depth == 0.0f) {
        data_edge.vbo.clear();                                                  // discard edge data if this string has zero depth
        data_edge.ibo.clear();
      }
      GLuint vbo_end_outline   = cast_if_required<GLuint>(data_outline.vbo.size());
      GLuint vbo_end_fill      = cast_if_required<GLuint>(data_fill.vbo.size());
      GLuint vbo_end_back      = cast_if_required<GLuint>(data_back.vbo.size());
      GLuint vbo_end_edge      = cast_if_required<GLuint>(data_edge.vbo.size());
      for(GLuint p = vbo_start_outline; p != vbo_end_outline; ++p) {            // apply the previous advance to every point in this character, outline
        data_outline.vbo[p].coords.x += advance.x;
        data_outline.vbo[p].coords.y -= advance.y;
      }
      for(GLuint p = vbo_start_fill; p != vbo_end_fill; ++p) {                  // apply the previous advance to every point in this character, filled front
        data_fill.vbo[p].coords.x += advance.x;
        data_fill.vbo[p].coords.y -= advance.y;
      }
      for(GLuint p = vbo_start_back; p != vbo_end_back; ++p) {                  // apply the previous advance to every point in this character, filled back
        data_back.vbo[p].coords.x += advance.x;
        data_back.vbo[p].coords.y -= advance.y;
      }
      for(GLuint p = vbo_start_edge; p != vbo_end_edge; ++p) {                  // apply the previous advance to every point in this character, filled edge
        data_edge.vbo[p].coords.x += advance.x;
        data_edge.vbo[p].coords.y -= advance.y;
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
        data_outline.vbo[p].coords.x += offset;
      }
      for(GLuint p = this_line.index_from_fill; p != this_line.index_to_fill; ++p) {  // slide every point in this line's front fill by the offset
        data_fill.vbo[p].coords.x += offset;
      }
      for(GLuint p = this_line.index_from_back; p != this_line.index_to_back; ++p) {  // slide every point in this line's back fill by the offset
        data_back.vbo[p].coords.x += offset;
      }
      for(GLuint p = this_line.index_from_edge; p != this_line.index_to_edge; ++p) {  // slide every point in this line's edge fill by the offset
        data_edge.vbo[p].coords.x += offset;
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
        data_outline.vbo[p].coords.x += offset;
      }
      for(GLuint p = this_line.index_from_fill; p != this_line.index_to_fill; ++p) {  // slide every point in this line's front fill by the offset
        data_fill.vbo[p].coords.x += offset;
      }
      for(GLuint p = this_line.index_from_back; p != this_line.index_to_back; ++p) {  // slide every point in this line's back fill by the offset
        data_back.vbo[p].coords.x += offset;
      }
      for(GLuint p = this_line.index_from_edge; p != this_line.index_to_edge; ++p) {  // slide every point in this line's edge fill by the offset
        data_edge.vbo[p].coords.x += offset;
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
      p.coords.x += align_offset_width;
      p.coords.y += align_offset_height;
      p.coords.x *= scale;                                                      // scale the model, x
      p.coords.y *= scale;                                                      // scale the model, y
      p.coords.z *= depth;                                                      // apply depth
      bounds_left   = std::min(bounds_left,   p.coords.x);                      // update the outer bounds prior to shifting into place
      bounds_right  = std::max(bounds_right,  p.coords.x);
      bounds_bottom = std::min(bounds_bottom, p.coords.y);
      bounds_top    = std::max(bounds_top,    p.coords.y);
      p.coords *= orientation;                                                  // rotate the model
      p.coords += position;                                                     // position the model in space
    }
  }
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: String \"" << contents << "\" bounds " << get_bounds_2d() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: Position " << position << std::endl;
    std::cout << "VectorFontStorm: DEBUG: outline vbo size " << data_outline.vbo.size() << " ibo size " << data_outline.ibo.size() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: fill vbo size " << data_fill.vbo.size() << " ibo size " << data_fill.ibo.size() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: back vbo size " << data_back.vbo.size() << " ibo size " << data_back.ibo.size() << std::endl;
    std::cout << "VectorFontStorm: DEBUG: edge vbo size " << data_edge.vbo.size() << " ibo size " << data_edge.ibo.size() << std::endl;
  #endif // DEBUG_VECTORFONTSTORM

  outline.upload(data_outline);
  fill.upload(data_fill);
  back.upload(data_back);
  if(depth != 0.0f) {
    edge.upload(data_edge);
  }
}

template<typename T>
std::string const &string<T>::get_contents() const {
  return contents;
}

/*
template<typename T>
void string<T>::update_contents(std::string const &new_contents) {
  /// Reinitialise this with new contents
  contents = new_contents;
  init();
}
*/

template<typename T>
float string<T>::get_bounds_left() const {
  return bounds_left;
}
template<typename T>
float string<T>::get_bounds_right() const {
  return bounds_right;
}
template<typename T>
float string<T>::get_bounds_top() const {
  return bounds_top;
}
template<typename T>
float string<T>::get_bounds_bottom() const {
  return bounds_bottom;
}
template<typename T>
vector2f string<T>::get_bounds_bottomleft_2d() const {
  return vector2f(bounds_left, bounds_bottom);
}
template<typename T>
vector2f string<T>::get_bounds_bottomright_2d() const {
  return vector2f(bounds_right, bounds_bottom);
}
template<typename T>
vector2f string<T>::get_bounds_topleft_2d() const {
  return vector2f(bounds_left, bounds_top);
}
template<typename T>
vector2f string<T>::get_bounds_topright_2d() const {
  return vector2f(bounds_right, bounds_top);
}
template<typename T>
vector3f string<T>::get_bounds_bottomleft_3d() const {
  return vector3f(bounds_left, bounds_bottom, 0.0f);
}
template<typename T>
vector3f string<T>::get_bounds_bottomright_3d() const {
  return vector3f(bounds_right, bounds_bottom, 0.0f);
}
template<typename T>
vector3f string<T>::get_bounds_topleft_3d() const {
  return vector3f(bounds_left, bounds_top, 0.0f);
}
template<typename T>
vector3f string<T>::get_bounds_topright_3d() const {
  return vector3f(bounds_right, bounds_top, 0.0f);
}
template<typename T>
aabb2f string<T>::get_bounds_2d() const {
  return aabb2f(bounds_left, bounds_bottom, bounds_right, bounds_top);
}
template<typename T>
aabb3f string<T>::get_bounds_3d() const {
  return aabb3f(bounds_left, bounds_bottom, 0.0f, bounds_right, bounds_top, 0.0f);
}

}

#endif // VECTORFONTSTORM_STRING_H_INCLUDED
