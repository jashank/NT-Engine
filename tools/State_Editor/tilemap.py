#!/usr/bin/env python


from collections import defaultdict
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

        # Map coordinates to tiles and lists of objects
        self._tileMapping = dict()
        self._objMapping = defaultdict(list)

        # Map dimensions in tiles
        self._mapWidth = 0
        self._mapHeight = 0

        # Whether mouse is pressed on the map
        self._mousePressed = False

        # Whether an object or tile is currently selected
        self._objSelected = False
        self._tileSelected = False

        # Current Object or Tile selected for mapping
        self._selection = None

        # size == dimensions of tile, i.e. (size x size)
        self._tileSize = 0

        # Z values of grid lines, objects, and tiles
        self._zValLine = 2
        self._zValObj = 1
        self._zValTile = 0

    def fill(self):
        if self._selection:
            self._tileMapping.clear()
            for item in self.items():
                if item.zValue() != self._zValLine:
                    self.removeItem(item)

            for i in range(0, self._mapWidth):
                for j in range(0, self._mapHeight):
                    point = str(i) + "," + str(j)
                    self._tileMapping[point] = self._selection

                    pos = QtCore.QPointF(i * self._tileSize, j * self._tileSize)
                    pixmap = QtGui.QGraphicsPixmapItem(
                        self._selection.pixmap().copy())
                    pixmap.setPos(pos)

                    self.addItem(pixmap)

    def mousePressEvent(self, event):
        """Handles mouse press events, responding accordingly.

        On a left button press, the function will call mouseMoveEvent
        to start tile/object placement.

        On a right button press, the function will call _removeItem to
        remove the tile/object under the cursor.

        """
        if event.button() == QtCore.Qt.LeftButton:
            self._mousePressed = True
            self.mouseMoveEvent(event)

        elif event.button() == QtCore.Qt.RightButton:
            pos = event.scenePos()
            self._removeItem(pos)

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
                point = str(x) + "," + str(y)

                if self._objSelected:
                    self._placeObject(pos, x, y, point)
                elif self._tileSelected:
                    self._placeTile(pos, x, y, point)

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

            self._tileMapping.clear()
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

    def setSelectionObject(self, selection):
        """Sets selection to QGraphicsItem passed. Assumes it is Object."""
        self._objSelected = True
        self._tileSelected = False
        self._selection = selection

    def setSelectionTile(self, selection):
        """Sets selection to QGraphicsItem passed. Assumes it is Tile."""
        self._tileSelected = True
        self._objSelected = False
        self._selection = selection

    def _placeObject(self, pos, x, y, point):
        """Places object at grid coordinate (x,y).

        Arguments: pos -- Position relative to scene of area pressed
                   x -- x coordinate on grid
                   y -- y coordinate on grid
                   point -- string representation of coordinates in form "x,y".
                            Needed for storing in actual objects in dictionary.

        """
        images = self.items(pos)

        lines = [l for l in images if l.zValue() == self._zValLine]
        if len(lines) > 0:
            return

        objs = self._objMapping.get(point)
        if objs:
            clone = [o for o in objs if o == self._selection]
            if len(clone) > 0:
                return

        # Store actual object internally, only placing its image on the grid
        self._objMapping[point].append(self._selection)

        objImg = QtGui.QGraphicsPixmapItem(
                self._selection.pixmap().copy())

        # Take object's height into account
        yPos = self._tileSize * y
        if objImg.pixmap().height() > self._tileSize:
            yPos -= (objImg.pixmap().height() - self._tileSize)
        objImg.setPos(self._tileSize * x, yPos)

        objImg.setZValue(self._zValObj)
        self.addItem(objImg)


    def _placeTile(self, pos, x, y, point):
        """Places tile at grid coordinate (x,y).

        Arguments: pos -- Position of cursor relative to scene
                   x -- x coordinate on grid
                   y -- y coordinate on grid
                   point -- string representation of coordinates in form "x,y".
                            Needed for storing in actual objects in dictionary.

        """
        images = self.items(pos)

        lines = [l for l in images if l.zValue() == self._zValLine]
        if len(lines) > 0:
            return

        if self._tileMapping.get(point) != self._selection:
            tile = [t for t in images if t.zValue() == self._zValTile]
            if len(tile) > 0:
                self.removeItem(tile[0])

            # Store actual tile internally, only placing its image on the grid
            self._tileMapping[point] = self._selection

            tileImg = QtGui.QGraphicsPixmapItem(
                    self._selection.pixmap().copy())
            tileImg.setPos(self._tileSize * x, self._tileSize * y)
            tileImg.setZValue(self._zValTile)
            self.addItem(tileImg)

    def _removeItem(self, pos):
        """Removes the top item under cursor from the grid and internally.

        Arguments: pos -- Position of cursor relative to scene

        """
        images = self.items(pos)

        if len(images) > 0:
            lines = [l for l in images if l.zValue() == self._zValLine]
            if len(lines) > 0:
                return

            self.removeItem(images[0])

            x = int(pos.x() / self._tileSize)
            y = int(pos.y() / self._tileSize)
            point = str(x) + "," + str(y)

            objs = self._objMapping.get(point)
            if objs and len(objs) > 0:
                self._objMapping[point].pop()
                return

            tile = self._tileMapping.get(point)
            if tile:
                del self._tileMapping[point]


