  constrained_edge[2] = false;
  delaunay_edge[0] = false;
  delaunay_edge[1] = false;
  delaunay_edge[2] = false;
  interior_ = false;
}

// Update neighbor pointers
void Triangle::MarkNeighbor(Point *p1, Point *p2, Triangle *t) {
  if((p1 == points_[2] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[2])) {
    neighbors_[0] = t;
  } else if((p1 == points_[0] && p2 == points_[2]) || (p1 == points_[2] && p2 == points_[0])) {
    neighbors_[1] = t;
  } else if((p1 == points_[0] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[0])) {
    neighbors_[2] = t;
  } else {
    assert(0);
/*
  }
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
}
 * http://code.google.com/p/poly2tri/

 *
// Exhaustive search to update neighbor pointers
 * All rights reserved.
void Triangle::MarkNeighbor(Triangle &t) {
 *
  if(t.Contains(points_[1], points_[2])) {
 * Redistribution and use in source and binary forms, with or without modification,
    neighbors_[0] = &t;
 * are permitted provided that the following conditions are met:
    t.MarkNeighbor(points_[1], points_[2], this);
 *
  } else if(t.Contains(points_[0], points_[2])) {
 * * Redistributions of source code must retain the above copyright notice,
    neighbors_[1] = &t;
 *   this list of conditions and the following disclaimer.
    t.MarkNeighbor(points_[0], points_[2], this);
 * * Redistributions in binary form must reproduce the above copyright notice,
  } else if(t.Contains(points_[0], points_[1])) {
 *   this list of conditions and the following disclaimer in the documentation
    neighbors_[2] = &t;
 *   and/or other materials provided with the distribution.
    t.MarkNeighbor(points_[0], points_[1], this);
 * * Neither the name of Poly2Tri nor the names of its contributors may be
  }
 *   used to endorse or promote products derived from this software without specific
}
 *   prior written permission.

 *
/**
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * Clears all references to all other triangles and points
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 */
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
void Triangle::Clear() {
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  for(int i = 0; i < 3; i++) {
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    Triangle *t = neighbors_[i];
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    if(t) {
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
      t->ClearNeighbor(this);
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    }
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  }
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  ClearNeighbors();
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  points_[0] = nullptr;
 */
  points_[1] = nullptr;
#include "shapes.h"
  points_[2] = nullptr;
#include <iostream>
}


namespace p2t {

void Triangle::ClearNeighbor(Triangle const *triangle) {
Triangle::Triangle(Point &a, Point &b, Point &c) {
  if(neighbors_[0] == triangle) {
  points_[0] = &a;
    neighbors_[0] = nullptr;
  points_[1] = &b;
  } else if(neighbors_[1] == triangle) {
  points_[2] = &c;
    neighbors_[1] = nullptr;
  neighbors_[0] = nullptr;
  } else {
  neighbors_[1] = nullptr;
    neighbors_[2] = nullptr;
  neighbors_[2] = nullptr;
  }
  constrained_edge[0] = false;
}
  constrained_edge[1] = false;

  constrained_edge[2] = false;
void Triangle::ClearNeighbors() {
  delaunay_edge[0] = false;
  delaunay_edge[1] = false;
  neighbors_[0] = nullptr;
  delaunay_edge[2] = false;
  neighbors_[1] = nullptr;
  interior_ = false;
  neighbors_[2] = nullptr;
}
}


// Update neighbor pointers
void Triangle::ClearDelunayEdges() {
void Triangle::MarkNeighbor(Point *p1, Point *p2, Triangle *t) {
  delaunay_edge[0] = delaunay_edge[1] = delaunay_edge[2] = false;
  if((p1 == points_[2] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[2])) {
}
    neighbors_[0] = t;

  } else if((p1 == points_[0] && p2 == points_[2]) || (p1 == points_[2] && p2 == points_[0])) {
Point *Triangle::OppositePoint(Triangle &t, Point const &p) {
    neighbors_[1] = t;
  Point *cw = t.PointCW(p);
  } else if((p1 == points_[0] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[0])) {
  return PointCW(*cw);
    neighbors_[2] = t;
}
  } else {

    assert(0);
// Legalized triangle by rotating clockwise around point(0)
  }
void Triangle::Legalize(Point &point) {
}
  points_[1] = points_[0];

  points_[0] = points_[2];
// Exhaustive search to update neighbor pointers
  points_[2] = &point;
void Triangle::MarkNeighbor(Triangle &t) {
}
  if(t.Contains(points_[1], points_[2])) {

    neighbors_[0] = &t;
    t.MarkNeighbor(points_[1], points_[2], this);
// Legalize triagnle by rotating clockwise around oPoint
  } else if(t.Contains(points_[0], points_[2])) {
void Triangle::Legalize(Point &opoint, Point &npoint) {
    neighbors_[1] = &t;
  if(&opoint == points_[0]) {
    t.MarkNeighbor(points_[0], points_[2], this);
    points_[1] = points_[0];
  } else if(t.Contains(points_[0], points_[1])) {
    points_[0] = points_[2];
    neighbors_[2] = &t;
    points_[2] = &npoint;
    t.MarkNeighbor(points_[0], points_[1], this);
  } else if(&opoint == points_[1]) {
  }
    points_[2] = points_[1];
}
    points_[1] = points_[0];

/**
    points_[0] = &npoint;
 * Clears all references to all other triangles and points
  } else if(&opoint == points_[2]) {
 */
    points_[0] = points_[2];
void Triangle::Clear() {
    points_[2] = points_[1];
  for(int i = 0; i < 3; i++) {
    points_[1] = &npoint;
    Triangle *t = neighbors_[i];
  } else {
    if(t) {
    assert(0);
      t->ClearNeighbor(this);
  }
    }
}
  }

  ClearNeighbors();
int Triangle::Index(Point const *p) {
  points_[0] = nullptr;
  points_[1] = nullptr;
  if(p == points_[0]) {
  points_[2] = nullptr;
    return 0;
}
  } else if(p == points_[1]) {

    return 1;
void Triangle::ClearNeighbor(Triangle const *triangle) {
  } else if(p == points_[2]) {
  if(neighbors_[0] == triangle) {
    return 2;
    neighbors_[0] = nullptr;
  }
  } else if(neighbors_[1] == triangle) {
  assert(0);
    neighbors_[1] = nullptr;
  return -1;
  } else {
}
    neighbors_[2] = nullptr;

  }
}
int Triangle::EdgeIndex(Point const *p1, Point const *p2) {

  if(points_[0] == p1) {
void Triangle::ClearNeighbors() {
    if(points_[1] == p2) {
  neighbors_[0] = nullptr;
      return 2;
  neighbors_[1] = nullptr;
    } else if(points_[2] == p2) {
  neighbors_[2] = nullptr;
      return 1;
}
    }

  } else if(points_[1] == p1) {
void Triangle::ClearDelunayEdges() {
    if(points_[2] == p2) {
  delaunay_edge[0] = delaunay_edge[1] = delaunay_edge[2] = false;
      return 0;
}

    } else if(points_[0] == p2) {
Point *Triangle::OppositePoint(Triangle &t, Point const &p) {
      return 2;
  Point *cw = t.PointCW(p);
    }
  return PointCW(*cw);
  } else if(points_[2] == p1) {
}
    if(points_[0] == p2) {

      return 1;
// Legalized triangle by rotating clockwise around point(0)
    } else if(points_[1] == p2) {
void Triangle::Legalize(Point &point) {
      return 0;
  points_[1] = points_[0];
    }
  points_[0] = points_[2];
  }
  points_[2] = &point;
}
  return -1;

}
// Legalize triagnle by rotating clockwise around oPoint

void Triangle::Legalize(Point &opoint, Point &npoint) {
void Triangle::MarkConstrainedEdge(int index) {
  if(&opoint == points_[0]) {
  constrained_edge[index] = true;
    points_[1] = points_[0];
}
    points_[0] = points_[2];

    points_[2] = &npoint;
void Triangle::MarkConstrainedEdge(Edge &edge) {
  } else if(&opoint == points_[1]) {
  MarkConstrainedEdge(edge.p, edge.q);
    points_[2] = points_[1];
}
    points_[1] = points_[0];

    points_[0] = &npoint;
// Mark edge as constrained
  } else if(&opoint == points_[2]) {
    points_[0] = points_[2];
void Triangle::MarkConstrainedEdge(Point *p, Point *q) {
    points_[2] = points_[1];
  if((q == points_[0] && p == points_[1]) || (q == points_[1] && p == points_[0])) {
    points_[1] = &npoint;
    constrained_edge[2] = true;
  } else {
  } else if((q == points_[0] && p == points_[2]) || (q == points_[2] && p == points_[0])) {
    assert(0);
    constrained_edge[1] = true;
  }
}
  } else if((q == points_[1] && p == points_[2]) || (q == points_[2] && p == points_[1])) {

    constrained_edge[0] = true;
int Triangle::Index(Point const *p) {
  }
  if(p == points_[0]) {
}
    return 0;

  } else if(p == points_[1]) {
// The point counter-clockwise to given point
    return 1;
Point *Triangle::PointCW(Point const &point) {
  } else if(p == points_[2]) {
  if(&point == points_[0]) {
    return 2;
    return points_[2];
  }
  } else if(&point == points_[1]) {
  assert(0);
    return points_[0];
  return -1;
}
  } else if(&point == points_[2]) {

    return points_[1];
int Triangle::EdgeIndex(Point const *p1, Point const *p2) {
  }
  if(points_[0] == p1) {
  assert(0);
    if(points_[1] == p2) {
  return nullptr;
      return 2;
}
    } else if(points_[2] == p2) {

      return 1;
// The point counter-clockwise to given point
    }
Point *Triangle::PointCCW(Point const &point) {
  } else if(points_[1] == p1) {
  if(&point == points_[0]) {
    if(points_[2] == p2) {
    return points_[1];
      return 0;
  } else if(&point == points_[1]) {
    } else if(points_[0] == p2) {
      return 2;
    return points_[2];
    }
  } else if(&point == points_[2]) {
  } else if(points_[2] == p1) {
    return points_[0];
    if(points_[0] == p2) {
  }
      return 1;
  assert(0);
    } else if(points_[1] == p2) {
  return nullptr;
      return 0;
}
    }

  }
// The neighbor clockwise to given point
  return -1;
Triangle *Triangle::NeighborCW(Point const &point) {
}
  if(&point == points_[0]) {

    return neighbors_[1];
void Triangle::MarkConstrainedEdge(int index) {
  } else if(&point == points_[1]) {
  constrained_edge[index] = true;
    return neighbors_[2];
}

  }
void Triangle::MarkConstrainedEdge(Edge &edge) {
  return neighbors_[0];
  MarkConstrainedEdge(edge.p, edge.q);
}
}


// The neighbor counter-clockwise to given point
// Mark edge as constrained
Triangle *Triangle::NeighborCCW(Point const &point) {
void Triangle::MarkConstrainedEdge(Point *p, Point *q) {
  if(&point == points_[0]) {
  if((q == points_[0] && p == points_[1]) || (q == points_[1] && p == points_[0])) {
    return neighbors_[2];
    constrained_edge[2] = true;
  } else if(&point == points_[1]) {
  } else if((q == points_[0] && p == points_[2]) || (q == points_[2] && p == points_[0])) {
    return neighbors_[0];
    constrained_edge[1] = true;
  }
  } else if((q == points_[1] && p == points_[2]) || (q == points_[2] && p == points_[1])) {
  return neighbors_[1];
    constrained_edge[0] = true;
}
  }

}

bool Triangle::GetConstrainedEdgeCCW(Point const &p) {
// The point counter-clockwise to given point
  if(&p == points_[0]) {
Point *Triangle::PointCW(Point const &point) {
    return constrained_edge[2];
  if(&point == points_[0]) {
  } else if(&p == points_[1]) {
    return points_[2];
    return constrained_edge[0];
  } else if(&point == points_[1]) {
  }
    return points_[0];
  return constrained_edge[1];
  } else if(&point == points_[2]) {
}
    return points_[1];

  }
bool Triangle::GetConstrainedEdgeCW(Point const &p) {
  assert(0);
  if(&p == points_[0]) {
  return nullptr;
}
    return constrained_edge[1];

  } else if(&p == points_[1]) {
// The point counter-clockwise to given point
    return constrained_edge[2];
Point *Triangle::PointCCW(Point const &point) {
  }
  if(&point == points_[0]) {
  return constrained_edge[0];
    return points_[1];
}
  } else if(&point == points_[1]) {

    return points_[2];
void Triangle::SetConstrainedEdgeCCW(Point const &p, bool ce) {
  } else if(&point == points_[2]) {
  if(&p == points_[0]) {
    return points_[0];
    constrained_edge[2] = ce;
  }
  } else if(&p == points_[1]) {
  assert(0);
    constrained_edge[0] = ce;
  return nullptr;
}
  } else {

    constrained_edge[1] = ce;
// The neighbor clockwise to given point
  }
Triangle *Triangle::NeighborCW(Point const &point) {
}
  if(&point == points_[0]) {

    return neighbors_[1];
void Triangle::SetConstrainedEdgeCW(Point const &p, bool ce) {
  } else if(&point == points_[1]) {
  if(&p == points_[0]) {
    return neighbors_[2];
    constrained_edge[1] = ce;
  }
  } else if(&p == points_[1]) {
  return neighbors_[0];
    constrained_edge[2] = ce;
}
  } else {

    constrained_edge[0] = ce;
// The neighbor counter-clockwise to given point
  }
Triangle *Triangle::NeighborCCW(Point const &point) {
}
  if(&point == points_[0]) {

    return neighbors_[2];
bool Triangle::GetDelunayEdgeCCW(Point const &p) {
  } else if(&point == points_[1]) {
    return neighbors_[0];
  if(&p == points_[0]) {
  }
    return delaunay_edge[2];
  return neighbors_[1];
  } else if(&p == points_[1]) {
}
    return delaunay_edge[0];

  }
bool Triangle::GetConstrainedEdgeCCW(Point const &p) {
  return delaunay_edge[1];
  if(&p == points_[0]) {
}
    return constrained_edge[2];

  } else if(&p == points_[1]) {
bool Triangle::GetDelunayEdgeCW(Point const &p) {
    return constrained_edge[0];
  if(&p == points_[0]) {
  }
    return delaunay_edge[1];
  return constrained_edge[1];
}
  } else if(&p == points_[1]) {

    return delaunay_edge[2];
bool Triangle::GetConstrainedEdgeCW(Point const &p) {
  }
  if(&p == points_[0]) {
    return constrained_edge[1];
  return delaunay_edge[0];
  } else if(&p == points_[1]) {
}
    return constrained_edge[2];

  }
void Triangle::SetDelunayEdgeCCW(Point const &p, bool e) {
  return constrained_edge[0];
  if(&p == points_[0]) {
}
    delaunay_edge[2] = e;

  } else if(&p == points_[1]) {
void Triangle::SetConstrainedEdgeCCW(Point const &p, bool ce) {
    delaunay_edge[0] = e;
  if(&p == points_[0]) {
  } else {
    constrained_edge[2] = ce;
    delaunay_edge[1] = e;
  } else if(&p == points_[1]) {
  }
    constrained_edge[0] = ce;
}
  } else {

    constrained_edge[1] = ce;
  }
void Triangle::SetDelunayEdgeCW(Point const &p, bool e) {
}
  if(&p == points_[0]) {

    delaunay_edge[1] = e;
void Triangle::SetConstrainedEdgeCW(Point const &p, bool ce) {
  } else if(&p == points_[1]) {
  if(&p == points_[0]) {
    delaunay_edge[2] = e;
    constrained_edge[1] = ce;
  } else {
  } else if(&p == points_[1]) {
    delaunay_edge[0] = e;
    constrained_edge[2] = ce;
  }
  } else {
}
    constrained_edge[0] = ce;

  }
// The neighbor across to given point
}

Triangle &Triangle::NeighborAcross(Point const &opoint) {
bool Triangle::GetDelunayEdgeCCW(Point const &p) {
  if(&opoint == points_[0]) {
  if(&p == points_[0]) {
    return *neighbors_[0];
    return delaunay_edge[2];
  } else if(&opoint == points_[1]) {
  } else if(&p == points_[1]) {
    return *neighbors_[1];
    return delaunay_edge[0];
  }
  }
  return *neighbors_[2];
  return delaunay_edge[1];
}
}


void Triangle::DebugPrint() {
bool Triangle::GetDelunayEdgeCW(Point const &p) {
  std::cout << points_[0]->x << "," << points_[0]->y << " ";
  if(&p == points_[0]) {
    return delaunay_edge[1];
  std::cout << points_[1]->x << "," << points_[1]->y << " ";
  } else if(&p == points_[1]) {
  std::cout << points_[2]->x << "," << points_[2]->y << std::endl;
    return delaunay_edge[2];
}
  }

  return delaunay_edge[0];
}
}

void Triangle::SetDelunayEdgeCCW(Point const &p, bool e) {
  if(&p == points_[0]) {
    delaunay_edge[2] = e;
  } else if(&p == points_[1]) {
    delaunay_edge[0] = e;
  } else {
    delaunay_edge[1] = e;
  }
}

void Triangle::SetDelunayEdgeCW(Point const &p, bool e) {
  if(&p == points_[0]) {
    delaunay_edge[1] = e;
  } else if(&p == points_[1]) {
    delaunay_edge[2] = e;
  } else {
    delaunay_edge[0] = e;
  }
}

// The neighbor across to given point
Triangle &Triangle::NeighborAcross(Point const &opoint) {
  if(&opoint == points_[0]) {
    return *neighbors_[0];
  } else if(&opoint == points_[1]) {
    return *neighbors_[1];
  }
  return *neighbors_[2];
}

void Triangle::DebugPrint() {
  std::cout << points_[0]->x << "," << points_[0]->y << " ";
  std::cout << points_[1]->x << "," << points_[1]->y << " ";
  std::cout << points_[2]->x << "," << points_[2]->y << std::endl;
}

}
