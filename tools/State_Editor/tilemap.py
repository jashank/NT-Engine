#!/usr/bin/env python

from PyQt4 import QtCore, QtGui


class MapDialog(QtGui.QDialog):
    """Window for entering dimensions for the tile map.

    SIGNALS: 'gotDims', size, width, height -- emitted from 'okPress'

    """
    def __init__(self, parent = None):
        """Sets up components (labels and input areas)."""
        QtGui.QDialog.__init__(self, parent)

        self.setModal(True)
        self.setWindowTitle('Enter tile dimensions')

        sizeLabel = QtGui.QLabel('Size of a tile in pixels:')
        widthLabel = QtGui.QLabel('Width of map in tiles:')
        heightLabel = QtGui.QLabel('Height of map in tiles:')

        self.tileSize = QtGui.QLineEdit('0')
        self.mapWidth = QtGui.QLineEdit('0')
        self.mapHeight = QtGui.QLineEdit('0')

        ok = QtGui.QPushButton()
        ok.setText('OK')
        ok.connect(ok, QtCore.SIGNAL('clicked()'), self.okPress)

        sizeBox = QtGui.QHBoxLayout()
        sizeBox.addWidget(sizeLabel)
        sizeBox.addWidget(self.tileSize)

        widthBox = QtGui.QHBoxLayout()
        widthBox.addWidget(widthLabel)
        widthBox.addWidget(self.mapWidth)

        heightBox = QtGui.QHBoxLayout()
        heightBox.addWidget(heightLabel)
        heightBox.addWidget(self.mapHeight)

        layout = QtGui.QVBoxLayout()
        layout.addLayout(sizeBox)
        layout.addLayout(widthBox)
        layout.addLayout(heightBox)
        layout.addWidget(ok)

        self.setLayout(layout)

    def okPress(self):
        """Emits 'gotDims', size, width, height when OK is pressed.

        Note that this function tries to convert the text in the line edit
        fields to integers, and an exception will be thrown if the text isn't
        a valid integer.

        Closes dialog window.

        """
        size = int(self.tileSize.text())
        width = int(self.mapWidth.text())
        height = int(self.mapHeight.text())

        self.emit(QtCore.SIGNAL('gotDims'), size, width, height)
        self.done(1)


class SetMapDimButton(QtGui.QPushButton):
    """Opens dialog window for entering map dimensions when pressed.

    SIGNALS: 'gotDims', tileSize, mapWidth, mapHeight --
        emitted from 'emitDims'.

    """
    def __init__(self, parent = None):
        """Sets button text and makes dimension dialog open when clicked."""
        QtGui.QPushButton.__init__(self, parent)
        self.setText('Map Dimensions')
        self.connect(self, QtCore.SIGNAL('clicked()'), self.openMapDialog)

    def openMapDialog(self):
        """Opens the map dimension dialog.

        Will retrieve map dimensions from dimension dialog if entered and emit
        them in a 'gotDims' SIGNAL.

        """
        dialog = MapDialog()
        dialog.connect(dialog, QtCore.SIGNAL('gotDims'), self.emitDims)
        dialog.exec_()

    def emitDims(self, tileSize, mapWidth, mapHeight):
        """Called if user enters values in dimensions dialog.

        SIGNALS: 'gotDims', tileSize, mapWidth, mapHeight -- contains
            specifications for tile size, map width, and map height.

        Arguments: tileSize -- size of a tile (i.e. if 48, then tile is 48x48).
                   mapWidth -- width of map in tiles.
                   mapHeight -- height of map in tiles.

        """
        self.emit(QtCore.SIGNAL('gotDims'), tileSize, mapWidth, mapHeight)


class TileMap(QtGui.QGraphicsScene):
    """Grid for user to map tiles to a map for an NT State."""
    def __init__(self, parent = None):
        """Default initialization."""
        QtGui.QGraphicsScene.__init__(self, parent)

    def setDims(self, tileSize, mapWidth, mapHeight):
        """Sets up grid given dimensions passed.

        Arguments: tileSize -- size of a tile (i.e. if 48, then tile is 48x48).
                   mapWidth -- width of map in tiles.
                   mapHeight -- height of map in tiles.

        """
        for item in self.items():
            self.removeItem(item)

        gridWidth = tileSize * mapWidth
        gridHeight = tileSize * mapHeight

        for i in range(0, mapWidth + 1):
            x = i * tileSize
            self.addLine(x, 0, x, gridHeight)

        for i in range(0, mapHeight + 1):
            y = i * tileSize
            self.addLine(0, y, gridWidth, y)


