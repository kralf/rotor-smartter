#ifndef URUS_WP2_POSE_GRAPH_H
#define URUS_WP2_POSE_GRAPH_H


#include "Pose.h"
#include <rana-graph/Exceptions.h>
#include <tr1/unordered_set>
#include <list>


namespace Urus {
namespace Wp2 {


class PoseGraph
{
public:
  typedef std::list<Pose> Neighborhood;
  typedef std::tr1::unordered_set<Pose> Nodes;
  
  PoseGraph( 
    double arcLength,
    double maxCurvature, 
    size_t branchingFactor );
  
  const Neighborhood & children( const Pose & node ) const throw( Rana::Graph::NotFoundError );
  const Nodes & nodes() const;
  
private:
  Pose child( const Pose & node, double curvature ) const;

  double       _arcLength;
  double       _maxCurvature;
  double       _increment;
  Neighborhood _children;
  Nodes        _nodes;
};


}
}


#endif //URUS_WP2_POSE_GRAPH_H
