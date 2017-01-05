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
/*
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * http://code.google.com/p/poly2tri/
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * All rights reserved.
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * Redistribution and use in source and binary forms, with or without modification,
 */
 * are permitted provided that the following conditions are met:
/**
 *
 * Sweep-line, Constrained Delauney Triangulation (CDT) See: Domiter, V. and
 * * Redistributions of source code must retain the above copyright notice,
 * Zalik, B.(2008)'Sweep-line algorithm for constrained Delaunay triangulation',
 *   this list of conditions and the following disclaimer.
 * International Journal of Geographical Information Science
 * * Redistributions in binary form must reproduce the above copyright notice,
 *
 *   this list of conditions and the following disclaimer in the documentation
 * "FlipScan" Constrained Edge Algorithm invented by Thomas ?hl?n, thahlen@gmail.com
 *   and/or other materials provided with the distribution.
 */
 * * Neither the name of Poly2Tri nor the names of its contributors may be

 *   used to endorse or promote products derived from this software without specific
#ifndef SWEEP_H
 *   prior written permission.
#define SWEEP_H
 *

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#include <vector>
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT

 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
namespace p2t {
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR

 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
class SweepContext;
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
struct Node;
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
struct Point;
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
struct Edge;
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
class Triangle;
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS

 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
class Sweep {
 */
/**
public:
 * Sweep-line, Constrained Delauney Triangulation (CDT) See: Domiter, V. and

 * Zalik, B.(2008)'Sweep-line algorithm for constrained Delaunay triangulation',
  /**
 * International Journal of Geographical Information Science
   * Triangulate
 *
   *
 * "FlipScan" Constrained Edge Algorithm invented by Thomas ?hl?n, thahlen@gmail.com
   * @param tcx
 */
   */

  void Triangulate(SweepContext &tcx);
#ifndef SWEEP_H

#define SWEEP_H
  /**

   * Destructor - clean up memory
#include <vector>
   */

  ~Sweep();
namespace p2t {


class SweepContext;
private:
struct Node;

struct Point;
  /**
struct Edge;
   * Start sweeping the Y-sorted point set from bottom to top
class Triangle;
   *

   * @param tcx
class Sweep {
   */
public:
  void SweepPoints(SweepContext &tcx);


  /**
  /**
   * Triangulate
   * Find closes node to the left of the new point and
   *
   * create a new triangle. If needed new holes and basins
   * @param tcx
   * will be filled to.
   */
   *
  void Triangulate(SweepContext &tcx);

   * @param tcx
  /**
   * @param point
   * Destructor - clean up memory
   * @return
   */
   */
  ~Sweep();
  Node &PointEvent(SweepContext &tcx, Point &point);


private:
  /**

    *
  /**
    *
   * Start sweeping the Y-sorted point set from bottom to top
    * @param tcx
   *
    * @param edge
   * @param tcx
    * @param node
   */
    */
  void SweepPoints(SweepContext &tcx);

  void EdgeEvent(SweepContext &tcx, Edge *edge, Node *node);
  /**

   * Find closes node to the left of the new point and
  void EdgeEvent(SweepContext &tcx, Point &ep, Point &eq, Triangle *triangle, Point &point);
   * create a new triangle. If needed new holes and basins

   * will be filled to.
  /**
   *
   * Creates a new front triangle and legalize it
   * @param tcx
   *
   * @param point
   * @param tcx
   * @return
   * @param point
   */
   * @param node
  Node &PointEvent(SweepContext &tcx, Point &point);
   * @return

   */
  /**
    *
  Node &NewFrontTriangle(SweepContext &tcx, Point &point, Node &node);
    *

    * @param tcx
  /**
    * @param edge
   * Adds a triangle to the advancing front to fill a hole.
    * @param node
   * @param tcx
    */
   * @param node - middle node, that is the bottom of the hole
  void EdgeEvent(SweepContext &tcx, Edge *edge, Node *node);
   */

  void Fill(SweepContext &tcx, Node &node);
  void EdgeEvent(SweepContext &tcx, Point &ep, Point &eq, Triangle *triangle, Point &point);


