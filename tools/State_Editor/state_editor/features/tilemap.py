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
    """Holds optional label for unique identification."""
    def __init__(self, label = "", parent = None):
        QtGui.QGraphicsPixmapItem.__init__(self, parent)
        self._label = label

    def getLabel(self):
        return self._label


class MapAction(object):
    """Abstract base class for actions that can occur on map."""
    __metaclass__ = ABCMeta

    @abstractmethod
    def undo(self): pass

    @abstractmethod
    def redo(self): pass


class TilePlace(MapAction):
    """Action representing tile placement on map."""
    def __init__(self, tilemap, oldTile, newTile, posX, posY):
        """Action initialized with tile that was placed and its position.

        Arguments: tilemap - reference to tilemap in editor
                   oldTile - tile that was replaced by newTile (can be None)
                   newTile - tile selected at time of placement
                   posX - x-coordinate of position of mouse press on placement
                   posY - y-coordinate of position of mouse press on placement
        """
        self._map = tilemap
        self._oldTile = oldTile
        self._newTile = newTile
        self._posX = posX
        self._posY = posY

    def undo(self):
        self._map.removeTileAtPos(self._posX, self._posY)
        if (self._oldTile):
            self._map.placeTileAtPos(self._posX, self._posY, self._oldTile)

    def redo(self):
        self._map.placeTileAtPos(self._posX, self._posY, self._newTile)


