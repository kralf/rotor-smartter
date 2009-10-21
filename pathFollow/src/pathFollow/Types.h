#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Core>
#include <vector>

class Vector;

typedef Eigen::Vector2d Point;
typedef std::vector<Vector, Eigen::aligned_allocator<Vector> > Path;

#endif //TYPES_H
