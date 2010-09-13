#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
from fileop import subInPath


class HighlightEffect(QtGui.QGraphicsEffect):
    """A highlight effect for rectangular QGraphicsPixmapItems."""
    def __init__(self, thickness, parent = None):
        """Sets up color and area around which item will be highlighted.

        Arguments: thickness -- thickness of highlight in pixels

        """
        QtGui.QGraphicsEffect.__init__(self, parent)
        # semi-transparent yellow
        self.color = QtGui.QColor(255, 255, 0, 128)
        self.offset = QtCore.QPoint(thickness, thickness)

    def boundingRectFor(self, sourceRect):
        """Returns bounding rect for source that effect will surround.

        Overrides parent function.

        """
        return sourceRect.adjusted( -self.offset.x(), -self.offset.y(),
            self.offset.x(), self.offset.y())

    def draw(self, painter):
        """Draws effect, overrides parent function."""
        if self.sourceIsPixmap():
            pixmap, offset = self.sourcePixmap()
            bound = self.boundingRectFor(pixmap.rect())

            painter.save()
            painter.setPen(QtCore.Qt.NoPen)
            painter.setBrush(self.color)

            drawPoint = QtCore.QPoint(
                offset.x() - self.offset.x(), offset.y() - self.offset.y())
            bound.moveTopLeft(drawPoint)

            painter.drawRoundedRect(bound, 5, 5, QtCore.Qt.RelativeSize)
            painter.drawPixmap(offset, pixmap)
            painter.restore()


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
    """Holds tiles loaded in, organizing tiles in 5 column rows."""
    def __init__(self, parent = None):
        """Default initialization."""
        QtGui.QGraphicsScene.__init__(self, parent)

        self.tileSelected = None

    def mousePressEvent(self, event):
        """If left-click is on a tile, that tile is selected and highlighted.

        Overrides mousePressEvent in QGraphicsScene.

        """
        if event.button() == QtCore.Qt.LeftButton:
            point = event.buttonDownScenePos(QtCore.Qt.LeftButton)
            tile = self.itemAt(point)

            if tile != None:
                if self.tileSelected != None:
                    self.tileSelected.setGraphicsEffect(None)

                self.tileSelected = tile

                # 16 seems like a nice number!
                thickness = self.tileSelected.getSize() / 16
                self.tileSelected.setGraphicsEffect(HighlightEffect(thickness))

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
                self.addItem(tile)

                self.addLine(posX, posY, posX + float(width), posY)
                self.addLine(posX, posY, posX, posY + float(height))

                column += 1
                if column > MAX_COLUMNS:
                    posY += float(height)
                    posX = 0
                    column = 0
                else:
                    posX += float(width)


