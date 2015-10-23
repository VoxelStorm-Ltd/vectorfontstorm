#ifndef VECTORFONTSTORM_POINT_H_INCLUDED
#define VECTORFONTSTORM_POINT_H_INCLUDED

#include "vmath.h"

namespace vectorfontstorm {

class point {
  friend class segment;
  friend class font;

  enum class types {                                                            // what type of point this is - see http://www.freetype.org/freetype2/docs/glyphs/glyphs-6.html
    ON,                                                                         // on the curve
    OFF_SECONDORDER,                                                            // off the curve, second order bezier control point
    OFF_THIRDORDER,                                                             // off the curve, third order bezier control point
    VIRTUAL                                                                     // virtual on point joining two successive conic arcs
  };
  types type;
  Vector2f coords;

public:
  point(float x, float y, types newtype);
  point(Vector2f const &newcoords, types newtype);
  ~point() __attribute__((__const__));
};

}

#endif // VECTORFONTSTORM_POINT_H_INCLUDED
