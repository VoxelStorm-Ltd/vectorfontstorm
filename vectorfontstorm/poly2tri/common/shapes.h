 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
 * http://code.google.com/p/poly2tri/
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of Poly2Tri nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Include guard
#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <cstddef>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

namespace p2t {

struct Edge;

struct Point {

  float x, y;

  /// Default constructor does nothing (for performance).
  Point() {
    x = 0.0;
    y = 0.0;
  }

  /// The edges this point constitutes an upper ending point
  std::vector<Edge*> edge_list;

  /// Construct using coordinates.
  Point(float new_x, float new_y) : x(new_x), y(new_y) {}

  /// Set this point to all zeros.
  void set_zero() {
    x = 0.0;
    y = 0.0;
  }

  /// Set this point to some specified coordinates.
  void set(float x_, float y_) {
    x = x_;
    y = y_;
  }

  /// Negate this point.
  Point operator -() const {
    Point v;
    v.set(-x, -y);
    return v;
  }

  /// Add a point to this point.
  void operator +=(Point const &v) {
    x += v.x;
    y += v.y;
  }

  /// Subtract a point from this point.
  void operator -=(Point const &v) {
    x -= v.x;
    y -= v.y;
  }

  /// Multiply this point by a scalar.
  void operator *=(float a) {
    x *= a;
    y *= a;
  }

  /// Get the length of this point (the norm).
  float Length() const {
    return std::sqrt(x * x + y * y);
  }

  /// Convert this point into a unit point. Returns the Length.
  float Normalize() {
    float const len = Length();
    x /= len;
    y /= len;
    return len;
  }

};

// Represents a simple polygon's edge
struct Edge {
  Point *p;
  Point *q;

  /// Constructor
  Edge(Point &p1, Point &p2)
    : p(&p1),
      q(&p2) {
    if(p1.y > p2.y) {
      q = &p1;
      p = &p2;
    } else if(p1.y == p2.y) {
      if(p1.x > p2.x) {
        q = &p1;
        p = &p2;
      } else if(p1.x == p2.x) {
        // Repeat points
        std::cout << "ERROR: repeat points " << p1.x << " " << p1.y << " and " << p2.x << " " << p2.y << std::endl;
        assert(false);
      }
    }

    q->edge_list.emplace_back(this);
  }
};

// Triangle-based data structures are known to have better performance than quad-edge structures
// See: J. Shewchuk, "Triangle: Engineering a 2D Quality Mesh Generator and Delaunay Triangulator"
//      "Triangulations in CGAL"
class Triangle {
public:
  /// Constructor
  Triangle(Point &a, Point &b, Point &c);

  /// Flags to determine if an edge is a Constrained edge
  bool constrained_edge[3];
  /// Flags to determine if an edge is a Delauney edge
  bool delaunay_edge[3];

  Point *GetPoint(int index);
  Point *PointCW(Point const &point) __attribute__((__pure__));
  Point *PointCCW(Point const &point) __attribute__((__pure__));
  Point *OppositePoint(Triangle &t, Point const &p) __attribute__((__pure__));

  Triangle *GetNeighbor(int index);
  void MarkNeighbor(Point *p1, Point *p2, Triangle *t);
  void MarkNeighbor(Triangle &t);

  void MarkConstrainedEdge(int index);
  void MarkConstrainedEdge(Edge &edge);
  void MarkConstrainedEdge(Point *p, Point *q);

  int Index(Point const *p) __attribute__((__pure__));
  int EdgeIndex(Point const *p1, Point const *p2) __attribute__((__pure__));

  Triangle *NeighborCW(Point const &point) __attribute__((__pure__));
  Triangle *NeighborCCW(Point const &point) __attribute__((__pure__));
  bool GetConstrainedEdgeCCW(Point const &p) __attribute__((__pure__));
  bool GetConstrainedEdgeCW(Point const &p) __attribute__((__pure__));
  void SetConstrainedEdgeCCW(Point const &p, bool ce);
  void SetConstrainedEdgeCW(Point const &p, bool ce);
  bool GetDelunayEdgeCCW(Point const &p) __attribute__((__pure__));
  bool GetDelunayEdgeCW(Point const &p) __attribute__((__pure__));
  void SetDelunayEdgeCCW(Point const &p, bool e);
  void SetDelunayEdgeCW(Point const &p, bool e);

  bool Contains(Point const *p);
  bool Contains(Edge const &e);
  bool Contains(Point const *p, Point const *q);
  void Legalize(Point &point);
  void Legalize(Point &opoint, Point &npoint);
  /**
   * Clears all references to all other triangles and points
   */
  void Clear();
  void ClearNeighbor(Triangle const *triangle);
  void ClearNeighbors();
  void ClearDelunayEdges();

  inline bool IsInterior();
  inline void IsInterior(bool b);

  Triangle &NeighborAcross(Point const &opoint) __attribute__((__pure__));

  void DebugPrint();

//private:
  /// Triangle points
  Point *points_[3];
  /// Neighbor list
  Triangle *neighbors_[3];

  /// Has this triangle been marked as an interior triangle?
  bool interior_;
};

inline bool cmp(std::shared_ptr<Point> const a, std::shared_ptr<Point> const b) {
  if(a->y < b->y) {
    return true;
  } else if(a->y == b->y) {
    // Make sure q is point with greater x value
    if(a->x < b->x) {
      return true;
    }
  }
  return false;
}

/// Add two points_ component-wise.
inline Point operator +(Point const &a, Point const &b) {
  return Point(a.x + b.x, a.y + b.y);
}

/// Subtract two points_ component-wise.
inline Point operator -(Point const &a, Point const &b) {
  return Point(a.x - b.x, a.y - b.y);
}

/// Multiply point by scalar
inline Point operator *(float s, Point const &a) {
  return Point(s * a.x, s * a.y);
}

inline bool operator ==(Point const &a, Point const &b) {
  return a.x == b.x && a.y == b.y;
}

inline bool operator !=(Point const &a, Point const &b) {
  return !(a.x == b.x) && !(a.y == b.y);
}

/// Peform the dot product on two vectors.
inline float Dot(Point const &a, Point const &b) {
  return a.x * b.x + a.y * b.y;
}

/// Perform the cross product on two vectors. In 2D this produces a scalar.
inline float Cross(Point const &a, Point const &b) {
  return a.x * b.y - a.y * b.x;
}

/// Perform the cross product on a point and a scalar. In 2D this produces
/// a point.
inline Point Cross(Point const &a, float s) {
  return Point(s * a.y, -s * a.x);
}

/// Perform the cross product on a scalar and a point. In 2D this produces
/// a point.
inline Point Cross(float s, Point const &a) {
  return Point(-s * a.y, s * a.x);
}

inline Point *Triangle::GetPoint(int index) {
  return points_[index];
}

inline Triangle *Triangle::GetNeighbor(int index) {
  return neighbors_[index];
}

inline bool Triangle::Contains(Point const *p) {
  return p == points_[0] || p == points_[1] || p == points_[2];
}

inline bool Triangle::Contains(Edge const &e) {
  return Contains(e.p) && Contains(e.q);
}

inline bool Triangle::Contains(Point const *p, Point const *q) {
  return Contains(p) && Contains(q);
}

inline bool Triangle::IsInterior() {
  return interior_;
}

inline void Triangle::IsInterior(bool b) {
  interior_ = b;
}

}

#endif
