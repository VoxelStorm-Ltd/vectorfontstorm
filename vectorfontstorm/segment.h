#ifndef VECTORFONTSTORM_SEGMENT_H_INCLUDED
#define VECTORFONTSTORM_SEGMENT_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "cast_if_required.h"
#include "poly2tri/poly2tri.h"
#include "point.h"

namespace vectorfontstorm {

template<typename T> struct buffer_data;
class point;

template<typename T>
class segment {
  template<typename U> friend class font;

  enum class types {
    UNKNOWN,                                                                    // not yet defined
    LINE,                                                                       // two successive ON points form a line segment
    CONIC,                                                                      // a conic OFF point between two successive ON points forms a conic bezier arc
    CUBIC                                                                       // two cubic OFF points between two ON points forms a cubic bezier arc
  } type = types::UNKNOWN;

  std::vector<point> points;

public:
  segment();
  ~segment();

  point &get_second_to_last_point() __attribute__((__pure__));

  void get_outline(buffer_data<T> &data_out) const;
  void get_outline(p2t::polylinetype &polyline_out) const;
  void get_outline_first(p2t::polylinetype &polyline_out) const;
  void get_extrude(buffer_data<T> &data_out) const;

  void subdivide_conic(std::vector<segment> &new_segments) const;
  void subdivide_cubic(std::vector<segment> &new_segments) const;

  void reverse();
};

template<typename T>
segment<T>::segment() {
  /// Default constructor
}

template<typename T>
segment<T>::~segment() {
  /// Default destructor
}

template<typename T>
point &segment<T>::get_second_to_last_point() {
  /// Helper function to fetch the penultimate point from the list
  //return points.rbegin()[1];
  // the above seems to produce corrupt results only when run under gdb!  Possibly a gdb bug.
  return points[points.size() - 1];
}

template<typename T>
void segment<T>::get_outline(buffer_data<T> &data_out) const {
  /// Output the outline for this segment to a buffer
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  switch(type) {
  case segment<T>::types::UNKNOWN:
    std::cout << "VectorFontStorm: ERROR: curve type unknown!" << std::endl;
    return;
  case segment<T>::types::LINE:
    data_out.ibo.emplace_back(data_out.vbo.size());
    data_out.vbo.emplace_back(points.front().coords.x, points.front().coords.y, 0.0f);
    data_out.ibo.emplace_back(data_out.vbo.size());
    data_out.vbo.emplace_back(points.back().coords.x, points.back().coords.y, 0.0f);
    break;
  case segment<T>::types::CONIC:
    {
      std::vector<segment> subdivisions;
      subdivide_conic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_outline(data_out);
      }
    }
    break;
  case segment<T>::types::CUBIC:
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

template<typename T>
void segment<T>::get_outline(p2t::polylinetype &polyline_out) const {
  /// Output the outline for this segment to a poly2tri polyline
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points, polyline size " << polyline_out.size() << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  switch(type) {
  case segment<T>::types::UNKNOWN:
    std::cout << "VectorFontStorm: ERROR: curve type unknown!" << std::endl;
    return;
  case segment<T>::types::LINE:
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
  case segment<T>::types::CONIC:
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
  case segment<T>::types::CUBIC:
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

template<typename T>
void segment<T>::get_outline_first(p2t::polylinetype &polyline_out) const {
  /// Return just the first point of the outline
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "DEBUG: first      " << polyline_out.size() << ": " << points.front().coords.x << " " << points.front().coords.y << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  polyline_out.emplace_back(new p2t::Point(points.front().coords.x, points.front().coords.y));
}

template<typename T>
void segment<T>::get_extrude(buffer_data<T> &data_out) const {
  /// Output the extruded outline of this segment to a buffer
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:     Segment with " << points.size() << " points - extrude" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  switch(type) {
  case segment<T>::types::UNKNOWN:
    std::cout << "VectorFontStorm: ERROR: curve type unknown!" << std::endl;
    return;
  case segment<T>::types::LINE:
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
  case segment<T>::types::CONIC:
    {
      std::vector<segment> subdivisions;
      subdivide_conic(subdivisions);
      for(auto const &s : subdivisions) {
        s.get_extrude(data_out);
      }
    }
    break;
  case segment<T>::types::CUBIC:
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

template<typename T>
void segment<T>::subdivide_conic(std::vector<segment<T>> &new_segments) const {
  /// Take this curve and return a subdivision
  // placeholder - treat curves as line segments for now
  new_segments.emplace_back();
  new_segments.back().type = types::LINE;
  new_segments.back().points.emplace_back(points.front().coords, point::types::ON);
  new_segments.back().points.emplace_back(points.back().coords,  point::types::ON);
  // TODO: minimum segment length test
  // TODO: colinearity test
}
template<typename T>
void segment<T>::subdivide_cubic(std::vector<segment<T>> &new_segments) const {
  /// Take this curve and return a subdivision
  // placeholder - treat curves as line segments for now
  new_segments.emplace_back();
  new_segments.back().type = types::LINE;
  new_segments.back().points.emplace_back(points.front().coords, point::types::ON);
  new_segments.back().points.emplace_back(points.back().coords,  point::types::ON);
}

template<typename T>
void segment<T>::reverse() {
  /// Reverse this segment's points from back to front
  std::reverse(points.begin(), points.end());                                   // reverse the order of points
}

}

#endif // VECTORFONTSTORM_SEGMENT_H_INCLUDED
