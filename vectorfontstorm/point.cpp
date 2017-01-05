#include "point.h"

namespace vectorfontstorm {

point::point(float x, float y, types newtype)
  : type(newtype),
    coords(x, y) {
  /// Component constructor
}

point::point(vector2f const &newcoords, types newtype)
  : type(newtype),
    coords(newcoords) {
  /// Vector constructor
}

point::~point() {
  /// Default destructor
}

}