class ObjectPlace(MapAction):
    """Action representing object placement on map."""
    def __init__(self, tilemap, obj, tX, tY):
        """Action initialized with Object placed and tile coordinates.

        Arguments: tilemap - reference to tilemap in editor
                   obj - Object that was placed
                   tX - x coordinate of tile it was placed on
                   tY - y coordinate of tile it was placed on
        """
        self._map = tilemap
        self._obj = obj
        self._tX = tX
        self._tY = tY

    def undo(self):
        self._map.removeTopObjectOnTile(self._tX, self._tY)

    def redo(self):
        self._map.placeObjectOnTile(self._tX, self._tY, self._obj)


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
        if (len(self._undos) > 0):
            action = self._undos.pop()
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
                    if self._objSelected:
                        self.placeObjectOnTile(i, j)
                    elif self._tileSelected:
                        self.placeTileOnTile(i, j)

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
        posX = pos.x()
        posY = pos.y()

        if self._mousePressed == QtCore.Qt.LeftButton:
            inGrid = (posX >= 0 and posX < self._mapWidth * self._tileSize and
                      posY >= 0 and posY < self._mapHeight * self._tileSize)

            if self._selection != None and inGrid:

                # Action additions are handled here so that actions can undo
                # redo safely without creating new actions
                if self._objSelected:
                    placed = \
                        self.placeObjectAtPos(posX, posY, self._selection)
                    if placed:
                        (tX, tY) = self._posToTile(posX, posY)
                        op = ObjectPlace(self, self._selection, tX, tY)
                        self._addAction(op)

                elif self._tileSelected:
                    (placed, oldTile) = \
                        self.placeTileAtPos(posX, posY, self._selection)
                    if placed:
                        tp = TilePlace(
                                self,
                                oldTile,
                                self._selection,
                                posX,
                                posY
                             )
                        self._addAction(tp)

        elif self._mousePressed == QtCore.Qt.RightButton:
            self.removeTopAtPos(posX, posY)

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
                        self._clearTile(x, y)

            if self._mapHeight > mapHeight:
                for x in range(0, self._mapWidth):
                    for y in range(mapHeight, self._mapHeight):
                        self._clearTile(x, y)

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
        """Sets selection to QGraphicsItem passed. Assumes it is Object.

        This should NOT be called unless a user actually clicks on an object
        in the Object Bar.
        """
        self._objSelected = True
        self._tileSelected = False
        self._selection = selection

    def setSelectionTile(self, selection):
        """Sets selection to QGraphicsItem passed. Assumes it is Tile.

        This should NOT be called unless a user actually clicks on a tile
        in the Tile Bar.
        """
        self._tileSelected = True
        self._objSelected = False
        self._selection = selection

    def currentState(self):
        """Returns all data relevant to map's current state.

        Returns: tile size, map width, map height, tile mapping, object mapping.

        """
        return (self._tileSize, self._mapWidth, self._mapHeight,
                self._tileMapping, self._objMapping)

    def placeObjectAtPos(self, posX, posY, obj):
        """Places obj on tile located at scene position passed.

        Returns True if object was placed down.
        """
        (tX, tY) = self._posToTile(posX, posY)
        return self._placeObject(posX, posY, tX, tY, obj)

    def placeObjectOnTile(self, tX, tY, obj):
        """Places obj on tile coordinate passed.

        Returns True if object was placed down.
        """
        (posX, posY) = self._tileToPos(tX, tY)
        return self._placeObject(posX, posY, tX, tY, obj)

    def placeTileAtPos(self, posX, posY, tile):
        """Places tile on tile located at scene position passed.

        Returns: 2 values. First is True if tile was placed, False if not.
                 Second is the tile replaced, None if spot was blank.
        """
        (tX, tY) = self._posToTile(posX, posY)
        return self._placeTile(posX, posY, tX, tY, tile)

    def placeTileOnTile(self, tX, tY, tile):
        """Places tile on tile coordinate passed.

        Returns: 2 values. First is True if tile was placed.
                 Second is the tile replaced, None if spot was blank.
        """
        (posX, posY) = self._tileToPos(tX, tY)
        return self._placeTile(posX, posY, tX, tY, tile)

    def removeTopAtPos(self, posX, posY):
        """Removes the top item under the scene position passed.

        Item is removed visually and internally.
        """
        images = self._imagesAtPos(posX, posY)

        if (len(images) > 0):
            if (self._hasLine(images)):
                return

            img = images[0]
            self.removeItem(img)
            (tX, tY) = self._posToTile(posX, posY)

            if (not self._removeObjectOnTile(tX, tY, img)):
                self._removeTileOnTile(tX, tY)

    def removeTopObjectAtPos(self, posX, posY):
        """Removes top object under the scene position passed.

        Object is removed visually and internally.
        """
        images = self._imagesAtPos(posX, posY)

        if (len(images) > 0):
            if (self._hasLine(images)):
                return

            image = None
            for i in images:
                # All objects assign a label to their image
                if i.getLabel() != "":
                    image = i
                    self.removeItem(i)
                    break

            if image:
                (tX, tY) = self._posToTile(posX, posY)
                self._removeObjectOnTile(tX, tY, image)

    def removeTopObjectOnTile(self, tX, tY):
        """Removes top object at tile coordinate passed.

        Object is removed visually and internally.
        """
        rectArea = QtCore.QRectF(
            tX * self._tileSize,
            tY * self._tileSize,
            self._tileSize,
            self._tileSize
        )

        images = self.items(rectArea)

        if (len(images) > 0):
            image = None
            for i in images:
                if i.zValue() != self._zValLine and i.getLabel() != "":
                    image = i
                    self.removeItem(i)
                    break

            if image:
                self._removeObjectOnTile(tX, tY, image)

    def removeTileAtPos(self, posX, posY):
        """Removes tile (if one exists) at scene position passed."""
        images = self._imagesAtPos(posX, posY)

        if (len(images) > 0):
            if (self._hasLine(images)):
                return

            (tX, tY) = self._posToTile(posX, posY)
            if (self._removeTileOnTile(tX, tY)):
                for img in images:
                    # Tiles are not labeled, objects are.
                    if img.getLabel() == "":
                        self.removeItem(img)
                        break

    def _placeObject(self, posX, posY, tX, tY, obj):
        """Places obj at tile coordinate passed.

        Does nothing under the following conditions:
            If scene position passed is on a grid line.
            If obj has already been placed on the tile.

        Arguments: posX - x scene coordinate
                   posY - y scene coordinate
                   tX - x tile coordinate
                   tY - y tile coordinate
                   obj - object to place

        Returns: True if object was placed down, False if not which occurs
                 when the scene position passed is on a grid line or the
                 object was already on the tile.
        """
        images = self._imagesAtPos(posX, posY)

        if (self._hasLine(images)):
            return False

        key = self._tileToKey(tX, tY)
        # Don't allow multiples of the same object on a tile
        objs = self._objMapping.get(key)
        if objs != None:
            clone = [o for o in objs if o.getPath() == obj.getPath()]
            if len(clone) > 0:
                return False

        # Store actual object internally, only placing its image on the grid
        self._objMapping[key].append(obj)

        objImg = TiledPixmap(obj.getPath())
        objImg.setPixmap(obj.pixmap().copy())

        # Take object's height into account
        (posX, posY) = (self._tileToPos(tX, tY))
        posY = self._tileSize * tY
        if objImg.pixmap().height() > self._tileSize:
            posY -= (objImg.pixmap().height() - self._tileSize)
        objImg.setPos(posX, posY)

        objImg.setZValue(self._zValObj)
        self.addItem(objImg)

        return True

    def _placeTile(self, posX, posY, tX, tY, tile):
        """Places tile at tile coordinate passed.

        Any tile already at the coordinate is replaced.

        Returns early under the following conditions:
            Scene position passed is located on a grid line.
            Tile already at tile coordinate is same as tile being placed.

        Arguments: posX - x scene coordinate
                   posY - y scene coordinate
                   tX - x tile coordinate
                   tY - y tile coordinate
                   tile - tile to place

        Returns: 2 values. First is True if tile was placed, False if not,
                 which occurs when scene position passed is on a grid line or
                 the tile at the tile coordinate is the same as the one being
                 placed.
                 Second is the tile replaced, None if spot was blank.
        """
        images = self._imagesAtPos(posX, posY)

        if (self._hasLine(images)):
            return (False, None)

        key = self._tileToKey(tX, tY)
        if self._tileMapping.get(key) != tile:
            tiles = [t for t in images if t.zValue() == self._zValTile]
            if len(tiles) > 0:
                self.removeItem(tiles[0])

            # Store actual tile internally, only placing its image on the grid
            oldTile = self._tileMapping.get(key)
            self._tileMapping[key] = tile

            tileImg = TiledPixmap()
            tileImg.setPixmap(tile.pixmap().copy())
            tileImg.setPos(self._tileSize * tX, self._tileSize * tY)
            tileImg.setZValue(self._zValTile)
            self.addItem(tileImg)

            return (True, oldTile)

        return (False, None)

    def _removeObjectOnTile(self, tX, tY, image):
        """Removes Object on tile internally that matches the image label.

        Returns true if a matching Object was found.
        """
        key = self._tileToKey(tX, tY)

        objs = self._objMapping.get(key)
        if objs != None and len(objs) > 0:
            for o in objs:
                if o.getPath() == image.getLabel():
                    objs.remove(o)
                    # Two of the same object can't be on the same tile
                    # so return
                    return True

        return False

    def _removeTileOnTile(self, tX, tY):
        """Removes Tile internally at tile location passed.

        Returns true if there was a tile there, false otherwise.
        """
        key = self._tileToKey(tX, tY)

        tile = self._tileMapping.get(key)
        if tile:
            del self._tileMapping[key]
            return True

        return False

    def _clearTile(self, tX, tY):
        """Removes lines, tile and all objects at tile location passed."""
        rectArea = QtCore.QRectF(
            tX * self._tileSize,
            tY * self._tileSize,
            self._tileSize,
            self._tileSize
        )

        key = self._tileToKey(tX, tY)

        if key in self._tileMapping:
            del self._tileMapping[key]
        if key in self._objMapping:
            del self._objMapping[key]

        for item in self.items(rectArea):
            if item.zValue() != self._zValLine:
                self.removeItem(item)

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

    def _imagesAtPos(self, posX, posY):
        """Returns list of images located at scene position passed."""
        point = QtCore.QPointF(posX, posY)
        return self.items(point)

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


