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

        loadTiles = tilebar.LoadTilesButton()
        QtCore.QObject.connect(loadTiles, QtCore.SIGNAL('selectedFile'),
            self.tileBar.loadTiles)

        setMapDims = tilemap.SetMapDimButton()
        QtCore.QObject.connect(setMapDims, QtCore.SIGNAL('gotDims'),
            self.tileMap.setDims)


        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(setMapDims)
        hbox.addWidget(mapView)
        hbox.addWidget(tileBarView)
        hbox.addWidget(loadTiles)

        mainWidget = QtGui.QWidget()
        mainWidget.setLayout(hbox)

        self.setCentralWidget(mainWidget)


app = QtGui.QApplication(sys.argv)
mainWindow = MainWindow()
mainWindow.show()
sys.exit(app.exec_())
