#ifndef NAVIGATION_PLOT_H
#define NAVIGATION_PLOT_H


#include <QWidget>
#include <QReadWriteLock>
#include <QTimer>
#include <QProcess>

#include <map>
#include <vector>
#include <string>

#include <rotor/Registry.h>

class Configuration;


class NavigationPlot : public QWidget
{
  Q_OBJECT

public:
  NavigationPlot( QWidget * parent );
  virtual ~NavigationPlot();

  void setRegistry( Rotor::Registry& registry );
  void setDefaultPath( const std::string & defaultPath );

  void configuration( const Configuration & configuration );
  void addLaserPoint( double x, double y, unsigned char status );
  void resetLaserData();
  void steeringAngle( double value );
  void commandSteeringAngle( double value );
  void state( size_t state );
  void readPath( const std::string & filename );

public slots:
  void setScale( int value );
  void setGoal();
  void start( const std::string& name );
  void stop();
  void load();
  void loadFrom();

protected:
  virtual void paintEvent( QPaintEvent * event );
  virtual void mousePressEvent( QMouseEvent * event );


private:
  typedef std::map<std::string, std::vector<double> > PointSeries;

  Rotor::Registry*            _registry;
  std::string                 _defaultPath;
  const Configuration *       _configuration;
  QTimer                      _timer;
  QReadWriteLock              _lock;
  QProcess                    _gui;
  std::vector<double>         _laserX;
  std::vector<double>         _laserY;
  std::vector<unsigned char>  _laserStatus;
  double                      _steeringAngle;
  double                      _commandSteeringAngle;
  PointSeries                 _x;
  PointSeries                 _y;
  size_t                      _state;
  std::vector<double>         _nextPoint;

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
