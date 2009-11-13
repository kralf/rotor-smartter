#include "NavigationPlot.h"
#include "Configuration.h"
#include <QPaintEvent>
#include <QPainter>
#include <QFileDialog>
#include <cmath>
#include <fstream>
#include <sstream>

#include <rotor/Time.h>
#include <rotor/NetUtils.h>

using namespace std;

//------------------------------------------------------------------------------


NavigationPlot::NavigationPlot( QWidget * parent ) :
  QWidget( parent ),
  _registry ( 0 ),
  _defaultPath ( "Global" ),
  _steeringAngle( 0.0 ),
  _commandSteeringAngle( 0.0 ),
  _scale ( 1.0 )
{
  connect( &_timer, SIGNAL( timeout() ), this, SLOT( repaint() ) );
  _timer.start( 200 );
}

//------------------------------------------------------------------------------

void
NavigationPlot::setRegistry( Rotor::Registry & registry )
{
  _registry = &registry;
}

//------------------------------------------------------------------------------

void
NavigationPlot::setDefaultPath( const std::string & defaultPath )
{
  _defaultPath = defaultPath;
}

//------------------------------------------------------------------------------

void
NavigationPlot::configuration( const Configuration & value )
{
  _configuration = &value;
}

//------------------------------------------------------------------------------

void
NavigationPlot::addLaserPoint( double x, double y, unsigned char status )
{
  _laserX.push_back( x );
  _laserY.push_back( y );
  _laserStatus.push_back( status );
}

//------------------------------------------------------------------------------

void
NavigationPlot::resetLaserData()
{
  size_t size = _laserX.size();
  _laserX.clear();
  _laserX.reserve( size );
  _laserY.clear();
  _laserY.reserve( size );
  _laserStatus.clear();
  _laserStatus.reserve( size );
}

//------------------------------------------------------------------------------

void
NavigationPlot::steeringAngle( double value )
{
  _steeringAngle = value;
}

//------------------------------------------------------------------------------

void
NavigationPlot::commandSteeringAngle( double value )
{
  _commandSteeringAngle = value;
}

//------------------------------------------------------------------------------

void
NavigationPlot::readPath( const std::string & filename )
{
  _lock.lockForRead();
  ifstream f( filename.c_str() );
  std::vector<double> & x = _x[_defaultPath];
  std::vector<double> & y = _y[_defaultPath];

  x.clear();
  y.clear();

  string s;
  while ( getline( f, s ) ) {
    stringstream line( s );
    x.resize( x.size() + 1 );
    y.resize( y.size() + 1 );
    line >> x.back();
    line >> y.back();
  }

  _lock.unlock();
}

//------------------------------------------------------------------------------

void
NavigationPlot::setScale( int value ) {
  _scale = exp(0.01*value);
  repaint();
}

//------------------------------------------------------------------------------

void
NavigationPlot::gui() {
  if ( _registry && ( _gui.state() == QProcess::NotRunning ) )
  {
    _gui.start( _registry->options().getString( "smartMonitor",
      "guiProcess" ).c_str() );
  }
}

//------------------------------------------------------------------------------

void
NavigationPlot::load()
{
  readPath("path.txt");
  start( _defaultPath );
}

//------------------------------------------------------------------------------

void
NavigationPlot::loadFrom()
{
  QFileDialog dialog(this, "Load Path");

  dialog.setFilter("Path Files (*.txt)");
  dialog.selectFile("path.txt");
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setDirectory(QDir::current());

  if (dialog.exec()) {
    readPath(dialog.selectedFiles().front().toLatin1().constData());
    start( _defaultPath );
  }
}

//------------------------------------------------------------------------------

void
NavigationPlot::start( const std::string& name )
{
  if (_registry) {
    PointSeries::iterator it = _x.find( name );

    if ( it != _x.end()) {
      std::vector<double> & x = _x[name];
      std::vector<double> & y = _y[name];

      Rotor::Structure pathMessage = _registry->newStructure("path_message");
      pathMessage["point_count"] = static_cast<int>( x.size() );
      pathMessage.adjust();

      for ( size_t i = 0; i < x.size(); ++i )
      {
        pathMessage["x"][i]     = x[i];
        pathMessage["y"][i]     = y[i];
        pathMessage["theta"][i] = 0;
      }

      pathMessage["timestamp"] = Rotor::seconds();
      pathMessage["host"] = const_cast<char*>( Rotor::hostName().c_str() );
      _registry->sendStructure( "path_message", pathMessage );
    }
    else
      stop();
  }
}

//------------------------------------------------------------------------------

void
NavigationPlot::stop()
{
  if (_registry) {
    Rotor::Structure stopMessage = _registry->newStructure("path_stop_message");

    stopMessage["timestamp"] = Rotor::seconds();
    stopMessage["host"] = const_cast<char*>( Rotor::hostName().c_str() );
    _registry->sendStructure( "path_stop_message", stopMessage );
  }
}

//------------------------------------------------------------------------------

void
NavigationPlot::paintEvent( QPaintEvent * event )
{
  double scale = 25;
  _lock.lockForRead();
  QPainter painter;
  painter.begin( this );
  painter.setRenderHint( QPainter::Antialiasing );
  painter.fillRect( event->rect(), palette().brush( QPalette::Base ) );

  painter.translate( width() / 2.0, height() );
  if ( width() > height() ) {
    double ar = 1.5 * height() / width();
    painter.scale( ar * scale, ar * -scale );
  } else {
    double ar = 1.5 * width() / height();
    painter.scale( ar * scale, ar * -scale );
  }
  painter.scale( _scale, _scale );

  drawSteering( painter );
  drawCar( painter );
  painter.translate( 0, _configuration->laserDistance() );
  drawLaserPoints( painter );
  painter.end();
  _lock.unlock();
}