  /**
  /**
   * Returns true if triangle was legalized
   * Creates a new front triangle and legalize it
   */
   *
  bool Legalize(SweepContext &tcx, Triangle &t);
   * @param tcx

   * @param point
  /**
   * @param node
   * <b>Requirement</b>:<br>
   * @return
   */
   * 1. a,b and c form a triangle.<br>
  Node &NewFrontTriangle(SweepContext &tcx, Point &point, Node &node);
   * 2. a and d is know to be on opposite side of bc<br>

   * <pre>
  /**
   *                a
   * Adds a triangle to the advancing front to fill a hole.
   *                +
   * @param tcx
   *               / \
   * @param node - middle node, that is the bottom of the hole
   *              /   \
   */
   *            b/     \c
  void Fill(SweepContext &tcx, Node &node);
   *            +-------+

   *           /    d    \
  /**
   *          /           \
   * Returns true if triangle was legalized
   */
   * </pre>
  bool Legalize(SweepContext &tcx, Triangle &t);
   * <b>Fact</b>: d has to be in area B to have a chance to be inside the circle formed by

   *  a,b and c<br>
  /**
   *  d is outside B if orient2d(a,b,d) or orient2d(c,a,d) is CW<br>
   * <b>Requirement</b>:<br>
   *  This preknowledge gives us a way to optimize the incircle test
   * 1. a,b and c form a triangle.<br>
   * @param a - triangle point, opposite d
   * 2. a and d is know to be on opposite side of bc<br>
   * @param b - triangle point
   * <pre>
   * @param c - triangle point
   *                a
   * @param d - point opposite a
   *                +
   *               / \
   * @return true if d is inside circle, false if on circle edge
   *              /   \
   */
   *            b/     \c
  bool Incircle(Point const &pa, Point const &pb, Point const &pc, Point const &pd) const __attribute__((__pure__));
   *            +-------+

   *           /    d    \
  /**
   *          /           \
   * Rotates a triangle pair one vertex CW
   * </pre>
   *<pre>
   * <b>Fact</b>: d has to be in area B to have a chance to be inside the circle formed by
   *       n2                    n2
   *  a,b and c<br>
   *  P +-----+             P +-----+
   *  d is outside B if orient2d(a,b,d) or orient2d(c,a,d) is CW<br>
   *    | t  /|               |\  t |
   *  This preknowledge gives us a way to optimize the incircle test
   * @param a - triangle point, opposite d
   *    |   / |               | \   |
   * @param b - triangle point
   *  n1|  /  |n3           n1|  \  |n3
   * @param c - triangle point
   *    | /   |    after CW   |   \ |
   * @param d - point opposite a
   *    |/ oT |               | oT \|
   * @return true if d is inside circle, false if on circle edge
   *    +-----+ oP            +-----+
   */
   *       n4                    n4
  bool Incircle(Point const &pa, Point const &pb, Point const &pc, Point const &pd) const __attribute__((__pure__));
   * </pre>

   */
  /**
  #ifdef __MINGW32__
   * Rotates a triangle pair one vertex CW
    // this is a workaround for some incomprehensible windows bug or other
   *<pre>
    void RotateTrianglePair(Triangle &t, Point &p, Triangle &ot, Point &op) const __attribute__((__optimize__("O1")));
   *       n2                    n2
  #else
   *  P +-----+             P +-----+
   *    | t  /|               |\  t |
    void RotateTrianglePair(Triangle &t, Point &p, Triangle &ot, Point &op) const;
   *    |   / |               | \   |
  #endif // __MINGW32__
   *  n1|  /  |n3           n1|  \  |n3

   *    | /   |    after CW   |   \ |
  /**
   *    |/ oT |               | oT \|
   * Fills holes in the Advancing Front
   *    +-----+ oP            +-----+
   *
   *       n4                    n4
   *
   * </pre>
   * @param tcx
   */
   * @param n
  #ifdef __MINGW32__
   */
    // this is a workaround for some incomprehensible windows bug or other
  void FillAdvancingFront(SweepContext &tcx, Node &n);
    void RotateTrianglePair(Triangle &t, Point &p, Triangle &ot, Point &op) const __attribute__((__optimize__("O1")));

