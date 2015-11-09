#ifndef VECTORFONTSTORM_STRING_H_INCLUDED
#define VECTORFONTSTORM_STRING_H_INCLUDED

#include <string>
#include "vmath.h"
#include "buffer.h"

namespace vectorfontstorm {

class font;

class string {
public:
  enum class aligntype {
    TOPLEFT,
    TOP,
    TOPRIGHT,
    LEFT,
    RIGHT,
    CENTRE,
    BOTTOMLEFT,
    BOTTOM,
    BOTTOMRIGHT
  };
private:
  std::string contents;
  vectorfontstorm::font &thisfont;

  aligntype align = aligntype::CENTRE;

  buffer outline;                                                               // the opengl buffer for the outline of the string
  buffer fill;                                                                  // the opengl buffer for the polygonal fill of the string

  float bounds_left   = std::numeric_limits<float>::max();                      // outer-most points of the rendered model
  float bounds_bottom = std::numeric_limits<float>::max();
  float bounds_right  = std::numeric_limits<float>::min();
  float bounds_top    = std::numeric_limits<float>::min();

public:
  string(std::string const &newcontents,
         vectorfontstorm::font &newfont,
         Vector3f const &position = {0.0f, 0.0f, 0.0f},
         Quatf const &orientation = {1.0f, 0.0f, 0.0f, 0.0f},
         double scale = 1.0,
         aligntype alignment = aligntype::CENTRE);
  string(string const &other) = delete;                                         // disallow copy
  string &operator=(string const &other) = delete;
  string(string &&other) noexcept;                                              // allow move
  string &operator=(string &&other) noexcept;
  ~string();

private:
  void init(Vector3f const &position, Quatf const &orientation, double scale);

public:
  std::string const &get_contents() const __attribute__((__unused__, __const__));

  float get_bounds_left()   const __attribute__((__const__));
  float get_bounds_right()  const __attribute__((__const__));
  float get_bounds_top()    const __attribute__((__const__));
  float get_bounds_bottom() const __attribute__((__const__));
  Vector2f get_bounds_bottomleft_2d() const;
  Vector2f get_bounds_topright_2d() const;
  Vector3f get_bounds_bottomleft_3d() const;
  Vector3f get_bounds_topright_3d() const;
  Aabb2f get_bounds_2d() const;
  Aabb3f get_bounds_3d() const;

  void render_outline() const;
  void render_fill() const;
};

}

#endif // VECTORFONTSTORM_STRING_H_INCLUDED
