#include "segment.h"
#include "buffer_data.h"
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

void segment::get_outline(buffer_data &data_out) const {
  /// Output the outline for this segment to a buffer
  #ifdef DEBUG_VECTORFONTSTORM
    //std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  switch(type) {
  case segment::types::UNKNOWN:
    std::cout << "VectorFontStorm: ERROR: curve type unknown!" << std::endl;
    return;
  case segment::types::LINE:
    data_out.ibo.emplace_back(data_out.vbo.size());
    data_out.vbo.emplace_back(points.front().coords.x, points.front().coords.y, 0.0f);
    data_out.ibo.emplace_back(data_out.vbo.size());
    data_out.vbo.emplace_back(points.back().coords.x, points.back().coords.y, 0.0f);
    break;
  case segment::types::CONIC:
    {
      std::vector<segment> subdivisions;
      subdivide_conic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_outline(data_out);
      }
    }
    break;
  case segment::types::CUBIC:
    {
      std::vector<segment> subdivisions;
      subdivide_cubic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_outline(data_out);
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
