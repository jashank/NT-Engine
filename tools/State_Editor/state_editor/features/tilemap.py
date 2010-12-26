"""
(C) Copyright 2010-2011 Alex Shipley

This file is part of the NT State Editor.

The NT State Editor is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The NT State Editor is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the NT State Editor.  If not, see <http://www.gnu.org/licenses/>.
"""

#!/usr/bin/env python


from abc import ABCMeta, abstractmethod
from collections import defaultdict, deque
from copy import deepcopy
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
    """Holds tile position and an optional path for unique identification."""
    def __init__(self, x, y, path = "", parent = None):
        """Initializes parent and tile member with arguments passed."""
        QtGui.QGraphicsPixmapItem.__init__(self, parent)
        self._tile = QtCore.QPoint(x, y)
        self._path = path

    def getTile(self):
        """Returns tile position as a QPoint."""
        return self._tile

    def getPath(self):
        """Returns path set for this pixmap."""
        return self._path


class MapAction(object):
    """Abstract base class for actions that can occur on map."""
    __metaclass__ = ABCMeta

    @abstractmethod
    def undo(self): pass

    @abstractmethod
    def redo(self): pass


class TilePlace(MapAction):
    """Action representing tile placement on map."""
    def __init__(self, tilemap, oldTile, newTile, pos):
        """Action initialized with tile that was placed and its position.

        Arguments: tilemap - reference to tilemap in editor
                   oldTile - tile that was replaced by newTile (can be None)
                   newTile - tile selected at time of placement
                   pos - position of where mouse was pressed on placement
        """
        self._map = tilemap
        self._oldTile = oldTile
        self._newTile = newTile
        self._pos = pos

    def undo(self):
        pass
        """
        self._map.removePlacement(self._pos)
        if (self._oldTile):
            self._map.setSelectionTile(self._oldTile)
            point = self._map.posToPoint(self._pos)
            self._map.placeTile(self._pos, point.x(), point.y())
        """

    def redo(self):
        pass
        """
        point = self._map.posToPoint(self._pos)
        self._map.setSelectionTile(self._newTile)
        self._map.placeTile(self._pos, point.x(), point.y())
        """


