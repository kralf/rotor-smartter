#include "ApplicationWindow.h"

#include "ui_MainWindow.h"

//------------------------------------------------------------------------------

ApplicationWindow::ApplicationWindow( const Configuration & configuration )
  : _configuration( configuration ),
    _mainWidget( new Ui_MainWindow() )
{
  setAttribute( Qt::WA_DeleteOnClose );
  setWindowTitle( "application main window" );
  _mainWidget->setupUi( this );
  _mainWidget->navigationPlot->configuration( configuration );
  _mainWidget->localizationPlot->configuration( configuration );
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

//------------------------------------------------------------------------------

const Configuration &
ApplicationWindow::configuration() const
{
  return _configuration;
}
