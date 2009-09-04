#ifndef LOCALIZATION_PLOT_H
#define LOCALIZATION_PLOT_H


#include <QWidget>
#include <QReadWriteLock>
#include <QGridLayout>
#include <QTimer>
#include <qwt-qt4/qwt_plot.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <vector>


class LocalizationPlot : public QWidget
{
  Q_OBJECT

public:
  LocalizationPlot( QWidget * parent );

public slots:
  void reset();
  void save();
  void updateFigure();
  void updateGlobal( double x, double y );
  
private:
  QTimer              _timer;
  QReadWriteLock      _lock;
  QGridLayout         _layout;
  QwtPlot             _plot;
  QwtPlotCurve        _globalCurve;
  std::vector<double> _x;
  std::vector<double> _y;
};


#endif //LOCALIZATION_PLOT_H
