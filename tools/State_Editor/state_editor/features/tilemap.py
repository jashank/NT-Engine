#!/usr/bin/env python


from collections import defaultdict
from PyQt4 import QtCore, QtGui


class MapDimsForm(QtGui.QDialog):
    """Window for entering dimensions for the tile map.

    SIGNALS: 'gotDims', size, width, height -- emitted from 'okPress'

    """
    def __init__(self, parent = None):
        """Sets up components (labels and input areas)."""
        QtGui.QDialog.__init__(self, parent)

        self.setModal(True)
        self.setWindowTitle('Enter tile dimensions')

        self._tileSize = QtGui.QLineEdit('0')
        self._mapWidth = QtGui.QLineEdit('0')
        self._mapHeight = QtGui.QLineEdit('0')

        ok = QtGui.QPushButton()
        ok.setText('OK')
        ok.connect(ok, QtCore.SIGNAL('clicked()'), self._okPress)

        layout = QtGui.QFormLayout()
        layout.addRow('Size of a tile in pixels:', self._tileSize)
        layout.addRow('Width of map in tiles:', self._mapWidth)
        layout.addRow('Height of map in tiles:', self._mapHeight)
        layout.addRow(ok)
        self.setLayout(layout)

    def setDims(self, tileSize, mapWidth, mapHeight):
        """Used for non-GUI modification of tile map dimensions."""
        self._tileSize.setText(str(tileSize))
        self._mapWidth.setText(str(mapWidth))
        self._mapHeight.setText(str(mapHeight))
        self._okPress()

    def _okPress(self):
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


