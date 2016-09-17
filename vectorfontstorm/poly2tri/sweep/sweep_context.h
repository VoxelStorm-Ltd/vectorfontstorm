
    Basin() : left_node(nullptr), bottom_node(nullptr), right_node(nullptr), width(0.0), left_highest(false) {
    }

    void Clear() {
      left_node   = nullptr;
      bottom_node = nullptr;
      right_node  = nullptr;
      width = 0.0;
      left_highest = false;
    }
/*
  };
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors

 * http://code.google.com/p/poly2tri/
  struct EdgeEvent {
 *
    Edge *constrained_edge;
 * All rights reserved.
    bool right;
 *

 * Redistribution and use in source and binary forms, with or without modification,
    EdgeEvent() : constrained_edge(nullptr), right(false) {
 * are permitted provided that the following conditions are met:
    }
 *
  };

 * * Redistributions of source code must retain the above copyright notice,
  Basin basin;
 *   this list of conditions and the following disclaimer.
  EdgeEvent edge_event;
 * * Redistributions in binary form must reproduce the above copyright notice,

 *   this list of conditions and the following disclaimer in the documentation
//private:
 *   and/or other materials provided with the distribution.

 * * Neither the name of Poly2Tri nor the names of its contributors may be
  friend class Sweep;
 *   used to endorse or promote products derived from this software without specific

 *   prior written permission.
  std::vector<Triangle*> triangles_;
 *
  std::list<Triangle*> map_;
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  polylinetype points_;
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT

 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  // Advancing front
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  AdvancingFront *front_;
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  // head point used with advancing front
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  Point *head_;
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  // tail point used with advancing front
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  Point *tail_;

 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  Node *af_head_, *af_middle_, *af_tail_;
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS

 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  void InitTriangulation();
 */
  void InitEdges(polylinetype const &polyline);


#ifndef SWEEP_CONTEXT_H
};
#define SWEEP_CONTEXT_H


inline AdvancingFront *SweepContext::front() const {
#include <list>
  return front_;
#include <cstddef>
}
#include "../polylinetype.h"


inline size_t SweepContext::point_count() const {
namespace p2t {
  return points_.size();

}
// Inital triangle factor, seed triangle will extend 30% of

// PointSet width to both left and right.
inline void SweepContext::set_head(Point *p1) {
float constexpr const kAlpha = 0.3f;
  head_ = p1;

}
struct Point;

class Triangle;
inline Point *SweepContext::head() const {
struct Node;
  return head_;
struct Edge;
}
class AdvancingFront;


inline void SweepContext::set_tail(Point *p1) {
class SweepContext {
  tail_ = p1;
public:
}

inline Point *SweepContext::tail() const {
  return tail_;
}

}

#endif

  /// Constructor
  explicit SweepContext(polylinetype const &polyline);
  /// Destructor
  ~SweepContext();

  void set_head(Point *p1);

  Point *head() const;

  void set_tail(Point *p1);

  Point *tail() const;

  size_t point_count() const;

  Node &LocateNode(Point const &point);

  void RemoveNode(Node *node);

  void CreateAdvancingFront(std::vector<Node*> const &nodes);

  /// Try to map a node to all sides of this triangle that don't have a neighbor
  void MapTriangleToNodes(Triangle &t);

  void AddToMap(Triangle *triangle);

  Point *GetPoint(size_t index) __attribute__((__pure__));

  Point *GetPoints();

  void RemoveFromMap(Triangle *triangle);

  void AddHole(polylinetype const &polyline);

  void AddPoint(Point *point);

  AdvancingFront *front() const;

  void MeshClean(Triangle &triangle);

  std::vector<Triangle*> &GetTriangles() __attribute__((__const__));
  std::list<Triangle*> &GetMap() __attribute__((__const__));

  std::vector<Edge*> edge_list;

  struct Basin {
    Node *left_node;
    Node *bottom_node;
    Node *right_node;
    float width;
    bool left_highest;

    Basin() : left_node(nullptr), bottom_node(nullptr), right_node(nullptr), width(0.0), left_highest(false) {
    }

    void Clear() {
      left_node   = nullptr;
      bottom_node = nullptr;
      right_node  = nullptr;
      width = 0.0;
      left_highest = false;
    }
  };

  struct EdgeEvent {
    Edge *constrained_edge;
    bool right;

    EdgeEvent() : constrained_edge(nullptr), right(false) {
    }
  };

  Basin basin;
  EdgeEvent edge_event;

//private:

  friend class Sweep;

  std::vector<Triangle*> triangles_;
  std::list<Triangle*> map_;
  polylinetype points_;

  // Advancing front
  AdvancingFront *front_;
  // head point used with advancing front
  Point *head_;
  // tail point used with advancing front
  Point *tail_;

  Node *af_head_, *af_middle_, *af_tail_;

  void InitTriangulation();
  void InitEdges(polylinetype const &polyline);

};

inline AdvancingFront *SweepContext::front() const {
  return front_;
}

inline size_t SweepContext::point_count() const {
  return points_.size();
}

inline void SweepContext::set_head(Point *p1) {
  head_ = p1;
}

inline Point *SweepContext::head() const {
  return head_;
}

inline void SweepContext::set_tail(Point *p1) {
  tail_ = p1;
}

inline Point *SweepContext::tail() const {
  return tail_;
}

}

#endif
