#include "LocalizationPlot.h"
#include <rotor/Logger.h>
#include <qwt-qt4/qwt_plot_layout.h>
#include <qwt-qt4/qwt_symbol.h>
#include <QPen>
#include <algorithm>
#include <cfloat>

using namespace std;

//------------------------------------------------------------------------------

LocalizationPlot::LocalizationPlot( QWidget * parent )
  : QWidget( parent )
{
  setLayout( &_layout );
  _plot.setCanvasBackground( Qt::white );
  _plot.plotLayout()->setCanvasMargin( 0 );
  _plot.plotLayout()->setAlignCanvasToScales( true );
  
  _layout.addWidget( &_plot, 1, 1 );
  _plot.replot();
  
  connect( &_timer, SIGNAL( timeout() ), this, SLOT( updateFigure() ) );
  _timer.start( 500 );
}

//------------------------------------------------------------------------------

LocalizationPlot::~LocalizationPlot()
{
  PlotCurves::iterator curveIt;
  PlotCurves::iterator curveEnd = _curves.end();
  for ( curveIt = _curves.begin(); curveIt != curveEnd; ++curveIt ) {
    delete curveIt->second;
  }
  PointMarkers::iterator pointIt;
  PointMarkers::iterator pointEnd = _points.end();
  for ( pointIt = _points.begin(); pointIt != pointEnd; ++pointIt ) {
    delete pointIt->second;
  }
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
  
  double maxX = DBL_MIN;
  double minX = DBL_MAX;
  double maxY = DBL_MIN;
  double minY = DBL_MAX;

  PlotCurves::iterator it;
  PlotCurves::iterator end = _curves.end();
  for ( it = _curves.begin(); it != end; ++it ) {
    const string & name      = it->first;
    QwtPlotCurve & curve     = *(_curves[name]);
    QwtPlotMarker & point    = *(_points[name]);
    std::vector<double> & tx = _x[name];
    std::vector<double> & ty = _y[name];
    
    curve.setData( &(tx[0]), &(ty[0]), tx.size() );
    if ( tx.size() > 0 ) {
      point.setValue( tx.back(), ty.back() );
    }
    maxX = max( maxX, curve.maxXValue() );
    minX = min( minX, curve.minXValue() );
    maxY = max( maxY, curve.maxYValue() );
    minY = min( minY, curve.minYValue() );
  }
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
LocalizationPlot::updatePath( const std::string & name, double x, double y )
{
  _lock.lockForWrite();
  initializeCurve( name );
  std::vector<double> & tx = _x[name];
  std::vector<double> & ty = _y[name];
  tx.push_back( x );
  ty.push_back( y );
  if ( tx.size() > 2000 ) {
    tx.erase( tx.begin() );
    ty.erase( ty.begin() );
  }
  _lock.unlock();
}

//------------------------------------------------------------------------------

void
LocalizationPlot::initializeCurve( const std::string & name )
{
  if ( _curves.find( name ) == _curves.end() ) {
    _curves[name]         = new QwtPlotCurve();
    _points[name]         = new QwtPlotMarker();
    QwtPlotCurve & curve  = *(_curves[name]);
    QwtPlotMarker & point = *(_points[name]);
    
    curve.attach( &_plot );
    curve.setPen( QPen( Qt::blue ) );

    point.attach( &_plot );
    QwtSymbol symbol;
    symbol.setStyle( QwtSymbol::Cross );
    symbol.setPen(  QPen( _curves.size() ) );
    symbol.setSize( 10 );
    point.setSymbol( symbol );
  }
}