class SetMapDimsButton(QtGui.QPushButton):
    """Opens dialog window for entering map dimensions when pressed.

    SIGNALS: 'gotDims', tileSize, mapWidth, mapHeight --
        emitted from 'emitDims'.

    """
    def __init__(self, parent = None):
        """Sets button text and makes dimension dialog open when clicked."""
        QtGui.QPushButton.__init__(self, parent)
        self.setText('Map Dimensions')
        self.connect(self, QtCore.SIGNAL('clicked()'), self._openMapDimsForm)

        self._form = MapDimsForm()
        self._form.connect(self._form, QtCore.SIGNAL('gotDims'), self._emitDims)

    def setDims(self, tileSize, mapWidth, mapHeight):
        """Used for non-GUI modification of tile map dimensions."""
        self._form.setDims(tileSize, mapWidth, mapHeight)

    def _openMapDimsForm(self):
        """Opens the map dimensions forms.

        Will retrieve map dimensions from dimension dialog if entered and emit
        them in a 'gotDims' SIGNAL.

        """
        self._form.exec_()

    def _emitDims(self, tileSize, mapWidth, mapHeight):
        """Called if user enters values in dimensions form.

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

        self.connect(self, QtCore.SIGNAL('clicked()'), self._emitFill)

    def _emitFill(self):
        """Called when button is clicked. Emits 'fill' signal."""
        self.emit(QtCore.SIGNAL('fill'))


class TiledPixmap(QtGui.QGraphicsPixmapItem):
    """QGraphicsPixmapItem that holds a tile position."""
    def __init__(self, x = -1, y = -1, parent = None):
        """Initializes parent and tile member with arguments passed."""
        QtGui.QGraphicsPixmapItem.__init__(self, parent)
        self._tile = QtCore.QPoint(x, y)

    def setTile(self, x, y):
        """Sets tile position to integers passed."""
        self._tile.setX(x)
        self._tile.setY(y)

    def getTile(self):
        """Returns tile position as a QPoint."""
        return self._tile


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

        # Button pressed on mouse, None if none pressed 
        self._mousePressed = None

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

    def clearPlacements(self):
        """Clears images from grid and stored items."""
        for i in self.items():
            if i.zValue() != self._zValLine:
                self.removeItem(i)

        self._tileMapping.clear()
        self._objMapping.clear()

    def fill(self):
        """Fills map with selected item."""
        if self._selection != None:

            # Filling with a tile selection clears existing tiles
            if self._tileSelected:
                self._tileMapping.clear()
                for item in self.items():
                    if item.zValue() == self._zValTile:
                        self.removeItem(item)

            for i in range(0, self._mapWidth):
                for j in range(0, self._mapHeight):
                    # +1 to avoid grid lines
                    pos = QtCore.QPointF(i * self._tileSize + 1,
                        j * self._tileSize + 1)

                    if self._objSelected:
                        self.placeObject(pos, i, j)
                    elif self._tileSelected:
                        self.placeTile(pos, i, j)

    def mousePressEvent(self, event):
        """Responds to right and left mouse presses.

        Sets mouse pressed to button pressed, calling mouseMoveEvent to
        take appropriate action.

        """
        if ((event.button() == QtCore.Qt.LeftButton or
             event.button() == QtCore.Qt.RightButton) and
             self._mousePressed == None):
            self._mousePressed = event.button()
            self.mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        """Sets mouse pressed to None when mouse is released.

        Overrides mouseReleaseEvent in QGraphicsScene.

        """
        if event.button() == self._mousePressed:
            self._mousePressed = None

    def mouseMoveEvent(self, event):
        """Maps selected item (if any) to location on grid under cursor.

        Only called if mouse is currently pressed. Overrides mouseMoeveEvent
        in QGraphicsScene.

        """
        pos = event.scenePos()

        if self._mousePressed == QtCore.Qt.LeftButton:
            posX = pos.x()
            posY = pos.y()

            inGrid = (posX >= 0 and posX < self._mapWidth * self._tileSize and
                      posY >= 0 and posY < self._mapHeight * self._tileSize)

            if self._selection != None and inGrid:

                x = int(pos.x() / self._tileSize)
                y = int(pos.y() / self._tileSize)

                if self._objSelected:
                    self.placeObject(pos, x, y)
                elif self._tileSelected:
                    self.placeTile(pos, x, y)

        elif self._mousePressed == QtCore.Qt.RightButton:
            self._removePlacement(pos)

    def setDims(self, tileSize, mapWidth, mapHeight):
        """Sets up grid given dimensions passed.

        This function should only be called when the appropriate signal is
        received from the Set Map Dimensions button. Dimensions are checked to
        make sure they are okay. If items exists in locations that no longer
        exist then they are removed.

        Arguments: tileSize -- size of a tile (i.e. if 48, then tile is 48x48).
                   mapWidth -- width of map in tiles.
                   mapHeight -- height of map in tiles.

        """
        if (tileSize >= 0 and mapWidth >= 0 and mapHeight >= 0 and
           (tileSize != self._tileSize or mapWidth != self._mapWidth or
            mapHeight != self._mapHeight)):

            for line in self.items():
                if line.zValue() == self._zValLine:
                    self.removeItem(line)

            if self._mapWidth > mapWidth:
                for x in range(mapWidth, self._mapWidth):
                    for y in range(0, self._mapHeight):
                        self._removePlacementsAt(x, y)

            if self._mapHeight > mapHeight:
                for x in range(0, self._mapWidth):
                    for y in range(mapHeight, self._mapHeight):
                        self._removePlacementsAt(x, y)

            self._tileSize = tileSize
            self._mapWidth = mapWidth
            self._mapHeight = mapHeight

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

    def currentState(self):
        """Returns all data relevant to map's current state.

        Returns: tile size, map width, map height, tile mapping, object mapping.

        """
        return (self._tileSize, self._mapWidth, self._mapHeight,
                self._tileMapping, self._objMapping)

    def placeObject(self, pos, x, y):
        """Places object at grid coordinate (x,y).

        Arguments: pos -- Position relative to scene of area pressed
                   x -- x coordinate on grid
                   y -- y coordinate on grid
        """
        images = self.items(pos)

        lines = [l for l in images if l.zValue() == self._zValLine]
        if len(lines) > 0:
            return

        point = self._coordToKey(x, y)
        # Don't allow multiples of the same object on a tile
        objs = self._objMapping.get(point)
        if objs != None:
            clone = [o for o in objs if o.getPath() ==
                     self._selection.getPath()]
            if len(clone) > 0:
                return

        # Store actual object internally, only placing its image on the grid
        self._objMapping[point].append(self._selection)

        objImg = TiledPixmap()
        objImg.setPixmap(self._selection.pixmap().copy())
        objImg.setTile(x, y)

        # Take object's height into account
        yPos = self._tileSize * y
        if objImg.pixmap().height() > self._tileSize:
            yPos -= (objImg.pixmap().height() - self._tileSize)
        objImg.setPos(self._tileSize * x, yPos)

        objImg.setZValue(self._zValObj)
        self.addItem(objImg)

    def placeTile(self, pos, x, y):
        """Places tile at grid coordinate (x,y).

        Arguments: pos -- Position of cursor relative to scene (QPointF)
                   x -- x coordinate on grid
                   y -- y coordinate on grid

        """

        images = self.items(pos)
        lines = [l for l in images if l.zValue() == self._zValLine]
        if len(lines) > 0:
            return

        point = self._coordToKey(x, y)
        if self._tileMapping.get(point) != self._selection:
            tile = [t for t in images if t.zValue() == self._zValTile]
            if len(tile) > 0:
                self.removeItem(tile[0])

            # Store actual tile internally, only placing its image on the grid
            self._tileMapping[point] = self._selection

            tileImg = TiledPixmap()
            tileImg.setPixmap(self._selection.pixmap().copy())
            tileImg.setTile(x, y)
            tileImg.setPos(self._tileSize * x, self._tileSize * y)
            tileImg.setZValue(self._zValTile)
            self.addItem(tileImg)

    def _removePlacement(self, pos):
        """Removes the top item under cursor from the grid and internally.

        Arguments: pos -- Position of cursor relative to scene

        """
        images = self.items(pos)

        if len(images) > 0:
            lines = [l for l in images if l.zValue() == self._zValLine]
            if len(lines) > 0:
                return

            tile = images[0].getTile()
            self.removeItem(images[0])
            point = self._coordToKey(tile.x(), tile.y())

            objs = self._objMapping.get(point)
            if objs != None and len(objs) > 0:
                self._objMapping[point].pop()
                return

            tile = self._tileMapping.get(point)
            if tile:
                del self._tileMapping[point]

    def _removePlacementsAt(self, x, y):
        """Removes all items and images from coordinates on grid.

        Arguments: x -- x coordinate for removal, relative to tile grid
                   y -- y coordinate for removal, relative to tile grid

        """
        rectArea = QtCore.QRectF( x * self._tileSize,
                    y * self._tileSize, self._tileSize, self._tileSize)

        point = self._coordToKey(x, y)

        if point in self._tileMapping:
            del self._tileMapping[point]
        if point in self._objMapping:
            del self._objMapping[point]

        for item in self.items(rectArea):
            self.removeItem(item)

    def _coordToKey(self, x, y):
        """Given grid coords, returns key for use in internal mapping dicts.

        Arguments: x -- x coordinate on mapping grid
                   y -- y coordinate on mapping grid

        """
        return str(x) + "," + str(y)


