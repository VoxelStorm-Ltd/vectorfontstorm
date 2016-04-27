#ifndef POLY2TRI_POLYLINETYPE_H_INCLUDED
#define POLY2TRI_POLYLINETYPE_H_INCLUDED

#include <vector>
#include <memory>

namespace p2t {

struct Point;

//using polylinetype = std::vector<Point*>;
//using polylinetype = std::vector<std::unique_ptr<Point>>;
using polylinetype = std::vector<std::shared_ptr<Point>>;

}

#endif // POLY2TRI_POLYLINETYPE_H_INCLUDED
