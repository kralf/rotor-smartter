#include "LocalizationPlot.h"
#include <rotor/Logger.h>
#include <qwt-qt4/qwt_plot_layout.h>
#include <QPen>
#include <algorithm>

//------------------------------------------------------------------------------

LocalizationPlot::LocalizationPlot( QWidget * parent )
  : QWidget( parent ),
    _globalCurve( "Global" )
{
/*  _x = []
  _y = []
  _rx = []
  _ry = []
  _path = []
  _nextX = 0.0
  _nextY = 0.0*/
  setLayout( &_layout );
  _plot.setCanvasBackground( Qt::white );
  _plot.plotLayout()->setCanvasMargin( 0 );
  _plot.plotLayout()->setAlignCanvasToScales( true );
  
  _globalCurve.attach( &_plot );
  _globalCurve.setPen( QPen( Qt::blue ) );
// 
//   _point = Qwt.QwtPlotMarker()
//   _point.attach( _plot )
//   symbol = Qwt.QwtSymbol()
//   symbol.setStyle( Qwt.QwtSymbol.Cross )
//   symbol.setPen(  Qt.QPen( Qt.Qt.blue ) )
//   symbol.setSize( 10 )
//   _point.setSymbol( symbol )
// 
//   _next = Qwt.QwtPlotMarker()
//   _next.attach( _plot )
//   symbol = Qwt.QwtSymbol()
//   symbol.setStyle( Qwt.QwtSymbol.Star2 )
//   symbol.setPen(  Qt.QPen( Qt.Qt.green ) )
//   symbol.setSize( 10 )
//   _next.setSymbol( symbol )
// 
//   
  _layout.addWidget( &_plot, 1, 1 );
// 
  _plot.replot();
//   
  connect( &_timer, SIGNAL( timeout() ), this, SLOT( updateFigure() ) );
  _timer.start( 500 );
}

//------------------------------------------------------------------------------

void
LocalizationPlot::save()
{
  Rotor::Logger::error( "Not implemented" );
}

//------------------------------------------------------------------------------

void
LocalizationPlot::reset()
{
  Rotor::Logger::error( "Not implemented" );
}

//------------------------------------------------------------------------------

void
LocalizationPlot::updateFigure()
{
  _lock.lockForRead();
  _globalCurve.setData( &(_x[0]), &(_y[0]), _x.size() );
//   if len( _x ) > 0:
//     _point.setValue( _x[-1], _y[-1] )
//   _next.setValue( _nextX, _nextY )
  double maxX = _globalCurve.maxXValue();
  double minX = _globalCurve.minXValue();
  double maxY = _globalCurve.maxYValue();
  double minY = _globalCurve.minYValue();
/*  if len( _path ) > 0:
    _pcurve.setData( [p[0] for p in _path], [p[1] for p in _path] )
    maxX = max( maxX, _pcurve.maxXValue() )
    minX = max( minX, _pcurve.minXValue() )
    maxY = max( maxY, _pcurve.maxYValue() )
    minY = max( minY, _pcurve.minYValue() )*/
  double dX = maxX - minX + 1.0;
  double dY = maxY - minY + 1.0;
  double delta = std::max( dX, dY ) / 2.0;
  
  double x  = ( minX + maxX ) / 2.0;
  double y  = ( minY + maxY ) / 2.0;
  
  double sx = size().width();
  double sy = size().height();
  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 0.0;
  double y2 = 0.0;
  double factor = 1.0;
  
  if ( sx > sy ) {
    factor = 1.0 * sx / sy;
    x1 = x - delta * factor;
    x2 = x + delta * factor;
    y1 = y - delta;
    y2 = y + delta;
  } else {
    factor = 1.0 * sy / sx;
    x1 = x - delta;
    x2 = x + delta;
    y1 = y - delta * factor;
    y2 = y + delta * factor;
  }
  _plot.setAxisScale( QwtPlot::xBottom, x1, x2 );
  _plot.setAxisScale( QwtPlot::yLeft, y1, y2 );
  _plot.replot();
  _lock.unlock();
}

//------------------------------------------------------------------------------

void
LocalizationPlot::updateGlobal( double x, double y )
{
  _lock.lockForWrite();
  _x.push_back( x );
  _y.push_back( y );
  if ( _x.size() > 2000 ) {
    _x.erase( _x.begin() );
    _y.erase( _y.begin() );
  }
  _lock.unlock();
}
