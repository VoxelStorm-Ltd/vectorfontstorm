  af_middle_ = new Node(*triangle->GetPoint(0), *triangle);
  af_tail_ = new Node(*triangle->GetPoint(2));
  front_ = new AdvancingFront(*af_head_, *af_tail_);

/*
  // TODO: More intuitive if head is middles next and not previous?
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
  //       so swap head and tail
 * http://code.google.com/p/poly2tri/
  af_head_->next = af_middle_;
 *
  af_middle_->next = af_tail_;
 * All rights reserved.
  af_middle_->prev = af_head_;
 *
  af_tail_->prev = af_middle_;
 * Redistribution and use in source and binary forms, with or without modification,
}
 * are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice,
void SweepContext::RemoveNode(Node *node) {
 *   this list of conditions and the following disclaimer.
  delete node;
}
 * * Redistributions in binary form must reproduce the above copyright notice,

 *   this list of conditions and the following disclaimer in the documentation
void SweepContext::MapTriangleToNodes(Triangle &t) {
 *   and/or other materials provided with the distribution.
  for(int i = 0; i != 3; ++i) {
 * * Neither the name of Poly2Tri nor the names of its contributors may be
    if(!t.GetNeighbor(i)) {
 *   used to endorse or promote products derived from this software without specific
      Node *n = front_->LocatePoint(t.PointCW(*t.GetPoint(i)));
 *   prior written permission.
      if(n) {
 *
        n->triangle = &t;
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
      }
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    }
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  }
}
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR

 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
void SweepContext::RemoveFromMap(Triangle *triangle) {
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  map_.remove(triangle);
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
}
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF

 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
void SweepContext::MeshClean(Triangle &triangle) {
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  std::vector<Triangle*> triangles;
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  triangles.emplace_back(&triangle);
 */

#include "sweep_context.h"
  while(!triangles.empty()) {
#include <algorithm>
    Triangle *t = triangles.back();
#include "advancing_front.h"
    triangles.pop_back();


namespace p2t {
    if(t && !t->IsInterior()) {

      t->IsInterior(true);
SweepContext::SweepContext(polylinetype const &polyline)
      triangles_.emplace_back(t);
  : points_(polyline),
      for(int i = 0; i < 3; i++) {
    front_(0),
        if(!t->constrained_edge[i]) {
    head_(0),
          triangles.emplace_back(t->GetNeighbor(i));
    tail_(0),
        }
    af_head_(0),
      }
    af_middle_(0),
    }
    af_tail_(0) {
  }
  InitEdges(points_);
}

}
SweepContext::~SweepContext() {

  // Clean up memory
void SweepContext::AddHole(polylinetype const &polyline) {
  delete head_;
  InitEdges(polyline);
  delete tail_;
  for(auto const &it : polyline) {
  delete front_;
    points_.emplace_back(it);
  delete af_head_;
  }
  delete af_middle_;
}
  delete af_tail_;

  for(auto const &it : map_) {
void SweepContext::AddPoint(Point *point) {
    delete it;
  points_.emplace_back(point);
  }
}
  for(auto const &it : edge_list) {

    delete it;
  }
std::vector<Triangle*> &SweepContext::GetTriangles() {
}
  return triangles_;

}
}

