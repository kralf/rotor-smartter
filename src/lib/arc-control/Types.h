#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Core>
#include <vector>

enum State {
  idle = 0,
  following = 1,
  waiting = 2
};

class Vector;

typedef Eigen::Vector2d Point;
typedef std::vector<Vector, Eigen::aligned_allocator<Vector> > Path;

#endif //TYPES_H
