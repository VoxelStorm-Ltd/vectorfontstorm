#ifndef UTILS_H
#define UTILS_H

// Otherwise #defines like M_PI are undeclared under Visual Studio
#define _USE_MATH_DEFINES

#include <exception>
/*
#include <cmath>
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
#include <limits>
 * http://code.google.com/p/poly2tri/

 *
// C99 removes M_PI from math.h
 * All rights reserved.
#ifndef M_PI
 *
#define M_PI 3.14159265358979323846264338327
 * Redistribution and use in source and binary forms, with or without modification,
#endif
 * are permitted provided that the following conditions are met:

 *
namespace p2t {
 * * Redistributions of source code must retain the above copyright notice,

 *   this list of conditions and the following disclaimer.
float constexpr const PI_3div4 = 3.0f * static_cast<float>(M_PI) / 4.0f;
 * * Redistributions in binary form must reproduce the above copyright notice,
float constexpr const PI_div2 = 1.57079632679489661923f;
 *   this list of conditions and the following disclaimer in the documentation

 *   and/or other materials provided with the distribution.
enum Orientation {CW, CCW, COLLINEAR};
 * * Neither the name of Poly2Tri nor the names of its contributors may be

 *   used to endorse or promote products derived from this software without specific
Orientation Orient2d(Point const &pa, Point const &pb, Point const &pc) __attribute__((__pure__));
 *   prior written permission.
 *
bool InScanArea(Point const &pa, Point const &pb, Point const &pc, Point const &pd) __attribute__((__pure__));
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS

 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/**
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * Forumla to calculate signed area<br>
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * Positive if CCW<br>
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * Negative if CW<br>
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * 0 if collinear<br>
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * <pre>
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * A[P1,P2,P3]  =  (x1*y2 - y1*x2) + (x2*y3 - y2*x3) + (x3*y1 - y3*x1)
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *              =  (x1-x3)*(y2-y3) - (y1-y3)*(x2-x3)
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * </pre>
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 */
Orientation Orient2d(Point const &pa, Point const &pb, Point const &pc) {

  float detleft = (pa.x - pc.x) * (pb.y - pc.y);
#ifndef UTILS_H
  float detright = (pa.y - pc.y) * (pb.x - pc.x);
#define UTILS_H
  float val = detleft - detright;

  if(val > -std::numeric_limits<float>::epsilon() && val < std::numeric_limits<float>::epsilon()) {
// Otherwise #defines like M_PI are undeclared under Visual Studio
    return COLLINEAR;
#define _USE_MATH_DEFINES

  } else if(val > 0) {
#include <exception>
    return CCW;
#include <cmath>
  }
#include <limits>
  return CW;

}
// C99 removes M_PI from math.h

#ifndef M_PI
/*
#define M_PI 3.14159265358979323846264338327
bool InScanArea(Point &pa, Point &pb, Point &pc, Point &pd) {
#endif
  float pdx = pd.x;

  float pdy = pd.y;
namespace p2t {
  float adx = pa.x - pdx;

  float ady = pa.y - pdy;
float constexpr const PI_3div4 = 3.0f * static_cast<float>(M_PI) / 4.0f;
  float bdx = pb.x - pdx;
float constexpr const PI_div2 = 1.57079632679489661923f;

  float bdy = pb.y - pdy;
enum Orientation {CW, CCW, COLLINEAR};


  float adxbdy = adx * bdy;
Orientation Orient2d(Point const &pa, Point const &pb, Point const &pc) __attribute__((__pure__));
  float bdxady = bdx * ady;
bool InScanArea(Point const &pa, Point const &pb, Point const &pc, Point const &pd) __attribute__((__pure__));
  float oabd = adxbdy - bdxady;


/**
  if(oabd <= std::numeric_limits<float>::epsilon()) {
 * Forumla to calculate signed area<br>
    return false;
 * Positive if CCW<br>
  }
 * Negative if CW<br>

 * 0 if collinear<br>
  float cdx = pc.x - pdx;
 * <pre>
  float cdy = pc.y - pdy;
 * A[P1,P2,P3]  =  (x1*y2 - y1*x2) + (x2*y3 - y2*x3) + (x3*y1 - y3*x1)

 *              =  (x1-x3)*(y2-y3) - (y1-y3)*(x2-x3)
  float cdxady = cdx * ady;
 * </pre>
  float adxcdy = adx * cdy;
 */
  float ocad = cdxady - adxcdy;
Orientation Orient2d(Point const &pa, Point const &pb, Point const &pc) {

  float detleft = (pa.x - pc.x) * (pb.y - pc.y);
  if(ocad <= EPSILON) {
  float detright = (pa.y - pc.y) * (pb.x - pc.x);
    return false;
  float val = detleft - detright;
  }
  if(val > -std::numeric_limits<float>::epsilon() && val < std::numeric_limits<float>::epsilon()) {

    return COLLINEAR;
  return true;
  } else if(val > 0) {
    return CCW;
}
  }
*/
  return CW;

}
bool InScanArea(Point const &pa, Point const &pb, Point const &pc, Point const &pd) {

  float oadb = (pa.x - pb.x) * (pd.y - pb.y) - (pd.x - pb.x) * (pa.y - pb.y);
/*
  if(oadb >= -std::numeric_limits<float>::epsilon()) {
bool InScanArea(Point &pa, Point &pb, Point &pc, Point &pd) {
    return false;
  float pdx = pd.x;
  }
  float pdy = pd.y;

  float adx = pa.x - pdx;
  float ady = pa.y - pdy;
  float oadc = (pa.x - pc.x) * (pd.y - pc.y) - (pd.x - pc.x) * (pa.y - pc.y);
  float bdx = pb.x - pdx;
  if(oadc <= std::numeric_limits<float>::epsilon()) {
  float bdy = pb.y - pdy;
    return false;

  }
  float adxbdy = adx * bdy;
  return true;
  float bdxady = bdx * ady;
}
  float oabd = adxbdy - bdxady;


}
  if(oabd <= std::numeric_limits<float>::epsilon()) {

    return false;
#endif
  }

  float cdx = pc.x - pdx;
  float cdy = pc.y - pdy;

  float cdxady = cdx * ady;
  float adxcdy = adx * cdy;
  float ocad = cdxady - adxcdy;

  if(ocad <= EPSILON) {
    return false;
  }

  return true;
}
*/

bool InScanArea(Point const &pa, Point const &pb, Point const &pc, Point const &pd) {
  float oadb = (pa.x - pb.x) * (pd.y - pb.y) - (pd.x - pb.x) * (pa.y - pb.y);
  if(oadb >= -std::numeric_limits<float>::epsilon()) {
    return false;
  }

  float oadc = (pa.x - pc.x) * (pd.y - pc.y) - (pd.x - pc.x) * (pa.y - pc.y);
  if(oadc <= std::numeric_limits<float>::epsilon()) {
    return false;
  }
  return true;
}

}

#endif
