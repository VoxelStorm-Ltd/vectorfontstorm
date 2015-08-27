#include "string.h"
#include <vector>
#include "vmath.h"
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
  : align(alignment),
    contents(newstring),
    thisfont(newfont) {
  /// Default constructor
  init(position, orientation, scale);
}
string::string(string &&other) noexcept
  : align(other.align),
    contents(std::move(other.contents)),
    thisfont(other.thisfont) {
  /// Move constructor
  #ifndef NDEBUG
    std::cout << "VectorFontStorm: WARNING: moving string \"" << contents << "\" - this is expensive." << std::endl;
  #endif // NDEBUG
  align = other.align;
  std::swap(contents, other.contents);
  thisfont = other.thisfont;
  std::swap(vbo, other.vbo);
  std::swap(ibo, other.ibo);
  std::swap(num_verts, other.num_verts);
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
  std::swap(vbo, other.vbo);
  std::swap(ibo, other.ibo);
  std::swap(num_verts, other.num_verts);
  std::swap(bounds_left, other.bounds_left);
  std::swap(bounds_bottom, other.bounds_bottom);
  std::swap(bounds_right, other.bounds_right);
  std::swap(bounds_top, other.bounds_top);
  return *this;
}

string::~string() {
  /// Default destructor
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
}

void string::init(Vector3f const &position,
                  Quatf const &orientation,
                  double scale) {
  /// Initialise this string
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);

  std::vector<Vector3<GLfloat>> vbo_data;
  std::vector<GLuint>           ibo_data;

  float advance = 0.0f;
  for(auto const &thischar : contents) {
    GLuint vbo_start = static_cast<GLuint>(vbo_data.size());
    float const new_advance = thisfont.get_outline(thischar, vbo_data, ibo_data);
    GLuint vbo_end = static_cast<GLuint>(vbo_data.size());
    // apply the previous advance to every point in this character
    for(GLuint p = vbo_start; p != vbo_end; ++p) {
      vbo_data[p].x += advance;
    }
    advance += new_advance;
  }
  float align_offset_width, align_offset_height;
  switch(align) {                                                               // horizontal alignment
  case aligntype::TOPLEFT:
  case aligntype::LEFT:
  case aligntype::BOTTOMLEFT:
  default:
    align_offset_width = 0.0f;
    break;
  case aligntype::TOP:
  case aligntype::CENTRE:
  case aligntype::BOTTOM:
    align_offset_width = advance * -0.5f;
    break;
  case aligntype::TOPRIGHT:
  case aligntype::RIGHT:
  case aligntype::BOTTOMRIGHT:
    align_offset_width = -advance;
    break;
  }
  switch(align) {                                                               // vertical alignment
  case aligntype::TOPLEFT:
  case aligntype::TOP:
  case aligntype::TOPRIGHT:
    align_offset_height = static_cast<float>(-thisfont.get_height());
    break;
  case aligntype::LEFT:
  case aligntype::RIGHT:
  case aligntype::CENTRE:
    align_offset_height = static_cast<float>(thisfont.get_height() * -0.5);
    break;
  case aligntype::BOTTOMLEFT:
  case aligntype::BOTTOM:
  case aligntype::BOTTOMRIGHT:
  default:
    align_offset_height = 0.0f;
    break;
  }

  for(auto &p : vbo_data) {
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
  #endif // DEBUG_VECTORFONTSTORM

  num_verts = static_cast<GLuint>(ibo_data.size());
  if(num_verts == 0) {
    std::cout << "VectorFontStorm: WARNING: not uploading zero-sized buffer for string \"" << contents << "\"" << std::endl;
    return;
  }
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(Vector3<GLfloat>), &vbo_data[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_data.size() * sizeof(GLuint), &ibo_data[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

void string::render() const {
  /// Render this string
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // wireframe
  //glDisable(GL_FOG);

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_LINES, num_verts, GL_UNSIGNED_INT, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
