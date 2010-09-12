#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
from fileop import subInPath


class Tile(QtGui.QGraphicsPixmapItem):
    def __init__(self, parent = None):
        QtGui.QGraphicsPixmapItem.__init__(self, parent)

        self.idAttr = -1


class LoadTilesButton(QtGui.QPushButton):
    def __init__(self, parent = None):
        QtGui.QPushButton.__init__(self, parent)

        self.setText('Load Tiles')

        self.connect(self, QtCore.SIGNAL('clicked()'), self.selectFile)

    def selectFile(self):
        filename = QtGui.QFileDialog.getOpenFileName(self,
            'Select tile animation file')

        self.emit(QtCore.SIGNAL('selectedFile'), filename)


class TileBar(QtGui.QGraphicsScene):
    def __init__(self, parent = None):
        QtGui.QGraphicsScene.__init__(self, parent)


    def loadTiles(self, pathname):
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
                tile.idAttr = strip.get('id')

                clip = strip.find('clip')
                x = clip.get('x')
                y = clip.get('y')
                width = clip.get('width')
                height = clip.get('height')

                subImg = sheetImg.copy(int(x), int(y), int(width), int(height))
                pixmap = QtGui.QPixmap.fromImage(subImg)
                tile.setPixmap(pixmap)

                tile.setPos(posX, posY)
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


