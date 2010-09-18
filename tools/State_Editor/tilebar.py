#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
import bar
from fileop import subInPath


class Tile(QtGui.QGraphicsPixmapItem):
    """Item with id member for state file output."""
    def __init__(self, parent = None):
        """idAttr is set to -1 because it implies a null tile."""
        QtGui.QGraphicsPixmapItem.__init__(self, parent)

        self._idAttr = -1
        self._size = 0

    def getSize(self):
        """Returns size of this tile."""
        return self._size

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
    def __init__(self, parent = None):
        """Button is named and when clicked, calls selectFile."""
        QtGui.QPushButton.__init__(self, parent)

        self.setText('Load Tiles')

        self.connect(self, QtCore.SIGNAL('clicked()'), self.selectFile)

    def selectFile(self):
        """Opens dialog for user to select file to load tiles from.

        SIGNALS: 'selectedFile', filename -- emitted when file is selected,
            passing pathname of file along.

        """
        filename = QtGui.QFileDialog.getOpenFileName(self,
            'Select tile animation file')

        self.emit(QtCore.SIGNAL('selectedFile'), filename)


class TileBar(bar.Bar):
    """Holds tiles loaded in, organizing tiles in 4 column rows."""

    def __init__(self, parent = None):
        """Initializes members to starting values."""
        bar.Bar.__init__(self, parent)

        # Path to animation file containing tiles
        self._tilesPath = ""

        # Signal to emit when tile is selected
        self._selectSignal = 'selectedTile'

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
            # Need str() because pathname will be a Qt string
            sheetImg = QtGui.QImage(subInPath(str(pathname), sheetPath))

            strips = sheet.findall('strip')

            for strip in strips:
                tile = Tile()
                tile.setId(strip.get('id'))

                bar.clipFromSheet(sheetImg, strip, tile)

                tile.setSize(tile.pixmap().width())
                lnX, lnY = bar.setForBar(posX, posY, self._defOpacity, tile)

                self.addItem(tile)
                self.addLine(lnX)
                self.addLine(lnY)

                posX, posY, column = bar.updateGridPos(posX, posY, column,
                    MAX_COLUMNS, tile.pixmap().width(), tile.pixmap().height())

