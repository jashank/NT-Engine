#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
from fileop import subInPath


class Tile(QtGui.QGraphicsPixmapItem):
    """QGraphicsPixMap item with id member for state file output."""
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

    SIGNALS: 'selectedFile', filename - emitted from 'selectFile'

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


class TileBar(QtGui.QGraphicsScene):
    """Holds tiles loaded in, organizing tiles in 5 column rows.

    SIGNALS - selectedTile, self.selectedTile -- emitted by mouseMoveEvent

    """
    def __init__(self, parent = None):
        """Initializes members to starting values."""
        QtGui.QGraphicsScene.__init__(self, parent)

        self.defOpacity = 0.45
        self.mousePressed = False
        self.selectedTile = None

    def mousePressEvent(self, event):
        """Sets mouse pressed to true if left button is pressed.

        Overrides mousePressEvent in QGraphicsScene. Also calls mouseMoveEvent
        to perform action.

        """
        if event.button() == QtCore.Qt.LeftButton:
            self.mousePressed = True
            self.mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        """Sets mouse pressed to false if left button is pressed.

        Overrides mouseReleaseEvent in QGraphicsScene.

        """
        if event.button() == QtCore.Qt.LeftButton:
            self.mousePressed = False

    def mouseMoveEvent(self, event):
        """Selects tile under cursor if mouse is pressed.

        Emits 'selectedTile', self.selectedTile if a tile is selected. Overrides
        mouseMoveEvent in QGraphicsScene.

        """
        if self.mousePressed:
            point = event.scenePos()
            tile = self.itemAt(point)

            if tile != None and self.selectedTile != tile:
                if self.selectedTile != None:
                    self.selectedTile.setOpacity(self.defOpacity)

                self.selectedTile = tile
                self.selectedTile.setOpacity(1)
                self.emit(QtCore.SIGNAL('selectedTile'), self.selectedTile)

    def loadTiles(self, pathname):
        """Loads tiles from NT tile animation file.

        When tiles are loaded, they are put onto the tile bar for usage with
        the map editor. If the file isn't a tile animation file then nothing
        will happen. Any tiles previously loaded in are removed.

        Arguments: pathname -- name of path to tile animation file.

        """
        for item in self.items():
            self.removeItem(item)

        tree = ElementTree()
        tree.parse(pathname)
        root = tree.getroot()

        sheets = root.findall('sheet')

        posX = 0
        posY = 0
        column = 0
        MAX_COLUMNS = 5

        for sheet in sheets:
            sheetPath = sheet.get('path')
            # Need str() because pathname will be a Qt string
            sheetImg = QtGui.QImage(subInPath(str(pathname), sheetPath))

            strips = sheet.findall('strip')

            for strip in strips:
                tile = Tile()
                tile.setId(strip.get('id'))

                clip = strip.find('clip')
                x = clip.get('x')
                y = clip.get('y')
                width = clip.get('width')
                height = clip.get('height')

                subImg = sheetImg.copy(int(x), int(y), int(width), int(height))
                pixmap = QtGui.QPixmap.fromImage(subImg)
                tile.setPixmap(pixmap)

                tile.setPos(posX, posY)
                tile.setSize(int(width))
                tile.setOpacity(self.defOpacity)
                self.addItem(tile)

                # (-1) because lines were 1 pixel too long
                self.addLine(posX, posY, posX + float(width) - 1, posY)
                self.addLine(posX, posY, posX, posY + float(height) - 1)

                column += 1
                if column > MAX_COLUMNS:
                    posY += float(height)
                    posX = 0
                    column = 0
                else:
                    posX += float(width)


