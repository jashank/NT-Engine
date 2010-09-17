#!/usr/bin/env python

import sys
from PyQt4 import QtCore, QtGui
import objbar, tilemap, tilebar


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

        self.objBar = objbar.ObjectBar()
        self.tileMap = tilemap.TileMap()
        self.tileBar = tilebar.TileBar()
        QtCore.QObject.connect(self.tileBar, QtCore.SIGNAL('selectedTile'),
            self.tileMap.setSelection)

        objView = QtGui.QGraphicsView(self.objBar)
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

        loadObjects = objbar.LoadObjectsButton()
        QtCore.QObject.connect(loadObjects, QtCore.SIGNAL('selectedFiles'),
            self.objBar.loadObjects)

        layout = QtGui.QGridLayout()
        layout.addWidget(mapView, 0, 0, 2, 2)
        layout.addWidget(setMapDims, 2, 0)
        layout.addWidget(fill, 2, 1)
        layout.addWidget(tileBarView, 0, 2, 2, 1)
        layout.addWidget(loadTiles, 2, 2)
        layout.addWidget(objView, 4, 5)
        layout.addWidget(loadObjects, 5, 5)

        mainWidget = QtGui.QWidget()
        mainWidget.setLayout(layout)
        mainWidget.setMouseTracking(True)

        self.setCentralWidget(mainWidget)


app = QtGui.QApplication(sys.argv)
mainWindow = MainWindow()
mainWindow.show()
sys.exit(app.exec_())
