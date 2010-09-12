#!/usr/bin/env python

from PyQt4 import QtCore, QtGui


class MapDialog(QtGui.QDialog):
    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self, parent)

        self.setModal(True)
        self.setWindowTitle('Enter tile dimensions')

        sizeLabel = QtGui.QLabel('Size of a tile in pixels:')
        widthLabel = QtGui.QLabel('Width of map in tiles:')
        heightLabel = QtGui.QLabel('Height of map in tiles:')

        self.tileSize = QtGui.QLineEdit('0')
        self.mapWidth = QtGui.QLineEdit('0')
        self.mapHeight = QtGui.QLineEdit('0')

        ok = QtGui.QPushButton()
        ok.setText('OK')
        ok.connect(ok, QtCore.SIGNAL('clicked()'), self.okPress)

        sizeBox = QtGui.QHBoxLayout()
        sizeBox.addWidget(sizeLabel)
        sizeBox.addWidget(self.tileSize)

        widthBox = QtGui.QHBoxLayout()
        widthBox.addWidget(widthLabel)
        widthBox.addWidget(self.mapWidth)

        heightBox = QtGui.QHBoxLayout()
        heightBox.addWidget(heightLabel)
        heightBox.addWidget(self.mapHeight)

        layout = QtGui.QVBoxLayout()
        layout.addLayout(sizeBox)
        layout.addLayout(widthBox)
        layout.addLayout(heightBox)
        layout.addWidget(ok)

        self.setLayout(layout)

    def okPress(self):
        size = self.tileSize.text()
        width = self.mapWidth.text()
        height = self.mapHeight.text()
        dims = [int(size), int(width), int(height)]

        self.emit(QtCore.SIGNAL('gotDims'), dims)
        self.done(1)


class SetMapDimButton(QtGui.QPushButton):
    def __init__(self, parent = None):
        QtGui.QPushButton.__init__(self, parent)

        self.setText('Map Dimensions')

        self.connect(self, QtCore.SIGNAL('clicked()'), self.openMapDialog)

    def openMapDialog(self):
        dialog = MapDialog()
        dialog.exec_()


class TileMap(QtGui.QGraphicsScene):
    def __init__(self, parent = None):
        QtGui.QGraphicsScene.__init__(self, parent)

    def setDims(self, tileSize, mapWidth, mapHeight):
        i = 1
        # nothing


