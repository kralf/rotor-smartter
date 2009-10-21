#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H


#include "ui_MainWindow.h"
#include "Configuration.h"
#include <QMainWindow>


class ApplicationWindow : public QMainWindow
{
public:
    ApplicationWindow( const Configuration & configuration );
    Ui_MainWindow & mainWidget();
    const Configuration & configuration() const;
private:
  Configuration _configuration;
  Ui_MainWindow _mainWidget;
};


#endif //APPLICATION_WINDOW_H
