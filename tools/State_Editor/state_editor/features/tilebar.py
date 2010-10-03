#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
from state_editor.features import bar
from state_editor.io.fileop import subInPath


class Tile(QtGui.QGraphicsPixmapItem):
    """Item with id member for state file output."""
    def __init__(self, parent = None):
        """idAttr is set to -1 because it implies a null tile."""
        QtGui.QGraphicsPixmapItem.__init__(self, parent)

        self._animPath = ""
        self._idAttr = -1
        self._size = 0

    def getAnimPath(self):
        """Returns animation path to where tile is defined."""
        return self._animPath

    def getId(self):
        """Returns id of tile."""
        return self._idAttr

    def getSize(self):
        """Returns size of this tile."""
        return self._size

    def setAnimPath(self, path):
        """Sets animation path where this tile is defined."""
        self._animPath = path

    def setId(self, idNum):
        """Sets id of this tile."""
        self._idAttr = idNum

    def setSize(self, size):
        """Sets size of this tile."""
        self._size = size


class LoadTilesButton(QtGui.QPushButton):
    """When pressed, opens dialog to select file to load tiles from.

    SIGNALS: 'selectedFile', filename -- emitted from 'selectFile'

    """
    def __init__(self, workingPack, parent = None):
        """When initialized, directory to load from is set to working pack."""
        QtGui.QPushButton.__init__(self, parent)

        self._workingPack = workingPack
        self.setText('Load Tiles')

        self.connect(self, QtCore.SIGNAL('clicked()'), self._selectFile)

    def _selectFile(self):
        """Opens dialog for user to select file to load tiles from.

        SIGNALS: 'selectedFile', filename -- emitted when file is selected,
            passing pathname of file along.

        """
        filename = str(QtGui.QFileDialog.getOpenFileName(self,
            'Select tile animation file', self._workingPack, "*.xml"))

        # Only bother emitting if user actually selected a file
        if filename != '':
            self.emit(QtCore.SIGNAL('selectedFile'), filename)


class TileBar(bar.Bar):
    """Holds tiles loaded in, organizing tiles in 4 column rows."""

    def __init__(self, parent = None):
        """Initializes members to starting values."""
        bar.Bar.__init__(self, parent)

        # Dictionary containing (id, tile) pairs.
        self._tileIds = dict()

        # Path to animation file containing tiles
        self._tilesPath = ""

        # Signal to emit when tile is selected
        self._selectSignal = 'selectedTile'

    def clear(self):
        """Clears contents of tile bar."""
        bar.Bar.clear(self)
        self._tileIds.clear()

    def getTile(self, tileId):
        """Retrieve a tile by its id."""
        return self._tileIds.get(tileId)

    def loadTiles(self, pathname):
        """Loads tiles from NT tile animation file.

        When tiles are loaded, they are put onto the tile bar for usage with
        the map editor. If the file isn't a tile animation file then nothing
        will happen. Any tiles previously loaded in are removed.

        Arguments: pathname -- name of path to tile animation file.

        """
        self.clear()

        self._tilesPath = pathname

        tree = ElementTree()
        tree.parse(pathname)
        root = tree.getroot()

        sheets = root.findall('sheet')

        posX = 0
        posY = 0
        column = 0
        MAX_COLUMNS = 4

        for sheet in sheets:
            sheetPath = sheet.get('path')
            sheetImg = QtGui.QImage(subInPath(pathname, sheetPath))

            strips = sheet.findall('strip')

            for strip in strips:
                tile = Tile()
                tile.setAnimPath(self._tilesPath)
                tileId = int(strip.get('id'))
                tile.setId(tileId)
                self._tileIds[tileId] = tile

                bar.clipFromSheet(sheetImg, strip, tile)

                tile.setSize(tile.pixmap().width())
                lnX, lnY = bar.setForBar(posX, posY, self._defOpacity, tile)

                self.addItem(tile)
                self.addLine(lnX)
                self.addLine(lnY)

                posX, posY, column = bar.updateGridPos(posX, posY, column,
                    MAX_COLUMNS, tile.pixmap().width(), tile.pixmap().height())

