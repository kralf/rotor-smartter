#ifndef LOCALIZATION_PLOT_H
#define LOCALIZATION_PLOT_H

#include <QWidget>
#include <QReadWriteLock>
#include <QGridLayout>
#include <QTimer>
#include <QProcess>

#include <qwt-qt4/qwt_plot.h>
#include <qwt-qt4/qwt_legend.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <qwt-qt4/qwt_plot_marker.h>

#include <map>
#include <vector>
#include <string>

#include <rotor/Registry.h>

class LocalizationPlot : public QWidget
{
  Q_OBJECT

public:
  LocalizationPlot( QWidget * parent, size_t maxPathPoints = 10000,
    double minUpdateDistance = 0.2 );
  virtual ~LocalizationPlot();

  void setRegistry( Rotor::Registry& registry );
  void setDefaultPath( const std::string & defaultPath );

  void clearPath( const std::string & name );
  void updatePath( const std::string & name, double x, double y );
  void updatePoint( const std::string & name, double x, double y );
  void writePath( const std::string & name, const std::string & filename );

public slots:
  void setScale( int value );
  void reset();
  void clear();
  void setPose();
  void save();
  void saveAs();
  void updateFigure();

protected:
  virtual void mousePressEvent( QMouseEvent * event );

private:
  typedef std::map<std::string, QwtPlotCurve*>        PlotCurves;
  typedef std::map<std::string, QwtPlotMarker*>       PointMarkers;
  typedef std::map<std::string, std::vector<double> > PointSeries;

  Rotor::Registry*  _registry;
  std::string       _defaultPath;

  size_t            _maxPathPoints;
  double            _minUpdateDistance;

  QTimer            _timer;
  QReadWriteLock    _lock;
  QGridLayout       _layout;
  QProcess          _gui;
  QwtPlot           _plot;
  QwtLegend         _legend;
  PlotCurves        _curves;
  PointMarkers      _points;
  PointSeries       _x;
  PointSeries       _y;

  double            _scale;

  void initializeCurve( const std::string & name );
  void removeCurve( const std::string & name );
};


#endif //LOCALIZATION_PLOT_H
