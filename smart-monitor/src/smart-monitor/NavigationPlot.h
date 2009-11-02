#ifndef NAVIGATION_PLOT_H
#define NAVIGATION_PLOT_H


#include <QWidget>
#include <QReadWriteLock>
#include <QTimer>


class Configuration;


class NavigationPlot : public QWidget
{
  Q_OBJECT

public:
  NavigationPlot( QWidget * parent );
  void configuration( const Configuration & configuration );
  void addLaserPoint( double x, double y, unsigned char status );
  void resetLaserData();
  void steeringAngle( double value );
  void commandSteeringAngle( double value );

public slots:
  void setScale( int value );

protected:
  void paintEvent( QPaintEvent * event );

private:
  const Configuration *       _configuration;
  QTimer                      _timer;
  QReadWriteLock              _lock;
  std::vector<double>         _laserX;
  std::vector<double>         _laserY;
  std::vector<unsigned char>  _laserStatus;
  double                      _steeringAngle;
  double                      _commandSteeringAngle;

  double                      _scale;

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
