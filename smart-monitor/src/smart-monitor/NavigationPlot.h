#ifndef NAVIGATION_PLOT_H
#define NAVIGATION_PLOT_H


#include <QWidget>
#include <QReadWriteLock>
#include <QTimer>


class Configuration;


class NavigationPlot : public QWidget
{
public:
  NavigationPlot( QWidget * parent );
  void configuration( const Configuration & configuration );
  void addLaserPoint( double x, double y );
  void resetLaserData();
  void steeringAngle( double value );
  void commandSteeringAngle( double value );
  
protected:
  void paintEvent( QPaintEvent * event );
  
private:
  const Configuration * _configuration;
  QTimer                _timer;
  QReadWriteLock        _lock;
  std::vector<double>   _laserX;
  std::vector<double>   _laserY;
  double                _steeringAngle;
  double                _commandSteeringAngle;

  void drawLaserPoints( QPainter & painter );
  void drawCar( QPainter & painter );
  QPointF drawArc( 
    QPainter & painter, 
    double x, 
    double y, 
    double radius, 
    double a1, 
    double a2, 
    bool center = true 
  );
  void drawSteeringArc( QPainter & painter, double steeringAngle );
  QPointF drawClearArc( 
    QPainter & painter, 
    double steeringAngle, 
    double offset );
  void drawSteering( QPainter & painter );
};


#endif //NAVIGATION_PLOT_H
