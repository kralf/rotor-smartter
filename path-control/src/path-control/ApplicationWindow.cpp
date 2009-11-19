#include "ApplicationWindow.h"

//------------------------------------------------------------------------------

ApplicationWindow::ApplicationWindow()
{
  setAttribute( Qt::WA_DeleteOnClose );
  setWindowTitle( "application main window" );
  _mainWidget.setupUi( this );
}

//------------------------------------------------------------------------------

Ui_MainWindow &
ApplicationWindow::mainWidget()
{
  return _mainWidget;
}