  #else
  // Decision-making about when to Fill hole.
    void RotateTrianglePair(Triangle &t, Point &p, Triangle &ot, Point &op) const;
  // Contributed by ToolmakerSteve2
  #endif // __MINGW32__

  bool LargeHole_DontFill(Node const *node) const __attribute__((__pure__));
  /**
  bool AngleExceeds90Degrees(Point const *origin, Point const *pa, Point const *pb) const __attribute__((__pure__));
   * Fills holes in the Advancing Front
  bool AngleExceedsPlus90DegreesOrIsNegative(Point const *origin, Point const *pa, Point const *pb) const __attribute__((__pure__));
   *
  float Angle(Point const *origin, Point const *pa, Point const *pb) const __attribute__((__pure__));
   *

   * @param tcx
  /**
   * @param n
   *
   */
   * @param node - middle node
  void FillAdvancingFront(SweepContext &tcx, Node &n);

   * @return the angle between 3 front nodes
  // Decision-making about when to Fill hole.
   */
  // Contributed by ToolmakerSteve2
  float HoleAngle(Node const &node) const __attribute__((__pure__));
  bool LargeHole_DontFill(Node const *node) const __attribute__((__pure__));

  bool AngleExceeds90Degrees(Point const *origin, Point const *pa, Point const *pb) const __attribute__((__pure__));
  /**
  bool AngleExceedsPlus90DegreesOrIsNegative(Point const *origin, Point const *pa, Point const *pb) const __attribute__((__pure__));
   * The basin angle is decided against the horizontal line [1,0]
  float Angle(Point const *origin, Point const *pa, Point const *pb) const __attribute__((__pure__));
   */

  float BasinAngle(Node const &node) const __attribute__((__pure__));
  /**

   *
  /**
   * @param node - middle node
   * Fills a basin that has formed on the Advancing Front to the right
   * @return the angle between 3 front nodes
   * of given node.<br>
   */
   * First we decide a left,bottom and right node that forms the
  float HoleAngle(Node const &node) const __attribute__((__pure__));
   * boundaries of the basin. Then we do a reqursive fill.

   *
  /**
   * @param tcx
   * The basin angle is decided against the horizontal line [1,0]
   */
   * @param node - starting node, this or next node will be left node
  float BasinAngle(Node const &node) const __attribute__((__pure__));
   */

  void FillBasin(SweepContext &tcx, Node &node);
  /**

   * Fills a basin that has formed on the Advancing Front to the right
  /**
   * of given node.<br>
   * Recursive algorithm to fill a Basin with triangles
   * First we decide a left,bottom and right node that forms the
   *
   * boundaries of the basin. Then we do a reqursive fill.
   * @param tcx
   *
   * @param node - bottom_node
   * @param tcx
   * @param cnt - counter used to alternate on even and odd numbers
   * @param node - starting node, this or next node will be left node
   */
   */
  void FillBasinReq(SweepContext &tcx, Node *node);
  void FillBasin(SweepContext &tcx, Node &node);


  bool IsShallow(SweepContext &tcx, Node &node) __attribute__((__pure__));
  /**

   * Recursive algorithm to fill a Basin with triangles
  bool IsEdgeSideOfTriangle(Triangle &triangle, Point &ep, Point &eq);
   *

   * @param tcx
   * @param node - bottom_node
  void FillEdgeEvent(SweepContext &tcx, Edge *edge, Node *node);
   * @param cnt - counter used to alternate on even and odd numbers

   */
  void FillRightAboveEdgeEvent(SweepContext &tcx, Edge *edge, Node *node);
  void FillBasinReq(SweepContext &tcx, Node *node);


  void FillRightBelowEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
  bool IsShallow(SweepContext &tcx, Node &node) __attribute__((__pure__));


  void FillRightConcaveEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
  bool IsEdgeSideOfTriangle(Triangle &triangle, Point &ep, Point &eq);


  void FillRightConvexEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
  void FillEdgeEvent(SweepContext &tcx, Edge *edge, Node *node);


