#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include <QMainWindow>

#include "PathPlot.h"

class Ui_MainWindow;

class ApplicationWindow : public QMainWindow
{
public:
    ApplicationWindow();
    ~ApplicationWindow();
    
    Ui_MainWindow & mainWidget();
    PathPlot & pathPlot();

private:
  Ui_MainWindow* _mainWidget;
};


#endif //APPLICATION_WINDOW_H
