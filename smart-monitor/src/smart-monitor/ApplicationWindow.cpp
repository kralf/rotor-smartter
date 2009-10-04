#include "ApplicationWindow.h"

//------------------------------------------------------------------------------

ApplicationWindow::ApplicationWindow( const Configuration & configuration )
  : _configuration( configuration )
{
  setAttribute( Qt::WA_DeleteOnClose );
  setWindowTitle( "application main window" );
  _mainWidget.setupUi( this );
  _mainWidget.navigationPlot->configuration( configuration );
}

//------------------------------------------------------------------------------

Ui_MainWindow &
ApplicationWindow::mainWidget()
{
  return _mainWidget;
}
