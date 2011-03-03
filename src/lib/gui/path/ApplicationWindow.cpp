#include "ApplicationWindow.h"

#include "ui_MainWindow.h"

//------------------------------------------------------------------------------

ApplicationWindow::ApplicationWindow()
  : _mainWidget( new Ui_MainWindow() )
{
  setAttribute( Qt::WA_DeleteOnClose );
  setWindowTitle( "application main window" );
  _mainWidget->setupUi( this );
}

ApplicationWindow::~ApplicationWindow()
{
  delete _mainWidget;
}

//------------------------------------------------------------------------------

Ui_MainWindow &
ApplicationWindow::mainWidget()
{
  return *_mainWidget;
}

PathPlot &
ApplicationWindow::pathPlot()
{
  return *_mainWidget->pathPlot;
}
