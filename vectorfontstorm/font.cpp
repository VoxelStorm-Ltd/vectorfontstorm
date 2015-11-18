#include "font.h"
#include "glyph.h"
#include "contour.h"
#include "segment.h"
#include "point.h"

namespace vectorfontstorm {

font::font(unsigned char const *const memory_offset,
           size_t const memory_size,
           double desired_height)
  : height(desired_height) {
  /// Default constructor
  FT_Init_FreeType(&library);                                                   // initialise library
  FT_New_Memory_Face(library, memory_offset, memory_size, 0, &face);            // load font from memory
  FT_Select_Charmap(face, FT_ENCODING_UNICODE);                                 // select charmap
  float constexpr hres      = 64;                                               // from #define HRES 64 - Freetype uses 1/64th of a point scale
  float constexpr dpi       = 72;
  float const     font_size = face->units_per_EM;                               // use this for perfect scaling
  FT_F26Dot6 const font_size_ft = static_cast<FT_F26Dot6>(font_size * hres);    // something something fake fixed point conversion
  FT_Set_Char_Size(face, 0, font_size_ft, dpi, dpi);                            // set char size
  FT_Set_Transform(face, nullptr, nullptr);                                     // set transform matrix - identity

  scale = height / static_cast<double>(face->size->metrics.height);

  #ifndef NDEBUG
    std::cout << "VectorFontStorm: Loading font size " << memory_size / 1024 << "KB" << std::endl;
    /*
    std::cout << "VectorFontStorm: DEBUG: face->num_faces " << face->num_faces << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->face_index " << face->face_index << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->face_flags " << face->face_flags << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->style_flags " << face->style_flags << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->num_glyphs " << face->num_glyphs << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->family_name " << face->family_name << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->style_name " << face->style_name << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->num_fixed_sizes " << face->num_fixed_sizes << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->available_sizes " << face->available_sizes << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->num_charmaps " << face->num_charmaps << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->units_per_EM " << face->units_per_EM << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->ascender " << face->ascender << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->descender " << face->descender << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->height " << face->height << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->max_advance_width " << face->max_advance_width << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->max_advance_height " << face->max_advance_height << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->underline_position " << face->underline_position << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->underline_thickness " << face->underline_thickness << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.x_ppem; " << face->size->metrics.x_ppem << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.y_ppem; " << face->size->metrics.y_ppem << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.x_scale; " << face->size->metrics.x_scale << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.y_scale; " << face->size->metrics.y_scale << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.ascender; " << face->size->metrics.ascender << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.descender; " << face->size->metrics.descender << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.height " << face->size->metrics.height << std::endl;
    std::cout << "VectorFontStorm: DEBUG: face->size->metrics.max_advance " << face->size->metrics.max_advance << std::endl;
    std::cout << "VectorFontStorm: DEBUG: normalising scale " << scale << std::endl;
    */
  #endif // NDEBUG
}

font::~font() {
  /// Default destructor
  FT_Done_Face(face);                                                           // release the font
  FT_Done_FreeType(library);                                                    // shut down library
}

double font::get_height() const {
  return height;
}

float font::get_outline(char const thischar,
                        buffer_data &data_out) {
  /// Output the vertices for this character's outline to an indexed buffer, and return the advance
  auto it = glyphs.find(thischar);
  glyph &thisglyph = it == glyphs.end() ? load_glyph_from_freetype(thischar, it) : it->second;
  thisglyph.get_outline(data_out);
  return thisglyph.get_advance();
}
float font::get_fill(char const thischar,
                     buffer_data &data_out) {
  /// Output the vertices for this character's fill to an indexed buffer, and return the advance
  auto it = glyphs.find(thischar);
  glyph &thisglyph = it == glyphs.end() ? load_glyph_from_freetype(thischar, it) : it->second;
  thisglyph.get_fill(data_out);
  return thisglyph.get_advance();
}
float font::get_back(char const thischar,
                     buffer_data &data_out) {
  /// Output the vertices for this character's fill to an indexed buffer, and return the advance
  auto it = glyphs.find(thischar);
  glyph &thisglyph = it == glyphs.end() ? load_glyph_from_freetype(thischar, it) : it->second;
  thisglyph.get_back(data_out);
  return thisglyph.get_advance();
}
float font::get_edge(char const thischar,
                     buffer_data &data_out) {
  /// Output the vertices for this character's fill to an indexed buffer, and return the advance
  auto it = glyphs.find(thischar);
  glyph &thisglyph = it == glyphs.end() ? load_glyph_from_freetype(thischar, it) : it->second;
  thisglyph.get_edge(data_out);
  return thisglyph.get_advance();
}
float font::get_outline_and_fill(char const thischar,
                                 buffer_data &data_out_outline,
                                 buffer_data &data_out_fill,
                                 buffer_data &data_out_back,
                                 buffer_data &data_out_edge) {
  /// Output the vertices for this character's fill and outlines to an indexed buffer, and return the advance
  auto it = glyphs.find(thischar);
  glyph &thisglyph = it == glyphs.end() ? load_glyph_from_freetype(thischar, it) : it->second;
  thisglyph.correct_winding();
  thisglyph.get_outline(data_out_outline);
  thisglyph.get_fill(data_out_fill);
  thisglyph.get_back(data_out_back);
  thisglyph.get_edge(data_out_edge);
  return thisglyph.get_advance();
}

glyph &font::load_glyph_from_freetype(char const thischar, std::unordered_map<char, glyph>::iterator &it) {
  /// no glyph exists for this char yet, so construct and insert one
  FT_UInt glyph_index = FT_Get_Char_Index(face, thischar);
  #ifdef DEBUG_VECTORFONTSTORM
    std::cout << "VectorFontStorm: DEBUG: glyph: " << thischar << " index " << glyph_index << std::endl;
  #endif // DEBUG_VECTORFONTSTORM
  FT_Int32 flags = 0;
  flags |= FT_LOAD_TARGET_NORMAL;                                               // loading target
  flags |= FT_LOAD_NO_BITMAP;                                                   // freetype-gl default when using outlines
  flags |= FT_LOAD_RENDER;                                                      // freetype-gl default when using normal rendering
  FT_Load_Glyph(face, glyph_index, flags);
  FT_Outline const &outline = face->glyph->outline;
  #ifdef DEBUG_VECTORFONTSTORM_DETAILED
    std::cout << "VectorFontStorm: DEBUG: outline.n_contours " << outline.n_contours << std::endl;
    std::cout << "VectorFontStorm: DEBUG: outline.n_points " << outline.n_points << std::endl;
  #endif // DEBUG_VECTORFONTSTORM_DETAILED
  it = glyphs.emplace(thischar, glyph(thischar, static_cast<float>(static_cast<double>(face->glyph->metrics.horiAdvance) * scale))).first;
  glyph &thisglyph(it->second);
  for(int point_start = 0, c = 0; c < outline.n_contours; ++c) {                // can n_contours be contours?  I've no idea
    thisglyph.contours.emplace_back();
    contour &thiscontour(thisglyph.contours.back());
    thiscontour.segments.emplace_back();                                        // start the first segment
    int point_end = outline.contours[c];
    #ifdef DEBUG_VECTORFONTSTORM_DETAILED
      std::cout << "VectorFontStorm: DEBUG: contour " << c << " points " << point_start << "->" << point_end << std::endl;
    #endif // DEBUG_VECTORFONTSTORM_DETAILED
    for(int p = point_start; p <= point_end; ++p) {                             // note: we're NOT stopping one short of the end point, last segment goes on top
      point::types const ptype = outline.tags[p] & 0b00000001 ? point::types::ON : (outline.tags[p] & 0b00000010 ? point::types::OFF_THIRDORDER : point::types::OFF_SECONDORDER);
      thiscontour.segments.back().points.emplace_back(static_cast<double>(outline.points[p].x) * scale, static_cast<double>(outline.points[p].y) * scale, ptype);
      point const thispoint(thiscontour.segments.back().points.back());         // copy, do not take a reference!
      #ifdef DEBUG_VECTORFONTSTORM_DETAILED
        std::cout << "VectorFontStorm: DEBUG: processing " << thiscontour.segments.size() << ":" << thiscontour.segments.back().points.size() << std::endl;
      #endif // DEBUG_VECTORFONTSTORM_DETAILED
      switch(ptype) {
      case point::types::ON:
        if(thiscontour.segments.back().points.size() == 1) {                    // is this the first point of the segment?
          // this is the first point of the segment, so we don't know the segment's type yet
        } else {
          point &prevpoint(thiscontour.segments.back().get_second_to_last_point()); // get the second-to-last
          switch(prevpoint.type) {
          case point::types::ON:
            thiscontour.segments.back().type = segment::types::LINE;            // this was a straight line
            #ifdef DEBUG_VECTORFONTSTORM_DETAILED
              std::cout << "VectorFontStorm: DEBUG: " << thiscontour.segments.size() << ":" << thiscontour.segments.back().points.size() - 1 << " was a straight line at " << prevpoint.coords << std::endl;
            #endif // DEBUG_VECTORFONTSTORM_DETAILED
            break;
          case point::types::OFF_SECONDORDER:
            thiscontour.segments.back().type = segment::types::CONIC;           // this was a conic bezier curve
            #ifdef DEBUG_VECTORFONTSTORM_DETAILED
              std::cout << "VectorFontStorm: DEBUG: " << thiscontour.segments.size() << ":" << thiscontour.segments.back().points.size() - 1 << " was a conic bezier at " << prevpoint.coords << std::endl;
            #endif // DEBUG_VECTORFONTSTORM_DETAILED
            break;
          case point::types::OFF_THIRDORDER:
            thiscontour.segments.back().type = segment::types::CUBIC;           // this was a cubic bezier curve
            #ifdef DEBUG_VECTORFONTSTORM_DETAILED
              std::cout << "VectorFontStorm: DEBUG: " << thiscontour.segments.size() << ":" << thiscontour.segments.back().points.size() - 1 << " was a cubic bezier at " << prevpoint.coords << std::endl;
            #endif // DEBUG_VECTORFONTSTORM_DETAILED
            break;
          case point::types::VIRTUAL:
            #ifndef NDEBUG
              std::cout << "VectorFontStorm: ERROR: on point following an impossible virtual point at contour " << c << " point " << p << " coords " << prevpoint.coords << "!" << std::endl;
            #endif // NDEBUG
            break;
          default:
            #ifndef NDEBUG
              std::cout << "VectorFontStorm: ERROR: unknown point type " << static_cast<unsigned int>(prevpoint.type) << " at contour " << c << " point " << p << " coords " << prevpoint.coords << "!" << std::endl;
            #endif // NDEBUG
            break;
          }
          thiscontour.segments.emplace_back();                                  // this segment had already been started, so this point closed it
          thiscontour.segments.back().points.emplace_back(thispoint);           // copy this point to start the next segment
        }
        break;
      case point::types::OFF_SECONDORDER:
        if(thiscontour.segments.back().points.size() == 1) {                    // is this the first point of the segment?
          // this is the first point of the segment, so it must be the first segment, and a continuation of the final segment
          // so we start this segment with a virtual point at the midpoint between this and the last
          thiscontour.segments.back().points.emplace(thiscontour.segments.back().points.begin(), Vector2f(), point::types::VIRTUAL);  // placeholder location, we move this later
        } else {
          point &prevpoint(thiscontour.segments.back().get_second_to_last_point()); // get the second-to-last
          switch(prevpoint.type) {
          case point::types::ON:                                                // this is the first conic control point of this segment
            #ifdef DEBUG_VECTORFONTSTORM_DETAILED
              std::cout << "VectorFontStorm: DEBUG: " << thiscontour.segments.size() << ":" << thiscontour.segments.back().points.size() - 1 << " was the first conic control point" << std::endl;
            #endif // DEBUG_VECTORFONTSTORM_DETAILED
            break;
          case point::types::OFF_SECONDORDER:                                   // this is a continuing conic bezier
            {
              // split the segment with a virtual ON point at the midpoint
              Vector2f const midpoint(thispoint.coords.lerp(0.5f, prevpoint.coords));
              thiscontour.segments.back().type = segment::types::CONIC;
              thiscontour.segments.emplace_back();                              // this segment had already been started, so this point closed it
              thiscontour.segments.back().points.emplace_back(midpoint, point::types::VIRTUAL);
              thiscontour.segments.back().points.emplace_back(thispoint);       // copy this point to start the next segment
              segment &lastsegment(thiscontour.get_second_to_last_segment());   // the one before this one we just pushed
              lastsegment.points.pop_back();                                    // remove newest point from last segment (we've moved it to the next seg)
              lastsegment.points.emplace_back(midpoint, point::types::VIRTUAL); // and stitch the gap with the virtual point
              #ifdef DEBUG_VECTORFONTSTORM_DETAILED
                std::cout << "VectorFontStorm: DEBUG: " << thiscontour.segments.size() << ":" << thiscontour.segments.back().points.size() - 1 << " was a continuing cubic bezier" << std::endl;
              #endif // DEBUG_VECTORFONTSTORM_DETAILED
            }
            break;
          case point::types::OFF_THIRDORDER:                                    // conics shouldn't follow cubics!
            std::cout << "VectorFontStorm: ERROR: got an illegal point: conic following a cubic, contour " << c << " point " << p << "!" << std::endl;
            return thisglyph;
          case point::types::VIRTUAL:
            #ifndef NDEBUG
              std::cout << "VectorFontStorm: ERROR: conic point following an impossible virtual point at contour " << c << " point " << p << "!" << std::endl;
            #endif
            break;
          }
        }
        break;
      case point::types::OFF_THIRDORDER:
        /// NOTE: cubic curves are only used in Postscript CFF outlines, which
        /// are not supported in TrueType but are in OpenType.
        // TODO
        std::cout << "VectorFontStorm: WARNING: cubic third order points currently unsupported.  Ignored." << std::endl;
        break;
      default:
        std::cout << "VectorFontStorm: ERROR: invalid point type " << c << " point " << p << " type " << static_cast<int>(ptype) << "!" << std::endl;
        break;
      }
    }
    // special treatment for last segment of the curve
    point::types const ptype = outline.tags[point_end] & 0b00000001 ? point::types::ON : (outline.tags[point_end] & 0b00000010 ? point::types::OFF_THIRDORDER : point::types::OFF_SECONDORDER);
    thiscontour.segments.back().points.emplace_back(static_cast<double>(outline.points[point_end].x) * scale, static_cast<double>(outline.points[point_end].y) * scale, ptype);
    point const thispoint(thiscontour.segments.back().points.back());           // copy, do not take a reference!
    switch(thiscontour.segments.front().points.front().type) {
    case point::types::ON:                                                      // the first point was an ON point
      thiscontour.segments.back().type = segment::types::LINE;
      thiscontour.segments.emplace_back();                                      // start the final new segment
      thiscontour.segments.back().points.emplace_back(thispoint);               // copy this point to start the next segment
      thiscontour.segments.back().points.emplace_back(thiscontour.segments.front().points.front());   // and copy the first point to close the contour
      thiscontour.segments.back().type = segment::types::LINE;
      break;
    case point::types::OFF_SECONDORDER:                                         // this shouldn't ever happen unless virtual point creation fucked up earlier
      std::cout << "VectorFontStorm: ERROR: incorrect opening of contour " << c << " with a second order point - virtual point creation must have failed!" << std::endl;
      break;
    case point::types::OFF_THIRDORDER:
      // TODO - check if this situation is even allowed within the postscript / otf spec
      std::cout << "VectorFontStorm: WARNING: unsupported ending contour " << c << " with a third order cubic point!" << std::endl;
      break;
    case point::types::VIRTUAL:                                                 // the first point was a virtual point, but we didn't know where to position it until now
      {
        point &first_virtual(thiscontour.segments.front().points.front());      // get the virtual point at the start
        point &first_conic(thiscontour.segments.front().points.begin()[1]);     // get the second point in the first segment
        Vector2f const midpoint(thispoint.coords.lerp(0.5f, first_conic.coords));   // work out the midpoint location
        first_virtual.coords = midpoint;                                        // move the first virtual into correct place
        thiscontour.segments.back().points.emplace_back(first_virtual);         // copy it to close this segment
        thiscontour.segments.back().type = segment::types::CONIC;
      }
      break;
    }
    point_start = point_end + 1;
  }
  return thisglyph;
}

}