//------------------------------------------------------------------------------

void
NavigationPlot::drawLaserPoints( QPainter & painter )
{
  QPen pen;
  pen.setWidthF( 0.1 );

  for ( size_t i = 0; i < _laserX.size(); ++i ) {
    if ( _laserStatus[i] )
      pen.setColor( QColor( 128, 0, 0 ) );
    else
      pen.setColor( QColor( 0, 128, 0 ) );

    painter.setPen( pen );
    painter.drawPoint( QPointF( _laserX[i], _laserY[i] ) );
  }
}

//------------------------------------------------------------------------------

void
NavigationPlot::drawCar( QPainter & painter )
{
  painter.setPen( Qt::blue );
  double x = 0 - _configuration->wheelDistance() / 2.0;
  double y = 0;
  painter.drawRect( QRectF( x, y, _configuration->wheelDistance(), _configuration->laserDistance() ) );
}

//------------------------------------------------------------------------------

QPointF
NavigationPlot::drawArc(
  QPainter & painter,
  double x,
  double y,
  double radius,
  double a1,
  double a2,
  bool center
)
{
  double x1 = x + radius * cos( a1 );
  double y1 = y + radius * sin( a1 );
  double x2 = x + radius * cos( a1 + a2 );
  double y2 = y + radius * sin( a1 + a2 );
  if ( radius < 1000 || center ) {
    QRectF rect( x - radius, y - radius, 2 * radius, 2 * radius );
    painter.drawArc( rect, -a1 * 2880 / M_PI, -a2 * 2880 / M_PI );
  } else {
    painter.drawLine( QLineF( x1, y1, x1, y2+a2 ) );
  }

  painter.setPen( QColor( 0, 0, 255 ) );
  QPainterPath p;
  p.addEllipse( QRectF( x1 - 0.2, y1 - 0.2, 0.4, 0.4 ) );
  painter.drawPath( p );

  painter.setPen( QColor( 255, 0, 0 ) );
  QPainterPath p1;
  p1.addEllipse( QRectF( x2 - 0.2, y2 - 0.2, 0.4, 0.4 ) );
  painter.drawPath( p1 );
  return QPointF( x2, y2 );
}

//------------------------------------------------------------------------------

void
NavigationPlot::drawSteeringArc( QPainter & painter, double steeringAngle )
{
  double sign   = 0;
  double radius = 0;
  if ( fabs( steeringAngle ) > 1E-6 ) {
    sign   = -steeringAngle / fabs( steeringAngle );
    radius = sign * sqrt( pow( _configuration->axesDistance() / sin( steeringAngle ), 2 ) - pow( _configuration->axesDistance(), 2 ) );
  } else {
    radius = 4000.0;
  }

  double a1 = 0;
  double a2 = 0;
  if ( radius < 0 ) {
    a1 = 0;
    a2 = M_PI / 2.0;
  } else {
    a1 = -M_PI;
    a2 = -M_PI / 2.0;
  }

  drawArc( painter, radius, 0, fabs( radius ), a1, a2, false );
}

//------------------------------------------------------------------------------

QPointF
NavigationPlot::drawClearArc(
  QPainter & painter,
  double steeringAngle,
  double offset
)
{
  double sign   = 0;
  double radius = 0;
  if ( fabs( steeringAngle ) > 1E-6 ) {
    sign   = -steeringAngle / fabs( steeringAngle  );
    radius = sign * sqrt( pow( _configuration->axesDistance() / sin( steeringAngle ), 2 ) - pow( _configuration->axesDistance(), 2 ) );
  } else {
    radius = 4000.0;
  }

  double a1      = 0;
  double a2      = 0;
  double nradius = 0;
  if ( radius < 0 ) {
    a1 = atan2( _configuration->laserDistance(), fabs( radius + offset ) );
    nradius = sqrt( pow( radius + offset, 2 ) + pow( _configuration->laserDistance(), 2 ) );
    a2 = _configuration->securityDistance() / fabs( nradius );
  } else {
    a1 = atan2( _configuration->laserDistance(), -fabs( radius + offset ) );
    nradius = sqrt( pow( radius + offset, 2 ) + pow( _configuration->laserDistance(), 2 ) );
    a2 = -_configuration->securityDistance() / fabs( nradius );
  }
  return drawArc( painter, radius, 0, nradius, a1, a2, false );
}

//------------------------------------------------------------------------------

void
NavigationPlot::drawSteering( QPainter & painter )
{
  painter.setPen( QColor( 0, 0, 255 ) );
  drawSteeringArc( painter, _steeringAngle );
  painter.setPen( QColor( 255, 0, 0 ) );
  drawSteeringArc( painter, _commandSteeringAngle );
  painter.setPen( QColor( 128, 128, 128 ) );
  QPointF p1 = drawClearArc( painter, _steeringAngle, _configuration->wheelDistance() / 2.0 );
  painter.setPen( QColor( 128, 128, 128 ) );
  QPointF p2 = drawClearArc( painter, _steeringAngle, -_configuration->wheelDistance() / 2.0 );
  painter.setPen( QColor( 128, 128, 128 ) );
  painter.drawLine( QLineF( p1.x(), p1.y(), p2.x(), p2.y() ) );
}
