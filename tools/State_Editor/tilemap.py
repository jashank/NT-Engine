#!/usr/bin/env python

from PyQt4 import QtGui


class TileMap(QtGui.QGraphicsScene):
    def __init__(self, parent = None):
        QtGui.QGraphicsScene.__init__(self, parent)
