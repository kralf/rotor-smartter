#ifndef LOCALIZATION_PLOT_H
#define LOCALIZATION_PLOT_H


#include <QWidget>
#include <QReadWriteLock>
#include <QGridLayout>
#include <QTimer>
#include <qwt-qt4/qwt_plot.h>
#include <qwt-qt4/qwt_legend.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <qwt-qt4/qwt_plot_marker.h>
#include <map>
#include <vector>
#include <string>


class LocalizationPlot : public QWidget
{
  Q_OBJECT

public:
  LocalizationPlot( QWidget * parent );
  virtual ~LocalizationPlot();
  void updatePath( const std::string & name, double x, double y );
  void writePath( const std::string & filename );

public slots:
  void reset();
  void save();
  void saveAs();
  void updateFigure();

private:
  typedef std::map<std::string, QwtPlotCurve*>        PlotCurves;
  typedef std::map<std::string, QwtPlotMarker*>       PointMarkers;
  typedef std::map<std::string, std::vector<double> > PointSeries;

  QTimer         _timer;
  QReadWriteLock _lock;
  QGridLayout    _layout;
  QwtPlot        _plot;
  QwtLegend      _legend;
  PlotCurves     _curves;
  PointMarkers   _points;
  PointSeries    _x;
  PointSeries    _y;

  void initializeCurve( const std::string & name );
};


#endif //LOCALIZATION_PLOT_H
