#include "glyph.h"
#include <boost/range/iterator_range.hpp>
#include "vmath.h"
#include "poly2tri/poly2tri.h"
#include "cast_if_required.h"
#include "contour.h"
#ifdef DEBUG_VECTORFONTSTORM
  #include "timestorm/timer.h"
#endif // DEBUG_VECTORFONTSTORM

namespace vectorfontstorm {

glyph::glyph(char newchar, float newadvance)
  : advance(newadvance),
    character(newchar) {
  /// Default constructor
}

glyph::~glyph() {
  /// Default destructor
}

void glyph::correct_winding() {
  /// Make sure the winding of all curves in this glyph is in the correct direction
  /// The external outline clockwise, internal holes counterclockwise
  if(whitespace) {
    return;
  }
  if(contours.empty()) {
    whitespace = true;
    return;
  }
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: Glyph \"" << character << "\" correcting windings" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    unsigned int i = 0;
    for(auto const &c : contours) {
      std::cout << "VectorFontStorm: DEBUG: Glyph \"" << character << "\" contour " << i << " winding ";
      switch(c.get_winding()) {
      case contour::windingtype::CLOCKWISE:
        std::cout << "clockwise" << std::endl;
        break;
      case contour::windingtype::COUNTERCLOCKWISE:
        std::cout << "counterclockwise" << std::endl;
        break;
      case contour::windingtype::INDETERMINATE:
        std::cout << "indeterminate" << std::endl;
        break;
      }
      ++i;
    }
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  contour::windingtype const exterior = contours.front().get_winding();         // assume the first contour's winding is external style
  if(exterior == contour::windingtype::INTERNAL) {                              // winding is backward to what we need, so flip all windings
    for(auto &c : contours) {
      c.reverse();
    }
  }
}

void glyph::cache_buffer() {
  /// Walk through the glyph's components and cache the outline it makes
  #ifdef DEBUG_VECTORFONTSTORM
    std::stringstream ss;
    ss << "VectorFontStorm: Glyph \"" << character << "\" buffers cached in ";
    timestorm::timer<unsigned int> timer(ss.str());
    std::cout << "VectorFontStorm: DEBUG: Glyph \"" << character << "\" with " << contours.size() << " contours" << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  for(contour const &c : contours) {
    c.get_outline(outline);
    c.get_extrude(edge);
  }
  if(outline.vbo.empty()) {
    //std::cout << "VectorFontStorm: WARNING: Requested buffer from glyph with empty data_in.vbo!" << std::endl;
    whitespace = true;
    return;
  }

  // generate fills from outlines and holes
  std::vector<contour>::iterator c(contours.begin());
  while(c != contours.end()) {
    std::vector<p2t::Point*> polyline;
    c->get_outline(polyline);                                                   // the first must be an external contour
    //if(polyline.empty()) {
    //  std::cout << "VectorFontStorm: WARNING: Requested buffer from glyph with empty contour for fill!" << std::endl;
    //  return;
    //}
    p2t::SweepContext sweep_context(polyline);
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "VectorFontStorm: DEBUG: polyline size " << polyline.size() << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    ++c;
    while(c != contours.end() &&
          c->get_winding() == contour::windingtype::INTERNAL) {                 // iterate through all holes
      std::vector<p2t::Point*> polyline_hole;
      c->get_outline(polyline_hole);
      #ifdef DEBUG_VECTORFONTSTORM_DETAILED
        std::cout << "VectorFontStorm:   DEBUG: hole size " << polyline_hole.size() << std::endl;
      #endif // DEBUG_VECTORFONTSTORM_DETAILED
      sweep_context.AddHole(polyline_hole);
      ++c;
    }
    p2t::Sweep sweep;
    sweep.Triangulate(sweep_context);
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "VectorFontStorm: DEBUG: number of points:    " << sweep_context.points_.size() << std::endl;
      std::cout << "VectorFontStorm: DEBUG: number of triangles: " << sweep_context.triangles_.size() << std::endl;
      std::cout << "VectorFontStorm: DEBUG: triangle map size:   " << sweep_context.map_.size() << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    for(auto const &it : sweep_context.triangles_) {
      {
        // front face
        GLuint const index_start = cast_if_required<GLuint>(fill.vbo.size());
        fill.vbo.emplace_back(it->points_[0]->x, it->points_[0]->y, 0.0f);
        fill.vbo.emplace_back(it->points_[1]->x, it->points_[1]->y, 0.0f);
        fill.vbo.emplace_back(it->points_[2]->x, it->points_[2]->y, 0.0f);
        fill.ibo.emplace_back(index_start);
        fill.ibo.emplace_back(index_start + 1);
        fill.ibo.emplace_back(index_start + 2);
      }
      {
        // back face
        GLuint const index_start = cast_if_required<GLuint>(back.vbo.size());
        back.vbo.emplace_back(it->points_[0]->x, it->points_[0]->y, -1.0f);
        back.vbo.emplace_back(it->points_[2]->x, it->points_[2]->y, -1.0f);
        back.vbo.emplace_back(it->points_[1]->x, it->points_[1]->y, -1.0f);
        back.ibo.emplace_back(index_start);
        back.ibo.emplace_back(index_start + 1);
        back.ibo.emplace_back(index_start + 2);
      }
    }
  }
}

float glyph::get_advance() const {
  return advance;
}

void glyph::get_buffer(buffer_data const &data_in, buffer_data &data_out) {
  /// Output the vertices from the specified buffer to an indexed buffer, if non-empty
  if(whitespace) {
    return;
  } else {
    if(__builtin_expect(data_in.vbo.empty(), false)) {
      cache_buffer();
      if(whitespace) {
        return;
      }
    }
  }
  GLuint const vertex_offset = cast_if_required<GLuint>(data_out.vbo.size());
  data_out.vbo.reserve(data_out.vbo.size() + data_in.vbo.size());
  data_out.ibo.reserve(data_out.ibo.size() + data_in.ibo.size());
  auto const newrange_start  = data_out.ibo.end();                              // this NEEDS the reserve first, or the iterator will be invalidated
  // append the vectors to our target
  data_out.vbo.insert(data_out.vbo.end(), data_in.vbo.begin(), data_in.vbo.end());
  data_out.ibo.insert(data_out.ibo.end(), data_in.ibo.begin(), data_in.ibo.end());
  // transform the indices to the correct offset
  for(auto &it : boost::make_iterator_range(newrange_start, data_out.ibo.end())) {
    it += vertex_offset;
  }
}

void glyph::get_outline(buffer_data &data_out) {
  /// Output the vertices for this character to an indexed buffer
  get_buffer(outline, data_out);
}

void glyph::get_fill(buffer_data &data_out) {
  /// Output the vertices for this character to an indexed buffer - front fill
  get_buffer(fill, data_out);
}
void glyph::get_back(buffer_data &data_out) {
  /// Output the vertices for this character to an indexed buffer - back fill
  get_buffer(back, data_out);
}
void glyph::get_edge(buffer_data &data_out) {
  /// Output the vertices for this character to an indexed buffer - extruded edge
  get_buffer(edge, data_out);
}

}
