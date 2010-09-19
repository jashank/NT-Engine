#!/usr/bin/env python

import sys
from PyQt4 import QtCore, QtGui
import objbar, tilebar, tilemap


class MainWindow(QtGui.QMainWindow):
    """Holds all components of app and connects their interactions."""
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

        self._createComponents()
        self._connectComponents()
        self._layoutComponents()

        mainWidget = QtGui.QWidget()
        mainWidget.setLayout(self._layout)
        mainWidget.setMouseTracking(True)
        self.setCentralWidget(mainWidget)

    def _createComponents(self):
        """Creates components application will hold."""
        # Object Bar
        self._objBar = objbar.ObjectBar()
        self._objView = QtGui.QGraphicsView(self._objBar)
        self._loadObjects = objbar.LoadObjectsButton()
        self._clearObjects = objbar.ClearButton()

        # Tile/Object Map
        self._tileMap = tilemap.TileMap()
        self._mapView = QtGui.QGraphicsView(self._tileMap)
        self._setMapDims = tilemap.SetMapDimButton()
        self._fill = tilemap.FillButton()

        # Tile Bar
        self._tileBar = tilebar.TileBar()
        self._tileBarView = QtGui.QGraphicsView(self._tileBar)
        self._loadTiles = tilebar.LoadTilesButton()

    def _connectComponents(self):
        """Connects interactions between components."""
        # Object Bar - Buttons 
        QtCore.QObject.connect(self._loadObjects,
            QtCore.SIGNAL('selectedFiles'), self._objBar.loadObjects)
        QtCore.QObject.connect(self._clearObjects, QtCore.SIGNAL('clear'),
            self._objBar.clear)

        # Tile/Object Map - Buttons
        QtCore.QObject.connect(self._setMapDims, QtCore.SIGNAL('gotDims'),
            self._tileMap.setDims)
        QtCore.QObject.connect(self._fill, QtCore.SIGNAL('fill'),
            self._tileMap.fill)

        # Tile Bar - Buttons
        QtCore.QObject.connect(self._loadTiles, QtCore.SIGNAL('selectedFile'),
            self._tileBar.loadTiles)

        # Bars -> Map
        QtCore.QObject.connect(self._tileBar, QtCore.SIGNAL('selectedTile'),
            self._tileMap.setSelectionTile)
        QtCore.QObject.connect(self._objBar, QtCore.SIGNAL('selectedObject'),
            self._tileMap.setSelectionObject)

        # Bars <-> Bars
        QtCore.QObject.connect(self._tileBar, QtCore.SIGNAL('selectedTile'),
            self._objBar.unselect)
        QtCore.QObject.connect(self._objBar, QtCore.SIGNAL('selectedObject'),
            self._tileBar.unselect)

    def _layoutComponents(self):
        """Layout components onto a grid."""
        self._layout = QtGui.QGridLayout()

        # Object Bar Area
        self._layout.addWidget(self._objView, 0, 0, 1, 2)
        self._layout.addWidget(self._loadObjects, 1, 0)
        self._layout.addWidget(self._clearObjects, 1, 1)

        # Tile/Object Map Area
        self._layout.addWidget(self._mapView, 0, 2, 1, 2)
        self._layout.addWidget(self._setMapDims, 1, 2)
        self._layout.addWidget(self._fill, 1, 3)

        # Tile Bar Area
        self._layout.addWidget(self._tileBarView, 0, 4, 1, 2)
        self._layout.addWidget(self._loadTiles, 1, 4)


app = QtGui.QApplication(sys.argv)
mainWindow = MainWindow()
mainWindow.show()
sys.exit(app.exec_())
