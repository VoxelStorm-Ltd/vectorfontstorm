#ifndef VECTORFONTSTORM_CONTOUR_H_INCLUDED
#define VECTORFONTSTORM_CONTOUR_H_INCLUDED

#include <vector>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "cast_if_required.h"
#include "poly2tri/poly2tri.h"

namespace vectorfontstorm {

template<typename T> struct buffer_data;
template<typename T> class segment;

template<typename T>
class contour {
  template<typename U> friend class font;
public:
  enum class windingtype : char {                                               // polygon winding types
    UNTESTED,                                                                   // the winding has not yet been checked
    INDETERMINATE,                                                              // winding has been checked but cannot be determined (too few points?)
    CLOCKWISE,                                                                  // the polygon is wound clockwise
    COUNTERCLOCKWISE,                                                           // the polygon is wound counterclockwise
    EXTERNAL = CLOCKWISE,                                                       // which winding is used for external polygons (outlines)
    INTERNAL = COUNTERCLOCKWISE                                                 // which winding is used for internal polygons (holes)
  } winding = windingtype::UNTESTED;                                            // what sort of winding this polygon has - cached

private:
  std::vector<segment<T>> segments;

public:
  contour();
  ~contour();

  segment<T> &get_second_to_last_segment() __attribute__((__pure__));

  void get_outline(buffer_data<T> &data_out) const;
  void get_outline(std::vector<p2t::Point*> &polyline_out) const;
  void get_extrude(buffer_data<T> &data_out) const;

  windingtype get_winding();
  void set_winding_clockwise();
  void set_winding_counterclockwise();

  void reverse();
};

template<typename T>
contour<T>::contour() {
  /// Default constructor
}

template<typename T>
contour<T>::~contour() {
  /// Default destructor
}

template<typename T>
segment<T> &contour<T>::get_second_to_last_segment() {
  /// Helper function to fetch the penultimate point from the list
  //return segments.rbegin()[1];
  // the above seems to produce corrupt results only when run under gdb!  Possibly a gdb bug.
  return segments[segments.size() - 1];
}

template<typename T>
void contour<T>::get_outline(buffer_data<T> &data_out) const {
  /// Output the outline for this contour to a buffer
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:   Contour with " << segments.size() << " segments" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  for(auto const &s : segments) {
    s.get_outline(data_out);
  }
}

template<typename T>
void contour<T>::get_outline(std::vector<p2t::Point*> &polyline_out) const {
  /// Output the outline for this contour to a poly2tri polyline
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:   Contour with " << segments.size() << " segments (polyline)" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  unsigned int const index_start = cast_if_required<unsigned int>(polyline_out.size());
  segments.front().get_outline_first(polyline_out);
  for(auto const &s : segments) {
    s.get_outline(polyline_out);
  }
  if(polyline_out.size() == index_start) {
    //#ifdef DEBUG_VECTORFONTSTORM
      std::cout << "VectorFontStorm: WARNING: Contour added nothing to polyline (size " << polyline_out.size() << ")" << std::endl;
    //#endif // DEBUG_VECTORFONTSTORM
  } else {
    if(*polyline_out.back() == *polyline_out[index_start]) {
      // lop off the last point again, if it repeats the starting point - polylines are implicitly closed
      #ifdef DEBUG_VECTORFONTSTORM_DETAILED
        std::cout << "VectorFontStorm: DEBUG:   Removing repeated start/end point at " << polyline_out.back()->x << " " << polyline_out.back()->y << std::endl;
      #endif // DEBUG_VECTORFONTSTORM_DETAILED
      polyline_out.pop_back();
    } else {
      #ifdef DEBUG_VECTORFONTSTORM
        std::cout << "VectorFontStorm: DEBUG:   contour appears to be open, start " << polyline_out[index_start]->x << " " << polyline_out[index_start]->y << " end " << polyline_out.back()->x << " " << polyline_out.back()->y << std::endl;
      #endif // DEBUG_VECTORFONTSTORM
    }
  }
}

template<typename T>
void contour<T>::get_extrude(buffer_data<T> &data_out) const {
  /// Output the extruded outline of this contour to a buffer
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG:   Contour with " << segments.size() << " segments - extrude" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  for(auto const &s : segments) {
    s.get_extrude(data_out);
  }
}

template<typename T>
typename contour<T>::windingtype contour<T>::get_winding() {
  /// Find out the orientation of this contour's winding
  if(winding != windingtype::UNTESTED) {
    return winding;                                                             // use the cached value if it's known
  }
  // adapted with reference to http://debian.fmi.uni-sofia.bg/~sergei/cgsr/docs/clockwise.htm
  // note: this may be unnecessarily expensive - you could do this with just two segments around one convex point, see https://en.wikipedia.org/wiki/Curve_orientation
  std::vector<p2t::Point*> polyline;
  get_outline(polyline);
  if(polyline.size() < 3) {
    std::cout << "VectorFontStorm: WARNING: Contour has too few points to determine winding!" << std::endl;
    winding = windingtype::INDETERMINATE;
    return winding;
  }
  int count = 0;
  for(unsigned int i = 0; i != polyline.size(); ++i) {
    int j = (i + 1) % cast_if_required<int>(polyline.size());
    int k = (i + 2) % cast_if_required<int>(polyline.size());
    float const z = ((polyline[j]->x - polyline[i]->x) *
                     (polyline[k]->y - polyline[j]->y)) -
                    ((polyline[j]->y - polyline[i]->y) *
                     (polyline[k]->x - polyline[j]->x));
    if(z < 0.0f) {
      --count;
    } else if(z > 0.0f) {
      ++count;
    }
  }
  if(count > 0) {
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "VectorFontStorm: DEBUG:   Contour has counterclockwise winding (" << count << " more left turns)" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    winding = windingtype::COUNTERCLOCKWISE;
  } else if(count == 0) {
    std::cout << "VectorFontStorm: WARNING: Could not determine winding of a contour!" << std::endl;
    winding = windingtype::INDETERMINATE;
  } else {
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "VectorFontStorm: DEBUG:   Contour has clockwise winding (" << -count << " more right turns)" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    winding = windingtype::CLOCKWISE;
  }
  return winding;
}

template<typename T>
void contour<T>::set_winding_clockwise() {
  /// Make this winding clockwise by reversing it if necessary
  if(get_winding() == windingtype::COUNTERCLOCKWISE) {
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "VectorFontStorm: DEBUG:   Contour correcting winding to clockwise (outside)" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    reverse();
  }
}
template<typename T>
void contour<T>::set_winding_counterclockwise() {
  /// Make this winding counterclockwise by reversing it if necessary
  if(get_winding() == windingtype::CLOCKWISE) {
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "VectorFontStorm: DEBUG:   Contour correcting winding to counterclockwise (hole)" << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    reverse();
  }
}

template<typename T>
void contour<T>::reverse() {
  /// Reverse this contour's segments from back to front
  std::reverse(segments.begin(), segments.end());                               // reverse the order of segments
  for(auto &s : segments) {
    s.reverse();                                                                // reverse the content of the segments
  }
  if(winding == windingtype::CLOCKWISE) {                                       // reverse the cached winding value, if known
    winding = windingtype::COUNTERCLOCKWISE;
  } else if(winding == windingtype::COUNTERCLOCKWISE) {
    winding = windingtype::CLOCKWISE;
  }
}

}

#endif // VECTORFONTSTORM_CONTOUR_H_INCLUDED
