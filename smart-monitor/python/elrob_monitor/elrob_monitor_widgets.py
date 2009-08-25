import sys
from math import *
import numpy

from PyQt4 import *
import PyQt4.Qwt5 as Qwt

#-------------------------------------------------------------------------------

class LocalizationPlot( QtGui.QWidget ):
  def __init__( self, *args, **kwargs ):
    Qt.QWidget.__init__(self, *args)
    self.lock  = QtCore.QReadWriteLock()
    self.x = []
    self.y = []
    self.rx = []
    self.ry = []
    self.path = []
    self.nextX = 0.0
    self.nextY = 0.0
    self.layout  = Qt.QGridLayout( self )            
    self.plot    = Qwt.QwtPlot( self )
    #self.plot.setTitle( 'Localization' )
    self.plot.setCanvasBackground( Qt.Qt.white )
    self.plot.plotLayout().setCanvasMargin( 0 )
    self.plot.plotLayout().setAlignCanvasToScales( True )
    
    self.curve = Qwt.QwtPlotCurve( 'Localization') 
    self.curve.attach( self.plot )
    self.curve.setPen( Qt.QPen( Qt.Qt.red ) )
    self.curve.setData( self.x, self.y )

    self.rcurve = Qwt.QwtPlotCurve( 'Global') 
    self.rcurve.attach( self.plot )
    self.rcurve.setPen( Qt.QPen( Qt.Qt.blue ) )
    self.rcurve.setData( self.rx, self.ry )

    self.pcurve = Qwt.QwtPlotCurve( 'Path') 
    self.pcurve.attach( self.plot )
    self.pcurve.setPen( Qt.QPen( Qt.Qt.green ) )
    self.pcurve.setData( [p[0] for p in self.path], [p[1] for p in self.path] )

    self.point = Qwt.QwtPlotMarker()
    self.point.attach( self.plot )
    symbol = Qwt.QwtSymbol()
    symbol.setStyle( Qwt.QwtSymbol.Cross )
    symbol.setPen(  Qt.QPen( Qt.Qt.blue ) )
    symbol.setSize( 10 )
    self.point.setSymbol( symbol )

    self.next = Qwt.QwtPlotMarker()
    self.next.attach( self.plot )
    symbol = Qwt.QwtSymbol()
    symbol.setStyle( Qwt.QwtSymbol.Star2 )
    symbol.setPen(  Qt.QPen( Qt.Qt.green ) )
    symbol.setSize( 10 )
    self.next.setSymbol( symbol )

    
    self.layout.addWidget( self.plot, 1, 1 )

    self.plot.replot()    
    
    timer = QtCore.QTimer(self)    
    QtCore.QObject.connect(timer, QtCore.SIGNAL( "timeout()" ), self.updateFigure )
    timer.start( 500 )
    
  #-----------------------------------------------------------------------------
  
  def updateFigure( self ):
    self.lock.lockForRead()
    self.curve.setData( self.x, self.y )
    self.rcurve.setData( self.rx, self.ry )
    if len( self.x ) > 0:
      self.point.setValue( self.x[-1], self.y[-1] )
    self.next.setValue( self.nextX, self.nextY )
    maxX = max( self.curve.maxXValue(), self.rcurve.maxXValue() )
    minX = min( self.curve.minXValue(), self.rcurve.minXValue() )
    maxY = max( self.curve.maxYValue(), self.rcurve.maxYValue() )
    minY = min( self.curve.minYValue(), self.rcurve.minYValue() )
    if len( self.path ) > 0:
      self.pcurve.setData( [p[0] for p in self.path], [p[1] for p in self.path] )
      maxX = max( maxX, self.pcurve.maxXValue() )
      minX = max( minX, self.pcurve.minXValue() )
      maxY = max( maxY, self.pcurve.maxYValue() )
      minY = max( minY, self.pcurve.minYValue() )
    dX = maxX - minX + 1.0
    dY = maxY - minY + 1.0
    delta = max( dX, dY ) / 2.0
    
    x  = ( minX + maxX ) / 2.0
    y  = ( minY + maxY ) / 2.0
    
    sx = self.size().width()
    sy = self.size().height()
    if sx > sy:
      factor = 1.0 * sx / sy
      x1 = x - delta * factor
      x2 = x + delta * factor
      y1 = y - delta
      y2 = y + delta
    else:
      factor = 1.0 * sy / sx
      x1 = x - delta
      x2 = x + delta
      y1 = y - delta * factor
      y2 = y + delta * factor
    self.plot.setAxisScale( Qwt.QwtPlot.xBottom, x1, x2 )
    self.plot.setAxisScale( Qwt.QwtPlot.yLeft, y1, y2 )
    self.plot.replot()    
    self.lock.unlock()

  #-----------------------------------------------------------------------------
  
  def updateData( self, x, y ):
    self.lock.lockForWrite()
    self.x.append( x )
    self.y.append( y )
    if len( self.x ) > 2000:
      self.x.pop( 0 )
      self.y.pop( 0 )
    self.lock.unlock()

  #-----------------------------------------------------------------------------
  
  def updateReal( self, x, y ):
    self.lock.lockForWrite()
    self.rx.append( x )
    self.ry.append( y )
    if len( self.rx ) > 2000:
      self.rx.pop( 0 )
      self.ry.pop( 0 )
    self.lock.unlock()
    
  #-----------------------------------------------------------------------------
  
  def updatePath( self, path ):
    self.lock.lockForWrite()
    self.path = path
    self.lock.unlock()
    
  #-----------------------------------------------------------------------------
  
  def updateNext( self, x, y ):
    self.lock.lockForWrite()
    self.nextX = x
    self.nextY = y
    self.lock.unlock()

  #-----------------------------------------------------------------------------
  
  def reset( self ):
    self.lock.lockForWrite()
    self.x = [self.x[-1]]
    self.y = [self.y[-1]]
    self.rx = [self.rx[-1]]
    self.ry = [self.ry[-1]]
    self.lock.unlock()

  #-----------------------------------------------------------------------------
  
  def save( self ):
    self.lock.lockForRead()
    f = open( "path.txt", "w" )
    if len( self.rx ) > 0:
      for i in xrange( len( self.rx ) ):
        f.write( "%f %f\n" % ( self.rx[i], self.ry[i] )  )
    else:
      for i in xrange( len( self.x ) ):
        f.write( "%f %f\n" % ( self.x[i], self.y[i] )  )
    f.close()
    self.lock.unlock()

