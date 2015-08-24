#include "segment.h"
#include "point.h"

namespace vectorfontstorm {

segment::segment() {
  /// Default constructor
}

segment::~segment() {
  /// Default destructor
}

point &segment::get_second_to_last_point() {
  /// Helper function to fetch the penultimate point from the list
  //return points.rbegin()[1];
  // the above seems to produce corrupt results only when run under gdb!  Possibly a gdb bug.
  return points[points.size() - 1];
}

void segment::get_outline(std::vector<Vector3<GLfloat>> &vbo_data,
                          std::vector<GLuint>           &ibo_data) const {
  /// Output the outline for this segment to a buffer
  #ifdef DEBUG_VECTORFONTSTORM
    //std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  switch(type) {
  case segment::types::UNKNOWN:
    std::cout << "VectorFontStorm: ERROR: curve type unknown!" << std::endl;
    return;
  case segment::types::LINE:
    ibo_data.emplace_back(vbo_data.size());
    vbo_data.emplace_back(points.front().coords.x, points.front().coords.y, 0.0f);
    ibo_data.emplace_back(vbo_data.size());
    vbo_data.emplace_back(points.back().coords.x, points.back().coords.y, 0.0f);
    break;
  case segment::types::CONIC:
    {
      std::vector<segment> subdivisions;
      subdivide_conic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_outline(vbo_data, ibo_data);
      }
    }
    break;
  case segment::types::CUBIC:
    {
      std::vector<segment> subdivisions;
      subdivide_cubic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_outline(vbo_data, ibo_data);
      }
    }
    break;
  }
}

void segment::subdivide_conic(std::vector<segment> &new_segments) const {
  /// Take this curve and return a subdivision
  // placeholder - treat curves as line segments for now
  new_segments.emplace_back(segment());
  new_segments.back().type = types::LINE;
  new_segments.back().points.emplace_back(points.front().coords, point::types::ON);
  new_segments.back().points.emplace_back(points.back().coords,  point::types::ON);
  // TODO: minimum segment length test
  // TODO: colinearity test
}
void segment::subdivide_cubic(std::vector<segment> &new_segments) const {
  /// Take this curve and return a subdivision
  // placeholder - treat curves as line segments for now
  new_segments.emplace_back(segment());
  new_segments.back().type = types::LINE;
  new_segments.back().points.emplace_back(points.front().coords, point::types::ON);
  new_segments.back().points.emplace_back(points.back().coords,  point::types::ON);
}

}
