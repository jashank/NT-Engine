#!/usr/bin/env python

import sys
from PyQt4 import QtCore, QtGui
import tilemap, tilebar


class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent = None):
        QtGui.QMainWindow.__init__(self, parent)

        self.setWindowTitle('NT State Editor')

        winWidth = 1024
        winHeight = 768

        screen = QtGui.QDesktopWidget().availableGeometry()
        screenCenterX = (screen.width() - winWidth) / 2
        screenCenterY = (screen.height() - winHeight) / 2
        self.setGeometry(screenCenterX, screenCenterY, winWidth, winHeight)

        self.tileMap = tilemap.TileMap()
        self.tileBar = tilebar.TileBar()

        mapView = QtGui.QGraphicsView(self.tileMap)
        tileBarView = QtGui.QGraphicsView(self.tileBar)

        button = tilebar.LoadTilesButton()
        QtCore.QObject.connect(button, QtCore.SIGNAL('selectedFile'),
            self.tileBar.loadTiles)

        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(mapView)
        hbox.addWidget(tileBarView)
        hbox.addWidget(button)

        mainWidget = QtGui.QWidget()
        mainWidget.setLayout(hbox)

        self.setCentralWidget(mainWidget)


app = QtGui.QApplication(sys.argv)
mainWindow = MainWindow()
mainWindow.show()
sys.exit(app.exec_())