class TileMap(QtGui.QGraphicsScene):
    """Grid for user to map tiles to a map for an NT State."""
    def __init__(self, parent = None):
        """Initializes members to starting values."""
        QtGui.QGraphicsScene.__init__(self, parent)

        # Map coordinates to tiles and lists of objects
        self._tileMapping = dict()
        self._objMapping = defaultdict(list)

        # Stores last 100 actions that have taken place on the tile map
        # from earliest to most recent
        self._actions = deque(maxlen = 100)

        # Stores last 100 undos that have taken place on tile map (for
        # (redoing). Cleared anytime a new action occurs.
        self._undos = deque(maxlen = 100)

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

    def undo(self):
        """Undoes last action performed."""
        if (len(self._actions) > 0):
            action = self._actions.pop()
            action.undo()
            self._undos.append(action)

    def redo(self):
        """Redoes last undo."""
        if (len(self._redos) > 0):
            action = self._redos.pop()
            action.redo()
            self._actions.append(action)

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
                    pos = self.pointToPos(QtCore.QPoint(i, j))
                    # +1 to avoid grid lines
                    pos.setX(pos.x() + 1)
                    pos.setY(pos.y() + 1)

                    if self._objSelected:
                        self.placeObjectOnTile(i, j)
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

        Only called if mouse is currently pressed. Overrides mouseMoveEvent
        in QGraphicsScene.

        """
        pos = event.scenePos()

        if self._mousePressed == QtCore.Qt.LeftButton:
            posX = pos.x()
            posY = pos.y()

            inGrid = (posX >= 0 and posX < self._mapWidth * self._tileSize and
                      posY >= 0 and posY < self._mapHeight * self._tileSize)

            if self._selection != None and inGrid:

                point = self.posToPoint(pos)

                # Action additions are handled here so that actions can undo
                # redo safely without creating new actions
                if self._objSelected:
                    self.placeObjectAtPos(posX, posY)
                elif self._tileSelected:
                    (placed, oldTile) = \
                        self.placeTile(pos, point.x(), point.y())
                    if placed:
                        tp = TilePlace(self, oldTile, self._selection, pos)
                        self._addAction(tp)


        elif self._mousePressed == QtCore.Qt.RightButton:
            self.removePlacement(pos)

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
                        self.removePlacementsAt(x, y)

            if self._mapHeight > mapHeight:
                for x in range(0, self._mapWidth):
                    for y in range(mapHeight, self._mapHeight):
                        self.removePlacementsAt(x, y)

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

    def placeObjectAtPos(self, posX, posY, obj = None):
        """Places obj on tile located at scene position passed.

        If no obj is passed, defaults to current selection (if applicable).
        """
        (tX, tY) = self._posToTile(posX, posY)
        self._placeObject(posX, posY, tX, tY, obj)

    def placeObjectOnTile(self, tX, tY, obj = None):
        """Places obj on tile coordinate passed.

        If no obj is passed, defaults to current selection (if applicable).
        """
        (posX, posY) = self._tileToPos(tX, tY)
        self._placeObject(posX, posY, tX, tY, obj)

    def placeTile(self, pos, x, y):
        """Places tile at grid coordinate (x,y).

        Arguments: pos -- Position of cursor relative to scene (QPointF)
                   x -- x coordinate on grid
                   y -- y coordinate on grid

        Returns: 2 values. First is True if tile was placed, False if not.
                 Second is the tile replaced, None if spot was blank.
        """

        images = self.items(pos)

        if (self._hasLine(images)):
            return (False, None)

        key = self._tileToKey(x, y)
        if self._tileMapping.get(key) != self._selection:
            tiles = [t for t in images if t.zValue() == self._zValTile]
            if len(tiles) > 0:
                self.removeItem(tiles[0])

            # Store actual tile internally, only placing its image on the grid
            oldTile = self._tileMapping.get(key)
            self._tileMapping[key] = self._selection

            tileImg = TiledPixmap(x, y)
            tileImg.setPixmap(self._selection.pixmap().copy())
            tileImg.setPos(self._tileSize * x, self._tileSize * y)
            tileImg.setZValue(self._zValTile)
            self.addItem(tileImg)

            return (True, oldTile)

        return (False, None)

    def removePlacement(self, pos):
        """Removes the top item under cursor from the grid and internally.

        Arguments: pos -- Position of cursor relative to scene

        """
        images = self.items(pos)

        if len(images) > 0:
            if (self._hasLine(images)):
                return

            img = images[0]
            tileCoord = img.getTile()
            self.removeItem(img)
            key = self._tileToKey(tileCoord.x(), tileCoord.y())

            objs = self._objMapping.get(key)
            if objs != None and len(objs) > 0:
                for o in objs:
                    # Two of the same object can't be on the same tile
                    if o.getPath() == img.getPath():
                        objs.remove(o)
                        return

            tile = self._tileMapping.get(key)
            if tile:
                del self._tileMapping[key]

    def removePlacementsAt(self, x, y):
        """Removes all items and images from coordinates on grid.

        Arguments: x -- x coordinate for removal, relative to tile grid
                   y -- y coordinate for removal, relative to tile grid

        """
        point = QtCore.QPoint(x, y)
        pos = self.pointToPos(point)
        rectArea = QtCore.QRectF(
            pos.x(), pos.y(), self._tileSize, self._tileSize
        )

        key = self._tileToKey(x, y)

        if key in self._tileMapping:
            del self._tileMapping[pointKey]
        if key in self._objMapping:
            del self._objMapping[pointKey]

        for item in self.items(rectArea):
            self.removeItem(item)

    def _placeObject(self, posX, posY, tX, tY, obj = None):
        """Places obj at tile coordinate passed.

        Does nothing under the following conditions:
            If no obj is passed then current editor selection is used. If that
            selection is not an object, returns.

            If scene position passed is on a grid line.

            If obj has already been placed on the tile.

        Arguments: posX - x scene coordinate
                   posY - y scene coordinate
                   tX - x tile coordinate
                   tY - y tile coordinate
                   obj - object to place, will try to default to the
                         selected object if none was passed.
        """
        if (not obj):
            if (self._objSelected):
                obj = self._selection
            else:
                return

        pos = QtCore.QPointF(posX, posY)
        images = self.items(pos)

        if (self._hasLine(images)):
            return

        key = self._tileToKey(tX, tY)
        # Don't allow multiples of the same object on a tile
        objs = self._objMapping.get(key)
        if objs != None:
            clone = [o for o in objs if o.getPath() == obj.getPath()]
            if len(clone) > 0:
                return

        # Store actual object internally, only placing its image on the grid
        self._objMapping[key].append(obj)

        objImg = TiledPixmap(tX, tY, obj.getPath())
        objImg.setPixmap(obj.pixmap().copy())

        # Take object's height into account
        (posX, posY) = (self._tileToPos(tX, tY))
        posY = self._tileSize * tY
        if objImg.pixmap().height() > self._tileSize:
            posY -= (objImg.pixmap().height() - self._tileSize)
        objImg.setPos(posX, posY)

        objImg.setZValue(self._zValObj)
        self.addItem(objImg)

    def _tileToKey(self, tX, tY):
        """Given tile coord, returns string key for use in internal maps."""
        return str(tX) + "," + str(tY)

    def _posToTile(self, posX, posY):
        """Returns tile coordinate based on scene position passed.

        Tile coordinates are stored in a tuple: (x,y)
        """
        tX = int(posX / self._tileSize)
        tY = int(posY / self._tileSize)
        return (tX, tY)

    def _tileToPos(self, tX, tY):
        """Returns QGraphicsScene position based on tile coordinate passed.

        Unless there is no room between grid lines, position is guaranteed
        to not be on a grid line.
        """
        # Add 1 to get off of grid line
        posX = tX * self._tileSize + 1
        posY = tY * self._tileSize + 1
        return (posX, posY)

    def _hasLine(self, images):
        """Returns true if there is a grid line in list of images passed."""
        lines = [l for l in images if l.zValue() == self._zValLine]
        if len(lines) > 0:
            return True

        return False

    def _addAction(self, action):
        """Adds action to action deque.

        Clears out undos deque because a redo shouldn't be able to take
        place after a new action has occured.
        """
        self._actions.append(action)
        self._undos.clear()


