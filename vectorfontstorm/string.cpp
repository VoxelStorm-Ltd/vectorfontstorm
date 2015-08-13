#include "string.h"
#include <vector>
#include "vmath.h"
#include "font.h"

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
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);

  std::vector<Vector3<GLfloat>> vbo_data;
  std::vector<GLuint>           ibo_data;

  float advance = 0.0f;
  for(auto const &thischar : contents) {
    GLuint vbo_start = vbo_data.size();
    float const new_advance = thisfont.get_outline(thischar, vbo_data, ibo_data);
    GLuint vbo_end = vbo_data.size();
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
  }

  num_verts = ibo_data.size();
  if(num_verts == 0) {
    std::cout << "VectorFontStorm: WARNING: not uploading zero-sized buffer for string \"" << newstring << "\"" << std::endl;
    return;
  }
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(Vector3<GLfloat>), &vbo_data[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_data.size() * sizeof(GLuint), &ibo_data[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

string::~string() {
  /// Default destructor
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
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
