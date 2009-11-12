#include "LocalizationPlot.h"
#include <rotor/Logger.h>
#include <qwt-qt4/qwt_plot_layout.h>
#include <qwt-qt4/qwt_plot_canvas.h>
#include <qwt-qt4/qwt_symbol.h>
#include <QPen>
#include <QFileDialog>
#include <fstream>
#include <algorithm>
#include <cfloat>

using namespace std;

//------------------------------------------------------------------------------

LocalizationPlot::LocalizationPlot( QWidget * parent, size_t maxPathPoints,
  double minUpdateDistance ) :
  QWidget( parent ),
  _maxPathPoints(maxPathPoints),
  _minUpdateDistance(minUpdateDistance)
{
  setLayout( &_layout );

  _plot.canvas()->setFrameShape( QFrame::NoFrame );
  _plot.setCanvasBackground( palette().color( QPalette::Base ) );
  _plot.plotLayout()->setCanvasMargin( 0 );
  _plot.plotLayout()->setAlignCanvasToScales( true );

  QFont axisFont = font();
  _plot.enableAxis( QwtPlot::yLeft, true );
  _plot.setAxisFont( QwtPlot::yLeft, axisFont );
  _plot.enableAxis( QwtPlot::yRight, true );
  _plot.setAxisFont( QwtPlot::yRight, axisFont );
  _plot.enableAxis( QwtPlot::xBottom, true );
  _plot.setAxisFont( QwtPlot::xBottom, axisFont );
  _plot.enableAxis( QwtPlot::xTop, true );
  _plot.setAxisFont( QwtPlot::xTop, axisFont );

  _plot.insertLegend( &_legend, QwtPlot::TopLegend );

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
  writePath("path.txt");
}

//------------------------------------------------------------------------------

void
LocalizationPlot::saveAs()
{
  QFileDialog dialog(this, "Save Path");

  dialog.setFilter("Path Files (*.txt)");
  dialog.selectFile("path.txt");
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setConfirmOverwrite(true);
  dialog.setDirectory(QDir::current());

  if (dialog.exec())
    writePath(dialog.selectedFiles().front().toLatin1().constData());
}

//------------------------------------------------------------------------------

void
LocalizationPlot::reset()
{
  _lock.lockForRead();
  PlotCurves::iterator it;
  PlotCurves::iterator end = _curves.end();
  for ( it = _curves.begin(); it != end; ++it ) {
    const string & name      = it->first;
    _x[name].clear();
    _y[name].clear();
  }
  _lock.unlock();
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

  bool flag = false;

  PointSeries::iterator it;
  PointSeries::iterator end = _x.end();
  for ( it = _x.begin(); it != end; ++it ) {
    const string & name      = it->first;

    PlotCurves::iterator it = _curves.find( name );
    if ( it == _curves.end() )
      initializeCurve( name );

    QwtPlotCurve & curve     = *(_curves[name]);
    QwtPlotMarker & point    = *(_points[name]);
    std::vector<double> & tx = _x[name];
    std::vector<double> & ty = _y[name];

    if ( tx.size() > 0 ) {
      curve.setData( &(tx[0]), &(ty[0]), tx.size() );
      point.setValue( tx.back(), ty.back() );
      maxX = max( maxX, curve.maxXValue() );
      minX = min( minX, curve.minXValue() );
      maxY = max( maxY, curve.maxYValue() );
      minY = min( minY, curve.minYValue() );
      flag = true;
    }
  }

  if (flag == true)
  {
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
    _plot.setAxisScale( QwtPlot::yLeft, y1, y2 );
    _plot.setAxisScale( QwtPlot::yRight, y1, y2 );
    _plot.setAxisScale( QwtPlot::xBottom, x1, x2 );
    _plot.setAxisScale( QwtPlot::xTop, x1, x2 );
  }
  else
  {
    _plot.setAxisAutoScale( QwtPlot::yLeft );
    _plot.setAxisAutoScale( QwtPlot::yRight );
    _plot.setAxisAutoScale( QwtPlot::xBottom );
    _plot.setAxisAutoScale( QwtPlot::xTop );
  }

  _plot.replot();
  _lock.unlock();
}

//------------------------------------------------------------------------------

void
LocalizationPlot::updatePath( const std::string & name, double x, double y )
{
  _lock.lockForWrite();

  std::vector<double> & tx = _x[name];
  std::vector<double> & ty = _y[name];

  if ( !tx.empty() ) {
    double lx = tx.back();
    double ly = ty.back();
    double dist = ( lx - x ) * ( lx - x ) +
      ( ly - y ) * ( ly - y );

    if ( dist >= _minUpdateDistance ) {
      tx.push_back( x );
      ty.push_back( y );
    }
  }
  else {
    tx.push_back( x );
    ty.push_back( y );
  }

  if ( tx.size() > _maxPathPoints ) {
    tx.erase( tx.begin() );
    ty.erase( ty.begin() );
  }

  _lock.unlock();
}

//------------------------------------------------------------------------------

void
LocalizationPlot::writePath( const std::string & filename )
{
  _lock.lockForRead();
  std::ofstream f( filename.c_str() );
  std::vector<double> & x = _x["Global"];
  std::vector<double> & y = _y["Global"];
  if ( x.size() > 0 )
  {
    for ( size_t i = 0; i < x.size(); ++i ) {
      f << x[i] << " " << y[i] << endl;
    }
  }
  _lock.unlock();
}

//------------------------------------------------------------------------------

void
LocalizationPlot::initializeCurve( const std::string & name )
{
  if ( _curves.find( name ) == _curves.end() ) {
    _curves[name]         = new QwtPlotCurve( name.c_str() );
    _points[name]         = new QwtPlotMarker();
    QwtPlotCurve & curve  = *(_curves[name]);
    QwtPlotMarker & point = *(_points[name]);

    curve.attach( &_plot );
    point.attach( &_plot );
  }

  PlotCurves::iterator it;
  for ( it = _curves.begin(); it != _curves.end(); ++it ) {
    const string & name      = it->first;
    Qt::GlobalColor color = ( Qt::GlobalColor ) ( Qt::red +
        distance( _curves.begin(), it ) );

    _curves[name]->setPen( QPen( color ) );

    QwtSymbol symbol;
    symbol.setStyle( QwtSymbol::Cross );
    symbol.setPen( QPen( color ) );
    symbol.setSize( 10 );
    _points[name]->setSymbol( symbol );
  }
}
