#!/usr/bin/env python


from xml.etree.ElementTree import ElementTree
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
    filename = QtGui.QFileDialog.getSaveFileName(None, 'Save State File', '',
        '*.xml')
    if not filename:
        return

    size, mapWidth, mapHeight, tiles, objects = tilemap.currentState()
    tilesPath = ""

    # if there are tiles then grab one and see what its animation is
    for tile in tiles.itervalues():
        tilesPath = tile.getAnimPath()
        break

    tileElem = xmlout.createTiles(size, tilesPath, mapWidth, mapHeight, tiles)
    objectElem = xmlout.createObjects(objects)

    musicDict, portalsDict, fontsDict = extras.currentState()
    musicElem = xmlout.createPathName(musicDict, 'music', 'song')
    portalElem = xmlout.createPathName(portalsDict, 'portals', 'port')
    fontElem = xmlout.createPathName(fontsDict, 'fonts', 'font')

    stateElem = xmlout.createState(tileElem, objectElem, musicElem, portalElem,
        fontElem)

    xmlout.createStateFile(stateElem, filename)

