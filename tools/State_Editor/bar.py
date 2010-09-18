#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui


class Bar(QtGui.QGraphicsScene):
    """Base class for "bars" in the editor."""
    def __init__(self, parent = None):
        """Initialize members shared by bars."""
        QtGui.QGraphicsScene.__init__(self, parent)

        # QGraphicsPixmapItem currently selected in bar
        self._selectedItem = None

        # Signal to emit when item is selected
        self._selectSignal = ""

        # Default opacity for items in bar
        self._defOpacity = 0.45

        # Whether mouse is pressed in the bar
        self._mousePressed = False

        # Positional information for placement of next item in bar
        self._posX = 0
        self._posY = 0
        self._column = 0
        self._MAXCOLUMNS = 4

    def mousePressEvent(self, event):
        """Sets mouse pressed to true if left button is pressed."""
        if event.button() == QtCore.Qt.LeftButton:
            self._mousePressed = True
            self.mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        """Sets mouse pressed to false if left button is pressed."""
        if event.button() == QtCore.Qt.LeftButton:
            self._mousePressed = False

    def mouseMoveEvent(self, event):
        """Selects item under cursor if mouse is pressed.

        Emits self._selectSignal, self._selectedItem if an item is selected.

        """
        if self._mousePressed:
            point = event.scenePos()
            item = self.itemAt(point)

            if item != None and self._selectedItem != item:
                if self._selectedItem != None:
                    self._selectedItem.setOpacity(self._defOpacity)

                self._selectedItem = item
                self._selectedItem.setOpacity(1)
                self.emit(QtCore.SIGNAL(self._selectSignal),
                    self._selectedItem)

    def unselect(self):
        """Unselects currently selected item."""
        if self._selectedItem != None:
            self._selectedItem.setOpacity(self._defOpacity)
            self._selectedItem = None


def clipFromSheet(sheet, strip, pixItem):
    """Clips first image in NT strip and sets it to item's pixmap.

    Arguments: sheet -- sheet to clip strip from
               strip -- xml element from ElementTree.Element containing strip
                        from NT animation file.
               pixItem -- item to set pixmap of first clip to

    """
    clip = strip.find('clip')
    x = clip.get('x')
    y = clip.get('y')
    width = clip.get('width')
    height = clip.get('height')

    subImg = sheet.copy(int(x), int(y), int(width), int(height))
    pixmap = QtGui.QPixmap.fromImage(subImg)
    pixItem.setPixmap(pixmap)


def setForBar(posX, posY, opacity, pixItem):
    """Setup for placing an item on a bar.

    pixItem position and opacity are modified, and 2 gridlines are returned
    to surround it.

    Arguments: posX -- x position relative to scene to place pixItem
               posY -- y position relative to scene to place pixItem
               opacity -- opacity of pixItem when not selected
               pixItem -- QGraphicsPixmapItem to be modified, should already
                          have a pixmap assigned to it (see clipFromSheet)

    Returns: 2 QLineFs set to surround the pixItem

    """
    pixItem.setPos(posX, posY)
    pixItem.setOpacity(opacity)

    # (-1) because lines are 1 pixel too long
    lineX = QtCore.QLineF(posX, posY, posX + pixItem.pixmap().width() - 1, posY)
    lineY = QtCore.QLineF(posX, posY, posX, posY + pixItem.pixmap().height() - 1)
    return lineX, lineY


def updateGridPos(posX, posY, column, maxColumns, shiftX, shiftY):
    """Returns updated grid positions after placing item on grid.

    Arguments: posX -- last x position item was placed in
               posY -- last y position item was placed in
               column -- last column item was placed in
               maxColumns -- maximum number of columns per row
               shiftX -- number of pixels to shift over for next posX
               shiftY -- number of pixels to shift over for next posY

    Returns: posX, posY, column with updated values

    """
    column += 1
    if column >= maxColumns:
        posY += shiftY
        posX = 0
        column = 0
    else:
        posX += shiftX

    return posX, posY, column

