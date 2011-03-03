#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include <QMainWindow>

#include "Configuration.h"

class Ui_MainWindow;

class ApplicationWindow : public QMainWindow
{
public:
    ApplicationWindow( const Configuration & configuration );
    ~ApplicationWindow();
    
    Ui_MainWindow & mainWidget();
    const Configuration & configuration() const;
private:
  Configuration _configuration;
  Ui_MainWindow * _mainWidget;
};


#endif //APPLICATION_WINDOW_H
