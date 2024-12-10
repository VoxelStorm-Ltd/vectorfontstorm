#pragma once

#include <vector>
#include <memory>

namespace p2t {

struct Point;

//using polylinetype = std::vector<Point*>;
//using polylinetype = std::vector<std::unique_ptr<Point>>;
using polylinetype = std::vector<std::shared_ptr<Point>>;

}
