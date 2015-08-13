#include "contour.h"
#include "segment.h"

namespace vectorfontstorm {

contour::contour() {
  /// Default constructor
}

contour::~contour() {
  /// Default destructor
}

void contour::get_outline(std::vector<Vector3<GLfloat>> &vbo_data,
                          std::vector<GLuint>           &ibo_data) const {
  /// Output the outline for this contour to a buffer
  //std::cout << "VectorFontStorm: DEBUG:   Contour with " << segments.size() << " segments" << std::endl;
  for(segment const &s : segments) {
    s.get_outline(vbo_data, ibo_data);
  }
}

}
