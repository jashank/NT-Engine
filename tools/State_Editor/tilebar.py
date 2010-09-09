#! /usr/bin/env python

from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui


class Tile(QtGui.QGraphicsPixmapItem):
    def __init__(self, parent = None):
        QtGui.QGraphicsScene.__init__(self, parent)

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

    def loadTiles(self, filename):
        tree = ElementTree()
        tree.parse(filename)

        sheets = tree.findall('sheet')

        for sheet in sheets:
            sheetPath = sheet.get('path')
            sheetImg = QtGui.QImage(sheetPath)

            strips = sheet.findall('strip')
            for strip in strips:
                tile = Tile()
                tile.idAttr = strip.get('id')

                clip = strip.find('clip')
                x = clip.get('x')
                y = clip.get('y')
                width = clip.get('width')
                height = clip.get('height')

                subImg = sheetImage.copy(x, y, width, height)
                pixmap = QtGui.QPixmap.fromImage(subImg)
                tile.setPixmap(pixmap)

                self.addItem(tile)

