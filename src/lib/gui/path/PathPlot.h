#ifndef PATH_PLOT_H
#define PATH_PLOT_H

#include <QWidget>
#include <QReadWriteLock>
#include <QTimer>
#include <QImage>

#include <map>
#include <vector>
#include <string>

#include <rotor/Registry.h>

class PathPlot : public QWidget
{
  Q_OBJECT

public:
  PathPlot( QWidget * parent );
  virtual ~PathPlot();

  void setRegistry( Rotor::Registry& registry, const std::string & planner );
  void setMap( const std::string & filename );
  void setPose( double x, double y, double theta );

  double scale() const;
  QPointF toMap( const QPoint & point );
  QPoint fromMap( const QPointF & point );

  void clearPath();
  void updatePath( double x, double y );

public slots:
  void stop();

protected:
  virtual void paintEvent( QPaintEvent * event );
  virtual void mousePressEvent( QMouseEvent * event );

private:
  Rotor::Registry*            _registry;
  QTimer                      _timer;
  QReadWriteLock              _lock;
  QImage                      _map;
  QPoint                      _origin;
  double                      _cellSize;
  double                      _robotRadius;

  double                      _offsetX;
  double                      _offsetY;
  double                      _robotX;
  double                      _robotY;
  double                      _robotTheta;
  std::vector<double>         _pathX;
  std::vector<double>         _pathY;

  void drawMap( QPainter & painter );
  void drawRobot( QPainter & painter );
  void drawPath( QPainter & painter );
};


#endif //NAVIGATION_PLOT_H
