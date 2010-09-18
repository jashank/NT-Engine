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

        self._tileSize = QtGui.QLineEdit('0')
        self._mapWidth = QtGui.QLineEdit('0')
        self._mapHeight = QtGui.QLineEdit('0')

        ok = QtGui.QPushButton()
        ok.setText('OK')
        ok.connect(ok, QtCore.SIGNAL('clicked()'), self.okPress)

        sizeBox = QtGui.QHBoxLayout()
        sizeBox.addWidget(sizeLabel)
        sizeBox.addWidget(self._tileSize)

        widthBox = QtGui.QHBoxLayout()
        widthBox.addWidget(widthLabel)
        widthBox.addWidget(self._mapWidth)

        heightBox = QtGui.QHBoxLayout()
        heightBox.addWidget(heightLabel)
        heightBox.addWidget(self._mapHeight)

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
        size = int(self._tileSize.text())
        width = int(self._mapWidth.text())
        height = int(self._mapHeight.text())

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


class FillButton(QtGui.QPushButton):
    """When pressed, fills map with selected item.

    SIGNALS: 'fill' -- emitted by 'emitFill'

    """
    def __init__(self, parent = None):
        """Initializes click functionality."""
        QtGui.QPushButton.__init__(self, parent)

        self.setText('Fill')

        self.connect(self, QtCore.SIGNAL('clicked()'), self.emitFill)

    def emitFill(self):
        """Called when button is clicked. Emits 'fill' signal."""
        self.emit(QtCore.SIGNAL('fill'))


class TileMap(QtGui.QGraphicsScene):
    """Grid for user to map tiles to a map for an NT State."""
    def __init__(self, parent = None):
        """Initializes members to starting values."""
        QtGui.QGraphicsScene.__init__(self, parent)

        #Maps points to Tiles and Objects
        self._mapping = dict()

        # in tiles
        self._mapWidth = 0
        self._mapHeight = 0

        self._mousePressed = False

        # Current Object or Tile selected for mapping
        self._selection = None

        # size == dimensions of tile, i.e. (size x size)
        self._tileSize = 0

        self._zValLine = 1

    def fill(self):
        if self._selection:
            self._mapping.clear()
            for item in self.items():
                if item.zValue() != self._zValLine:
                    self.removeItem(item)

            for i in range(0, self._mapWidth):
                for j in range(0, self._mapHeight):
                    point = QtCore.QPoint(i, j)
                    self._mapping[point] = self._selection

                    pos = QtCore.QPointF(i * self._tileSize, j * self._tileSize)
                    pixmap = QtGui.QGraphicsPixmapItem(
                        self._selection.pixmap().copy())
                    pixmap.setPos(pos)

                    self.addItem(pixmap)

    def mousePressEvent(self, event):
        """Sets mouse pressed to true if left button is pressed.

        Overrides mousePressEvent in QGraphicsScene. Also calls mouseMoveEvent
        to perform action.

        """
        if event.button() == QtCore.Qt.LeftButton:
            self._mousePressed = True
            self.mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        """Sets mouse pressed to false if left button is pressed.

        Overrides mouseReleaseEvent in QGraphicsScene.

        """
        if event.button() == QtCore.Qt.LeftButton:
            self._mousePressed = False

    def mouseMoveEvent(self, event):
        """Maps selected item (if any) to location on grid under cursor.

        Only called if mouse is currently pressed. Overrides mouseMoeveEvent
        in QGraphicsScene.

        """
        if self._mousePressed:
            pos = event.scenePos()
            posX = pos.x()
            posY = pos.y()

            inGrid = (posX >= 0 and posX < self._mapWidth * self._tileSize and
                      posY >= 0 and posY < self._mapHeight * self._tileSize)

            if self._selection and inGrid:

                x = int(pos.x() / self._tileSize)
                y = int(pos.y() / self._tileSize)
                point = QtCore.QPoint(x, y)

                image = self.itemAt(pos)
                if (self._mapping.get(point) != self._selection and
                    (image == None or image.zValue() != self._zValLine)):

                    self.removeItem(image)

                    self._mapping[point] = self._selection

                    # only put pixmap on view, store actual item internally
                    pixmap = QtGui.QGraphicsPixmapItem(
                        self._selection.pixmap().copy())
                    pixmap.setPos(self._tileSize * x, self._tileSize * y)
                    self.addItem(pixmap)

    def setDims(self, tileSize, mapWidth, mapHeight):
        """Sets up grid given dimensions passed.

        Arguments: tileSize -- size of a tile (i.e. if 48, then tile is 48x48).
                   mapWidth -- width of map in tiles.
                   mapHeight -- height of map in tiles.

        Dimensions are checked to make sure they are okay.

        """
        if (tileSize > 0 and mapWidth >= 0 and mapHeight >= 0):
            self._tileSize = tileSize
            self._mapWidth = mapWidth
            self._mapHeight = mapHeight

            self._mapping.clear()
            self.clear()

            gridWidth = tileSize * mapWidth
            gridHeight = tileSize * mapHeight

            # lines have z value of 1 to be rendered over tiles
            for i in range(0, mapWidth + 1):
                x = i * tileSize
                line = QtGui.QGraphicsLineItem(x, 0, x, gridHeight)
                line.setZValue(self._zValLine)
                self.addItem(line)

            for i in range(0, mapHeight + 1):
                y = i * tileSize
                line = QtGui.QGraphicsLineItem(0, y, gridWidth, y)
                line.setZValue(self._zValLine)
                self.addItem(line)

    def setSelection(self, selection):
        """Sets selection to QGraphicsItem passed."""
        self._selection = selection


