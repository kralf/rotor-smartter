# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '../ui/elrob_monitor_ui.ui'
#
# Created: Sun Jul  5 13:06:15 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(388, 737)
        MainWindow.setDockNestingEnabled(True)
        self.centralwidget = QtGui.QWidget(MainWindow)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.centralwidget.sizePolicy().hasHeightForWidth())
        self.centralwidget.setSizePolicy(sizePolicy)
        self.centralwidget.setObjectName("centralwidget")
        self.resetButton = QtGui.QPushButton(self.centralwidget)
        self.resetButton.setGeometry(QtCore.QRect(239, 670, 71, 28))
        self.resetButton.setDefault(False)
        self.resetButton.setFlat(False)
        self.resetButton.setObjectName("resetButton")
        self.saveButton = QtGui.QPushButton(self.centralwidget)
        self.saveButton.setGeometry(QtCore.QRect(310, 670, 71, 28))
        self.saveButton.setObjectName("saveButton")
        self.navigationPlot = NavigationPlot(self.centralwidget)
        self.navigationPlot.setGeometry(QtCore.QRect(11, 341, 360, 322))
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.navigationPlot.sizePolicy().hasHeightForWidth())
        self.navigationPlot.setSizePolicy(sizePolicy)
        self.navigationPlot.setObjectName("navigationPlot")
        self.localizationPlot = LocalizationPlot(self.centralwidget)
        self.localizationPlot.setGeometry(QtCore.QRect(11, 14, 369, 323))
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.localizationPlot.sizePolicy().hasHeightForWidth())
        self.localizationPlot.setSizePolicy(sizePolicy)
        self.localizationPlot.setObjectName("localizationPlot")
        self.yawDial = QtGui.QDial(self.localizationPlot)
        self.yawDial.setEnabled(True)
        self.yawDial.setGeometry(QtCore.QRect(-2, 276, 51, 50))
        self.yawDial.setMaximum(360)
        self.yawDial.setProperty("value", QtCore.QVariant(0))
        self.yawDial.setSliderPosition(0)
        self.yawDial.setTracking(False)
        self.yawDial.setOrientation(QtCore.Qt.Vertical)
        self.yawDial.setWrapping(True)
        self.yawDial.setNotchTarget(10.0)
        self.yawDial.setNotchesVisible(False)
        self.yawDial.setObjectName("yawDial")
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QObject.connect(self.resetButton, QtCore.SIGNAL("clicked()"), self.localizationPlot.reset)
        QtCore.QObject.connect(self.saveButton, QtCore.SIGNAL("clicked()"), self.localizationPlot.save)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "ElrobMonitor", None, QtGui.QApplication.UnicodeUTF8))
        self.resetButton.setText(QtGui.QApplication.translate("MainWindow", "Reset", None, QtGui.QApplication.UnicodeUTF8))
        self.saveButton.setText(QtGui.QApplication.translate("MainWindow", "Save", None, QtGui.QApplication.UnicodeUTF8))

from elrob_monitor_widgets import NavigationPlot, LocalizationPlot
