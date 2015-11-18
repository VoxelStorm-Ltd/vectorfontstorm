#include "segment.h"
#include "cast_if_required.h"
#include "poly2tri/poly2tri.h"
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
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
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

void segment::get_outline(std::vector<p2t::Point*> &polyline_out) const {
  /// Output the outline for this segment to a poly2tri polyline
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points, polyline size " << polyline_out.size() << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  switch(type) {
  case segment::types::UNKNOWN:
    std::cout << "VectorFontStorm: ERROR: curve type unknown!" << std::endl;
    return;
  case segment::types::LINE:
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "DEBUG: old " << polyline_out.back()->x << " " << polyline_out.back()->y << std::endl;
      std::cout << "DEBUG: new " << polyline_out.size() << ": " << points.back().coords << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    if(polyline_out.size() != 0) {
      if(polyline_out.back()->x == points.back().coords.x &&
         polyline_out.back()->y == points.back().coords.y) {
        #ifdef DEBUG_VECTORFONTSTORM_DETAILED
          std::cout << "DEBUG: [last duplicate removed]" << std::endl;
        #endif // DEBUG_VECTORFONTSTORM_DETAILED
        break;                                                                  // avoid repeating points
      }
    }
    //polyline_out.emplace_back(new p2t::Point(points.front().coords.x, points.front().coords.y));
    polyline_out.emplace_back(new p2t::Point(points.back().coords.x, points.back().coords.y));
    break;
  case segment::types::CONIC:
    {
      #ifdef DEBUG_VECTORFONTSTORM_DETAILED
        std::cout << "DEBUG: temp conic " << polyline_out.size() << ": " << points.back().coords.x << " " << points.back().coords.y << std::endl;
      #endif // DEBUG_VECTORFONTSTORM_DETAILED
      std::vector<segment> subdivisions;
      subdivide_conic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_outline(polyline_out);
      }
    }
    break;
  case segment::types::CUBIC:
    {
      #ifdef DEBUG_VECTORFONTSTORM_DETAILED
        std::cout << "DEBUG: temp cubic " << polyline_out.size() << ": " << points.back().coords.x << " " << points.back().coords.y << std::endl;
      #endif // DEBUG_VECTORFONTSTORM_DETAILED
      std::vector<segment> subdivisions;
      subdivide_cubic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_outline(polyline_out);
      }
    }
    break;
  }
}

void segment::get_outline_first(std::vector<p2t::Point*> &polyline_out) const {
  /// Return just the first point of the outline
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "DEBUG: first      " << polyline_out.size() << ": " << points.front().coords.x << " " << points.front().coords.y << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  polyline_out.emplace_back(new p2t::Point(points.front().coords.x, points.front().coords.y));
}

void segment::get_extrude(buffer_data &data_out) const {
  /// Output the extruded outline of this segment to a buffer
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points - extrude" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  switch(type) {
  case segment::types::UNKNOWN:
    std::cout << "VectorFontStorm: ERROR: curve type unknown!" << std::endl;
    return;
  case segment::types::LINE:
    {
      GLuint const index_start = cast_if_required<GLuint>(data_out.vbo.size());
      #define VECTORFONTSTORM_DUPLICATE_VERTS
      #ifdef VECTORFONTSTORM_DUPLICATE_VERTS
        data_out.vbo.emplace_back(points.front().coords.x, points.front().coords.y, -1.0f);
        data_out.vbo.emplace_back(points.front().coords.x, points.front().coords.y,  0.0f);
        data_out.vbo.emplace_back(points.back().coords.x,  points.back().coords.y,   0.0f);
        data_out.vbo.emplace_back(points.back().coords.x,  points.back().coords.y,   0.0f);
        data_out.vbo.emplace_back(points.back().coords.x,  points.back().coords.y,  -1.0f);
        data_out.vbo.emplace_back(points.front().coords.x, points.front().coords.y, -1.0f);
        data_out.ibo.emplace_back(index_start + 0);
        data_out.ibo.emplace_back(index_start + 1);
        data_out.ibo.emplace_back(index_start + 2);
        data_out.ibo.emplace_back(index_start + 3);
        data_out.ibo.emplace_back(index_start + 4);
        data_out.ibo.emplace_back(index_start + 5);
      #else // VECTORFONTSTORM_DUPLICATE_VERTS
        data_out.vbo.emplace_back(points.front().coords.x, points.front().coords.y, -1.0f);
        data_out.vbo.emplace_back(points.front().coords.x, points.front().coords.y,  0.0f);
        data_out.vbo.emplace_back(points.back().coords.x,  points.back().coords.y,   0.0f);
        data_out.vbo.emplace_back(points.back().coords.x,  points.back().coords.y,  -1.0f);
        data_out.ibo.emplace_back(index_start + 0);
        data_out.ibo.emplace_back(index_start + 1);
        data_out.ibo.emplace_back(index_start + 2);
        data_out.ibo.emplace_back(index_start + 2);
        data_out.ibo.emplace_back(index_start + 3);
        data_out.ibo.emplace_back(index_start + 0);
      #endif // VECTORFONTSTORM_DUPLICATE_VERTS
    }
    break;
  case segment::types::CONIC:
    {
      std::vector<segment> subdivisions;
      subdivide_conic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_extrude(data_out);
      }
    }
    break;
  case segment::types::CUBIC:
    {
      std::vector<segment> subdivisions;
      subdivide_cubic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_extrude(data_out);
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

void segment::reverse() {
  /// Reverse this segment's points from back to front
  std::reverse(points.begin(), points.end());                                   // reverse the order of points
}

}
