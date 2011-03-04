#include <QPaintEvent>
#include <QPainter>

#include <cmath>
#include <fstream>
#include <sstream>

#include <rotor/BaseOptions.h>
#include <rotor/Time.h>
#include <rotor/NetUtils.h>

#include "PathPlot.h"

#include "Messages.h"

using namespace std;

//------------------------------------------------------------------------------

PathPlot::PathPlot( QWidget * parent ) :
  QWidget( parent ),
  _registry( 0 ),
  _cellSize( 0 ),
  _robotRadius( 1 ),
  _offsetX( 0 ),
  _offsetY( 0 ),
  _robotX( 0 ),
  _robotY( 0 ),
  _robotTheta( 0 )
{
  connect( &_timer, SIGNAL( timeout() ), this, SLOT( repaint() ) );
  _timer.start( 200 );
}

//------------------------------------------------------------------------------

PathPlot::~PathPlot()
{
}

//------------------------------------------------------------------------------

void
PathPlot::setRegistry( Rotor::Registry & registry, const std::string & planner )
{
  _registry = &registry;

  _cellSize = _registry->options().getDouble( planner, "cellSize" );
  _offsetX = _registry->options().getDouble( planner, "offsetX" );
  _offsetY = _registry->options().getDouble( planner, "offsetY" );
  _origin.setX(
    _registry->options().getDouble( planner, "originX" ) +
    _offsetX / _cellSize );
  _origin.setY(
    _registry->options().getDouble( planner, "originY" ) +
    -_offsetY / _cellSize );
  _robotRadius = _registry->options().getDouble( "smart", "radius" );
}

//------------------------------------------------------------------------------

void
PathPlot::setMap( const std::string & filename )
{
  _map.load( filename.c_str() );
}

//------------------------------------------------------------------------------

void
PathPlot::setPose( double x, double y, double theta )
{
  _robotX = x;
  _robotY = y;
  _robotTheta = theta;
}

//------------------------------------------------------------------------------

double
PathPlot::scale() const
{
  double mapAspect  = (double) _map.width() / _map.height();
  double drawAspect = (double)  width() / height();

  if ( mapAspect > drawAspect )
    return (double) width() / _map.width();
  else
    return (double) height() / _map.height();
}

//------------------------------------------------------------------------------

QPointF
PathPlot::toMap( const QPoint & point )
{
  double s = scale();
  QSize size( _map.width() * s, _map.height() * s );
  QPoint pos( 0.5 * ( width() - size.width() ),
    0.5 * ( height() - size.height() ) );

  double x = ( ( point.x() - pos.x() ) / s - _origin.x() ) * _cellSize;
  double y = - ( ( point.y() - pos.y() ) / s - _origin.y() ) * _cellSize;

  return QPointF( x, y );
}

//------------------------------------------------------------------------------

QPoint
PathPlot::fromMap( const QPointF & point )
{
  double s = scale();
  QSize size( _map.width() * s, _map.height() * s );
  QPoint pos( 0.5 * ( width() - size.width() ),
    0.5 * ( height() - size.height() ) );

  int x = ( point.x() / _cellSize + _origin.x() ) * s + pos.x() ;
  int y = ( - point.y() / _cellSize + _origin.y() ) * s + pos.y();

  return QPoint( x, y );
}

//------------------------------------------------------------------------------

void
PathPlot::clearPath()
{
  _lock.lockForWrite();

  _pathX.clear();
  _pathY.clear();

  _lock.unlock();
}

//------------------------------------------------------------------------------

void
PathPlot::updatePath( double x, double y )
{
  _lock.lockForWrite();

  _pathX.push_back( x );
  _pathY.push_back( y );

  _lock.unlock();
}

//------------------------------------------------------------------------------

void
PathPlot::stop()
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
PathPlot::paintEvent( QPaintEvent * event )
{
  _lock.lockForRead();
  QPainter painter;
  painter.begin( this );
  painter.fillRect( event->rect(), palette().brush( QPalette::Base ) );

  drawMap( painter );
  drawPath( painter );
  drawRobot( painter );

  painter.end();
  _lock.unlock();
}

//------------------------------------------------------------------------------

void
PathPlot::mousePressEvent( QMouseEvent * event )
{
  if ( _registry )
  {
    QPointF goal = toMap( event->pos() );

    Rotor::Structure data = _registry->newStructure( "planner_plan_message" );
    planner_plan_message & planMessage =
      * reinterpret_cast<planner_plan_message *>( data.buffer() );

    planMessage.start.x     = _robotX;
    planMessage.start.y     = _robotY;
    planMessage.start.theta = _robotTheta;

    planMessage.goal.x      = goal.x();
    planMessage.goal.y      = goal.y();
    planMessage.goal.theta  = 0;

    data["timestamp"] = Rotor::seconds();
    data["host"] = const_cast<char*>( Rotor::hostName().c_str() );
    _registry->sendStructure( "planner_plan_message", data );
  }

  QWidget::mousePressEvent( event );
}

//------------------------------------------------------------------------------

void
PathPlot::drawMap( QPainter & painter )
{
  double s = scale();
  QSize size( _map.width() * s, _map.height() * s );
  QPoint pos( 0.5 * ( width() - size.width() ),
    0.5 * ( height() - size.height() ) );

  painter.drawImage( QRect( pos, size ), _map );
}

//------------------------------------------------------------------------------

void
PathPlot::drawRobot( QPainter & painter )
{
  QPen pen( QColor( 0, 0, 0 ) );
  painter.setPen( pen );
  QBrush brush( QColor( 255, 0, 0 ) );
  painter.setBrush( brush );
  painter.setRenderHint( QPainter::Antialiasing );
  QPoint pose = fromMap( QPointF( _robotX, _robotY ) );
  int r = scale() * _robotRadius / _cellSize;

  painter.drawEllipse( pose, r, r );
}

//------------------------------------------------------------------------------

void
PathPlot::drawPath( QPainter & painter )
{
  QPen pen( QColor( 0, 0, 255 ) );
  painter.setPen( pen );
  painter.setRenderHint( QPainter::Antialiasing, false );

  if ( _pathX.size() > 1 )
  {
    for ( size_t i = 1; i < _pathX.size(); ++i )
    {
      QPoint p1 = fromMap( QPointF( _pathX[i - 1], _pathY[i - 1] ) );
      QPoint p2 = fromMap( QPointF( _pathX[i], _pathY[i] ) );
      painter.drawLine( p1, p2 );
    }
  }
}
