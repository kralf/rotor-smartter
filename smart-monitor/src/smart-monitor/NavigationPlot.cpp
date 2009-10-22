#include "NavigationPlot.h"
#include "Configuration.h"
#include <QPaintEvent>
#include <QPainter>
#include <cmath>

//------------------------------------------------------------------------------


NavigationPlot::NavigationPlot( QWidget * parent )
  : QWidget( parent )
{
  connect( &_timer, SIGNAL( timeout() ), this, SLOT( repaint() ) );
  _timer.start( 200 );
}

//------------------------------------------------------------------------------

void
NavigationPlot::configuration( const Configuration & value )
{
  _configuration = &value;
}

//------------------------------------------------------------------------------

void
NavigationPlot::addLaserPoint( double x, double y )
{
  _laserX.push_back( x );
  _laserY.push_back( y );
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
NavigationPlot::paintEvent( QPaintEvent * event )
{
  double scale = 25;
  _lock.lockForRead();
  QPainter painter;
  painter.begin( this );
  painter.setRenderHint( QPainter::Antialiasing );
/*  if self.status == 0:*/
    painter.fillRect( event->rect(), QBrush( Qt::white ) );
/*    self.drawStatusMessage( painter, self.statusMessage )
  elif self.status == 1:
    painter.fillRect( event.rect(), QtGui.QBrush( QtCore.Qt.yellow ) )
    self.drawStatusMessage( painter, self.statusMessage )
  elif self.status == 2:
    painter.fillRect( event.rect(), QtGui.QBrush( QtCore.Qt.red ) )
    self.drawStatusMessage( painter, self.statusMessage )*/
  painter.translate( width() / 2.0, height() );
  if ( width() > height() ) {
    double ar = 1.0 * height() / width();
    painter.scale( ar * scale, -scale );
  } else {
    double ar = 1.0 * width() / height();
    painter.scale( scale, -scale * ar );
  }
  drawSteering( painter );
  drawCar( painter );
//  self.drawNext( painter )
  painter.translate( 0, _configuration->laserDistance() );
  drawLaserPoints( painter );
  painter.end();
  _lock.unlock();
}

//------------------------------------------------------------------------------

void
NavigationPlot::drawLaserPoints( QPainter & painter )
{
  QPen pen( QColor( 0, 128, 0 ) );
  pen.setWidthF( 0.1 );
  painter.setPen( pen );
  for ( size_t i = 0; i < _laserX.size(); ++i ) {
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
  if ( radius < 40 || center ) {
    QRectF rect( x - radius, y - radius, 2 * radius, 2 * radius );
    painter.drawArc( rect, -a1 * 2880 / M_PI, -a2 * 2880 / M_PI );
  } else {
    painter.drawLine( QLineF( x1, y1, x2, y2 ) );
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
  drawArc( painter, radius, 0, fabs( radius ), a1, a2 );
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
