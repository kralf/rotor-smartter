#ifndef URUS_WP2_POSE_PLANNER_H
#define URUS_WP2_POSE_PLANNER_H


#include <list>

#include "PoseGraph.h"
#include "CostMap.h"


class PosePlanner
{
public:
  typedef std::list<Pose> Path;

  PosePlanner(
    std::string fileName,
    double cellSize,
    double x,
    double y,
    double robotRadius,
    double margin,
    double arcLength,
    double maxCurvature,
    double branchingFactor );

  Path plan(
    double x1, double y1, double theta1,
    double x2, double y2, double theta2,
    double timeoutSecond = 13.0, unsigned long int counterMax = 1000 );

  const PoseGraph & graph() const;

private:
  CostMap   _map;
  PoseGraph _graph;
  double    _robotRadius;
  double    _margin;
  double    _arcLength;
};


#endif //URUS_WP2_POSE_PLANNER_H
