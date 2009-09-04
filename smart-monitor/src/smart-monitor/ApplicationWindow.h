#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H


#include "ui_MainWindow.h"
#include <QMainWindow>


class ApplicationWindow : public QMainWindow
{
public:
    ApplicationWindow();
    Ui_MainWindow & mainWidget();
private:
  Ui_MainWindow _mainWidget;
};


#endif //APPLICATION_WINDOW_H