  void FillLeftAboveEdgeEvent(SweepContext &tcx, Edge *edge, Node *node);
  void FillRightAboveEdgeEvent(SweepContext &tcx, Edge *edge, Node *node);


  void FillLeftBelowEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
  void FillRightBelowEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);


  void FillLeftConcaveEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
  void FillRightConcaveEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);


  void FillLeftConvexEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
  void FillRightConvexEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);


  void FlipEdgeEvent(SweepContext &tcx, Point &ep, Point &eq, Triangle *t, Point &p);
  void FillLeftAboveEdgeEvent(SweepContext &tcx, Edge *edge, Node *node);


  /**
  void FillLeftBelowEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);

   * After a flip we have two triangles and know that only one will still be
  void FillLeftConcaveEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
   * intersecting the edge. So decide which to contiune with and legalize the other

   *
  void FillLeftConvexEdgeEvent(SweepContext &tcx, Edge *edge, Node &node);
   * @param tcx

   * @param o - should be the result of an orient2d(eq, op, ep)
  void FlipEdgeEvent(SweepContext &tcx, Point &ep, Point &eq, Triangle *t, Point &p);
   * @param t - triangle 1

   * @param ot - triangle 2
  /**
   * @param p - a point shared by both triangles
   * After a flip we have two triangles and know that only one will still be
   * @param op - another point shared by both triangles
   * intersecting the edge. So decide which to contiune with and legalize the other
   * @return returns the triangle still intersecting the edge
   *
   */
   * @param tcx
  Triangle &NextFlipTriangle(SweepContext &tcx, int o, Triangle  &t, Triangle &ot, Point &p, Point &op);
   * @param o - should be the result of an orient2d(eq, op, ep)

   * @param t - triangle 1
  /**
   * @param ot - triangle 2
    * When we need to traverse from one triangle to the next we need
   * @param p - a point shared by both triangles
    * the point in current triangle that is the opposite point to the next
   * @param op - another point shared by both triangles
    * triangle.
   * @return returns the triangle still intersecting the edge
    *
   */
    * @param ep
  Triangle &NextFlipTriangle(SweepContext &tcx, int o, Triangle  &t, Triangle &ot, Point &p, Point &op);

    * @param eq
  /**
    * @param ot
    * When we need to traverse from one triangle to the next we need
    * @param op
    * the point in current triangle that is the opposite point to the next
    * @return
    * triangle.
    */
    *
  Point &NextFlipPoint(Point &ep, Point &eq, Triangle &ot, Point &op);
    * @param ep

    * @param eq
  /**
    * @param ot
    * Scan part of the FlipScan algorithm<br>
    * @param op
    * When a triangle pair isn't flippable we will scan for the next
    * @return
    * point that is inside the flip triangle scan area. When found
    */
    * we generate a new flipEdgeEvent
  Point &NextFlipPoint(Point &ep, Point &eq, Triangle &ot, Point &op);
    *

    * @param tcx
  /**
    * @param ep - last point on the edge we are traversing
    * Scan part of the FlipScan algorithm<br>
    * @param eq - first point on the edge we are traversing
    * When a triangle pair isn't flippable we will scan for the next
    * @param flipTriangle - the current triangle sharing the point eq with edge
    * point that is inside the flip triangle scan area. When found
    * @param t
    * we generate a new flipEdgeEvent
    *
    * @param p
    * @param tcx
    */
    * @param ep - last point on the edge we are traversing
  void FlipScanEdgeEvent(SweepContext &tcx, Point &ep, Point &eq, Triangle &flip_triangle, Triangle &t, Point &p);
    * @param eq - first point on the edge we are traversing

    * @param flipTriangle - the current triangle sharing the point eq with edge
  void FinalizationPolygon(SweepContext &tcx);
    * @param t

    * @param p
  std::vector<Node*> nodes_;
    */
};
  void FlipScanEdgeEvent(SweepContext &tcx, Point &ep, Point &eq, Triangle &flip_triangle, Triangle &t, Point &p);


}
  void FinalizationPolygon(SweepContext &tcx);


#endif
  std::vector<Node*> nodes_;
};

}

#endif