#-------------------------------------------------------------------------------

class NavigationPlot( QtGui.QWidget ):
  def __init__(self, parent = None ):
    QtGui.QWidget.__init__( self, parent )
    self.lock  = QtCore.QReadWriteLock()
    self.scale = 25
    self.laser = []
    self.steeringAngle = 0.001
    self.commandSteeringAngle = 0.001
    self.wheelDistance = 2.0
    self.axesDistance  = 1.812
    self.laserDistance = 1.812
    self.securityDistance = 1.0
    self.status        = 0
    self.statusMessage = ""
    self.robotX        = 0
    self.robotY        = 0
    self.robotTheta    = 0
    self.nextX         = 0
    self.nextY         = 0
    self.path          = []
    self.setSizePolicy(
      QtGui.QSizePolicy.Expanding,
      QtGui.QSizePolicy.Expanding
    )
    timer = QtCore.QTimer(self)    
    QtCore.QObject.connect(timer, QtCore.SIGNAL( "timeout()" ), self.repaint)
    timer.start( 200 )  
    
  #-----------------------------------------------------------------------------

  def minimumSizeHint( self ):
    return QtCore.QSize( 100, 200 )

  #-----------------------------------------------------------------------------

  def sizeHint(self):
    return QtCore.QSize( 100, 100 )
    
  #-----------------------------------------------------------------------------

  def setLaserPoints( self, points ):
    self.lock.lockForWrite()
    self.laser = points
    self.lock.unlock()

  #-----------------------------------------------------------------------------

  def setSteeringAngle( self, steeringAngle ):
    self.lock.lockForWrite()
    self.steeringAngle = steeringAngle
    self.lock.unlock()

  #-----------------------------------------------------------------------------

  def setCommandSteeringAngle( self, steeringAngle ):
    self.lock.lockForWrite()
    self.commandSteeringAngle = steeringAngle
    self.lock.unlock()

  #-----------------------------------------------------------------------------

  def setStatus( self, status, statusMessage ):
    self.lock.lockForWrite()
    self.status        = status
    self.statusMessage = statusMessage
    self.lock.unlock()
    
  #-----------------------------------------------------------------------------

  def updateReal( self, x, y, theta ):
    self.lock.lockForWrite()
    self.robotX     = x
    self.roboty     = y
    self.robotTheta = theta
    self.lock.unlock()

  #-----------------------------------------------------------------------------
  
  def updatePath( self, path ):
    self.lock.lockForWrite()
    self.path = path
    self.lock.unlock()
    
  #-----------------------------------------------------------------------------

  def updateNext( self, x, y ):
    self.lock.lockForWrite()
    self.nextX = x
    self.nextY = y
    self.lock.unlock()

  #-----------------------------------------------------------------------------

  def drawCar( self, painter ):
    painter.setPen( QtCore.Qt.blue )
    x, y = 0 - self.wheelDistance / 2.0, 0
    painter.drawRect( QtCore.QRectF( x, y, self.wheelDistance, self.axesDistance ) )

  #-----------------------------------------------------------------------------

  def drawLaserPoints( self, painter ):
    pen =QtGui.QPen( QtGui.QColor( 0, 128, 0 ) )
    pen.setWidthF( 0.1 )
    painter.setPen( pen )
    for p in self.laser:
      painter.drawPoint( QtCore.QPointF( p[0], p[1] ) )

  #-----------------------------------------------------------------------------

  def drawSteering( self, painter ):
    painter.setPen( QtGui.QColor( 0, 0, 255 ) )
    self.drawSteeringArc( painter, self.steeringAngle )
    painter.setPen( QtGui.QColor( 255, 0, 0 ) )
    self.drawSteeringArc( painter, self.commandSteeringAngle )
    painter.setPen( QtGui.QColor( 128, 128, 128 ) )
    p1 = self.drawClearArc( painter, self.steeringAngle, self.wheelDistance / 2.0 )
    painter.setPen( QtGui.QColor( 128, 128, 128 ) )
    p2 = self.drawClearArc( painter, self.steeringAngle, -self.wheelDistance / 2.0 )
    painter.setPen( QtGui.QColor( 128, 128, 128 ) )
    painter.drawLine( QtCore.QLineF( p1[0], p1[1], p2[0], p2[1] ) )
    
  #-----------------------------------------------------------------------------
  
  def drawArc( self, painter, x, y, radius, a1, a2, center = True ):
    x1 = x + radius * cos( a1 )
    y1 = y + radius * sin( a1 )
    x2 = x + radius * cos( a1 + a2 )
    y2 = y + radius * sin( a1 + a2 )
    if radius < 40 or center:
      rect = QtCore.QRectF( x - radius, y - radius, 2 * radius, 2 * radius )
      painter.drawArc( rect, -a1 * 2880 / pi, -a2 * 2880 / pi )
    else:
      painter.drawLine( QtCore.QLineF( x1, y1, x2, y2 ) )

    painter.setPen( QtGui.QColor( 0, 0, 255 ) )
    p = QtGui.QPainterPath()
    p.addEllipse( QtCore.QRectF( x1 - 0.2, y1 - 0.2, 0.4, 0.4 ) )
    painter.drawPath( p )
    
    painter.setPen( QtGui.QColor( 255, 0, 0 ) )
    p = QtGui.QPainterPath()
    p.addEllipse( QtCore.QRectF( x2 - 0.2, y2 - 0.2, 0.4, 0.4 ) )
    painter.drawPath( p )
    return x2, y2

  #-----------------------------------------------------------------------------

  def drawSteeringArc( self, painter, steeringAngle ):
    if steeringAngle != 0:
      sign   = -steeringAngle / abs( steeringAngle  )
      radius = sign * sqrt( ( self.axesDistance / sin( steeringAngle ) )**2 - self.axesDistance**2 )
    else:
      radius = 400.0
      
    if radius < 0:
      a1 = 0
      a2 = pi / 2.0
    else:
      a1 = -pi
      a2 = -pi / 2.0
    self.drawArc( painter, radius, 0, abs( radius ), a1, a2 )
    
  #-----------------------------------------------------------------------------

  def drawClearArc( self, painter, steeringAngle, offset  ):
    if steeringAngle != 0:
      sign   = -steeringAngle / abs( steeringAngle  )
      radius = sign * sqrt( ( self.axesDistance / sin( steeringAngle ) )**2 - self.axesDistance**2 )
    else:
      radius = 400.0      

    if radius < 0:
      a1 = atan2( self.laserDistance, abs( radius + offset ) )
      nradius = sqrt( ( radius + offset ) ** 2 + self.laserDistance ** 2 )
      a2 = self.securityDistance / abs( nradius )
    else:
      a1 = atan2( self.laserDistance, -abs( radius + offset ) )
      nradius = sqrt( ( radius + offset ) ** 2 + self.laserDistance ** 2 )
      a2 = -self.securityDistance / abs( nradius )
    return self.drawArc( painter, radius, 0, nradius, a1, a2, False )
    
  #-----------------------------------------------------------------------------

  def drawGlobalPoint( self, painter, x, y ):
    dx = x - self.robotX
    dy = y - self.robotY
    r  = sqrt( dx ** 2 + dy ** 2 )
    a  = atan2( dy, dx ) - self.robotTheta
    tx = r * cos( a )
    ty = r * sin( a )
    painter.drawEllipse( QtCore.QRectF( -ty - 0.4, tx - 0.4, 0.2, 0.2 ) )
  
  #-----------------------------------------------------------------------------

  def drawStatusMessage( self, painter, message ):
    painter.scale( 1.5, 1.5 )
    painter.drawText( QtCore.QPoint( 10, 30 ), QtCore.QString( message ) )
    painter.scale( 1 / 1.5, 1 / 1.5 )
  
  #-----------------------------------------------------------------------------

  def drawNext( self, painter ):
    painter.setPen( QtCore.Qt.red )
    self.drawGlobalPoint( painter, self.nextX, self.nextY )

  #-----------------------------------------------------------------------------

  def paintEvent( self, event ):
    self.lock.lockForRead()
    painter = QtGui.QPainter()
    painter.begin( self )
    painter.setRenderHint( QtGui.QPainter.Antialiasing )
    if self.status == 0:
      painter.fillRect( event.rect(), QtGui.QBrush( QtCore.Qt.white ) )
      self.drawStatusMessage( painter, self.statusMessage )
    elif self.status == 1:
      painter.fillRect( event.rect(), QtGui.QBrush( QtCore.Qt.yellow ) )
      self.drawStatusMessage( painter, self.statusMessage )
    elif self.status == 2:
      painter.fillRect( event.rect(), QtGui.QBrush( QtCore.Qt.red ) )
      self.drawStatusMessage( painter, self.statusMessage )
    painter.translate( self.width() / 2.0, self.height() )
    if self.width() > self.height():
      ar = 1.0 * self.height() / self.width()
      painter.scale( ar * self.scale, -self.scale )
    else:
      ar = 1.0 * self.width() / self.height()
      painter.scale( self.scale, -self.scale * ar )
    self.drawSteering( painter )
    self.drawCar( painter )
    #self.drawNext( painter )
    painter.translate( 0, self.laserDistance )
    self.drawLaserPoints( painter )
    painter.end()
    self.lock.unlock()

