#ifndef VECTORFONTSTORM_POINT_H_INCLUDED
#define VECTORFONTSTORM_POINT_H_INCLUDED

#include "vectorstorm/vector/vector2.h"

namespace vectorfontstorm {

class point {
  template<typename T> friend class segment;
  template<typename T> friend class font;

  enum class types {                                                            // what type of point this is - see http://www.freetype.org/freetype2/docs/glyphs/glyphs-6.html
    ON,                                                                         // on the curve
    OFF_SECONDORDER,                                                            // off the curve, second order bezier control point
    OFF_THIRDORDER,                                                             // off the curve, third order bezier control point
    VIRTUAL                                                                     // virtual on point joining two successive conic arcs
  } type = types::ON;
  vector2f coords;

public:
  point(float x, float y, types newtype);
  point(vector2f const &newcoords, types newtype);
  ~point();
};

}

#endif // VECTORFONTSTORM_POINT_H_INCLUDED
