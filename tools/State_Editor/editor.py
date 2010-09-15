#!/usr/bin/env python

import sys
from PyQt4 import QtCore, QtGui
import tilemap, tilebar


class MainWindow(QtGui.QMainWindow):
    """Holds all components of app and specifies how they interact."""
    def __init__(self, parent = None):
        """Sets up components of application."""
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
        QtCore.QObject.connect(self.tileBar, QtCore.SIGNAL('selectedTile'),
            self.tileMap.setSelection)

        mapView = QtGui.QGraphicsView(self.tileMap)
        tileBarView = QtGui.QGraphicsView(self.tileBar)

        loadTiles = tilebar.LoadTilesButton()
        QtCore.QObject.connect(loadTiles, QtCore.SIGNAL('selectedFile'),
            self.tileBar.loadTiles)

        setMapDims = tilemap.SetMapDimButton()
        QtCore.QObject.connect(setMapDims, QtCore.SIGNAL('gotDims'),
            self.tileMap.setDims)

        fill = tilemap.FillButton()
        QtCore.QObject.connect(fill, QtCore.SIGNAL('fill'), self.tileMap.fill)

        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(setMapDims)
        hbox.addWidget(fill)
        hbox.addWidget(mapView)
        hbox.addWidget(tileBarView)
        hbox.addWidget(loadTiles)

        mainWidget = QtGui.QWidget()
        mainWidget.setLayout(hbox)
        mainWidget.setMouseTracking(True)

        self.setCentralWidget(mainWidget)


app = QtGui.QApplication(sys.argv)
mainWindow = MainWindow()
mainWindow.show()
sys.exit(app.exec_())
