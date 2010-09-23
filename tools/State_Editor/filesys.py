#!/usr/bin/env python


from PyQt4 import QtGui
from extras import Extras
from tilemap import TileMap
import xmlout

def save(tilemap, extras):
    """Saves data of state currently defined editor.

    Will first check to see if this is just overwriting an existing file that
    the user is working on. If so, will just overwrite that file. Otherwise,
    will open a file dialog for user to specify name/location of file to save,
    and then save it.

    Arguments: tilemap -- tile map to get data from
               extras -- extras dialog to get data from

    """
    # If existing file
        # save over and return
    filename = QtGui.QFileDialog.getSaveFileName("Save State File", "",
        "*.xml")

    size, mapWidth, mapHeight, tiles, objects = tileMap.currentState()


