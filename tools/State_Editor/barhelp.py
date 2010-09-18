#! /usr/bin/env/python 

from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui


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

