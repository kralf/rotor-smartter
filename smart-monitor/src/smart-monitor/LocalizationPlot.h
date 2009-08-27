#ifndef LOCALIZATION_PLOT_H
#define LOCALIZATION_PLOT_H


#include <QWidget>


class LocalizationPlot : public QWidget
{
  Q_OBJECT

public:
  LocalizationPlot( QWidget * parent );

public slots:
  void reset();
  void save();
};


#endif //LOCALIZATION_PLOT_H
