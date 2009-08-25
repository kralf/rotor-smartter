#!/usr/bin/env python

import rotorc
import threading
from math import *
from elrob_messages import *

from PyQt4 import QtCore, QtGui, Qt
from PyQt4 import *
from elrob_monitor_ui import Ui_MainWindow

import sys
import os

#-------------------------------------------------------------------------------

class ApplicationWindow( QtGui.QMainWindow ):
  def __init__(self):
    QtGui.QMainWindow.__init__( self )
    self.setAttribute( QtCore.Qt.WA_DeleteOnClose )
    self.setWindowTitle( "application main window" )
    self.mainWidget = Ui_MainWindow()
    self.mainWidget.setupUi( self )

#-------------------------------------------------------------------------------

def updatePlots( window ):
  oldAlascaTimestamp       = 0
  x     = 0
  y     = 0
  theta = 0
  window.navigationPlot.setStatus( 0, "Ok" )
  while True:
    message = registry.receiveMessage()
    data    = message.data()
    if message.name() == "axt_message":
      if data.timestamp - oldAlascaTimestamp > 0.2:
        points  = []
        dx      = data.x
        dy      = data.y
        channel = data.channel
        del points[:]
        for i in range( data.num_points ):
          if channel[i] == 2:
            points.append( ( -dy[i], dx[i] ) )
        window.navigationPlot.setLaserPoints( points )
        oldAlascaTimestamp = data.timestamp
      elif data.timestamp - oldAlascaTimestamp < -1.0:
        oldAlascaTimestamp = data.timestamp
    elif message.name() == globalLocalizationMessage:
      x       = data.globalpos.x
      y       = data.globalpos.y
      theta   = data.globalpos.theta
      window.localizationPlot.updateReal( x, y )
      window.navigationPlot.updateReal( x, y, theta)
    elif message.name() == "smart_status_message":
      window.navigationPlot.setCommandSteeringAngle( data.steering_angle )
    elif message.name() == "path_message":
      path = []
      for i in xrange( data.point_count ):
        path.append( ( data.x[i], data.y[i], data.theta[i] ) )
      window.localizationPlot.updatePath( path )
      window.navigationPlot.updatePath( path )

#-------------------------------------------------------------------------------

def configureWidgets( moduleName, options, window ):
  try:
    n = window.navigationPlot
    n.scale            = options.getDouble( moduleName, "scale" )
    n.wheelDistance    = options.getDouble( "smart", "wheelDistance" )
    n.axesDistance     = options.getDouble( "smart", "axesDistance" )
    n.laserDistance    = options.getDouble( "smart", "laserDistance" )
    n.securityDistance = options.getDouble( "smart", "securityDistance" )
  except rotorc.OptionError, e:
    rotorc.Logger.error( e.message )
    exit( 1 )

#-------------------------------------------------------------------------------

moduleName = os.path.basename( sys.argv[0] )
if len( sys.argv ) != 2:
  rotorc.Logger.error( "Usage: " + moduleName + " <config.ini>" )
  exit( 1 )
  
options = rotorc.BaseOptions()
options.fromString( rotorc.fileContents( sys.argv[1] ) );
registry  = rotorc.RemoteRegistry( "CarmenRegistry", moduleName, options, "lib" );

globalLocalizationMessage = options.getString( moduleName, "globalLocalizationMessage" )

#-------------------------------------------------------------------------------

registry.registerMessageType( "axt_message", axt_message )
registry.subscribeToMessage( "axt_message", False, 4 );

registry.registerMessageType( "smart_status_message", smart_status_message )
registry.subscribeToMessage( "smart_status_message", False, 4 );

registry.registerMessageType( "path_message", path_message )
registry.subscribeToMessage( "path_message" );

registry.registerType( carmen_point_t )
registry.registerMessageType( globalLocalizationMessage, carmen_localize_globalpos_message )
registry.subscribeToMessage( globalLocalizationMessage, False, 4 );

#-------------------------------------------------------------------------------

application = QtGui.QApplication( [] )
window = ApplicationWindow()
configureWidgets( moduleName, options, window.mainWidget )
window.show()
updateThread = threading.Thread( None, updatePlots, None, ( window.mainWidget, ) )
updateThread.start()
exit( application.exec_() )

