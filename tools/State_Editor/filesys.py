#!/usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
from extras import Extras
from fileop import subInPath
from tilemap import TileMap
import xmlout


def save(workingPack, tilemap, extras):
    """Saves data of state currently defined in editor.

    Will first check to see if this is just overwriting an existing file that
    the user is working on. If so, will just overwrite that file. Otherwise,
    will open a file dialog for user to specify name/location of file to save,
    and then save it.

    Arguments: workingPack -- path to pack to save to
               tilemap -- tile map to get data from
               extras -- extras dialog to get data from

    """
    # If existing file
        # save over and return
    filename = QtGui.QFileDialog.getSaveFileName(None, 'Save State File',
        workingPack, '*.xml')
    if not filename:
        return
    filename = str(filename)

    size, mapWidth, mapHeight, tiles, objects = tilemap.currentState()
    tilesPath = ''

    # if there are tiles then grab one and see what its animation is
    for tile in tiles.itervalues():
        tilesPath = tile.getAnimPath()
        break

    tileElem = xmlout.createTiles(
        workingPack, size, tilesPath, mapWidth, mapHeight, tiles)
    objectElem = xmlout.createObjects(workingPack, objects)

    musicDict, portalsDict, fontsDict = extras.currentState()
    musicElem = xmlout.createPathName(workingPack, musicDict, 'music', 'song')
    portalElem = xmlout.createPathName(workingPack, portalsDict, 'portals', 'port')
    fontElem = xmlout.createPathName(workingPack, fontsDict, 'fonts', 'font')

    stateElem = xmlout.createState(tileElem, objectElem, musicElem, portalElem,
        fontElem)

    xmlout.createStateFile(stateElem, filename)

def load(workingPack, tilemap, objbar, tilebar, extras):
    """Loads NT state file into editor.

    Arguments: workingPack -- Pack currently being worked on, which is where
                              state should be loaded from
               tilemap -- editor's tilemap
               objbar -- editor's object bar
               tilebar -- editor's tile bar
               extras -- editor's extras section

    """
    filename = QtGui.QFileDialog.getOpenFileName(None, 'Select state file',
        workingPack, "*.xml")
    if not filename:
        return
    filename = str(filename)

    tree = ElementTree()
    tree.parse(filename)
    root = tree.getroot()

    tilemap.clearPlacements()
    extras.clearAll()
    objbar.clear()
    # tile bar is automatically cleared when loaded

    # Load Tiles
    tiles = root.find('tiles')

    animPath = tiles.find('animation').get('path')
    if animPath != "":
        animPath = subInPath(filename, animPath)
        tilebar.loadTiles(animPath)

    layout = tiles.find('layout')

    size = int(tiles.find('size').get('px'))
    mapWidth = int(layout.get('width'))
    mapHeight = int(layout.get('height'))
    tilemap.setDims(size, mapWidth, mapHeight)

    if layout.text:
        # strip because there is whitespace on the ends
        tileIds = str(layout.text).strip().split(' ')

        # Go from (1,1) to avoid grid lines
        pos = QtCore.QPointF(1,1)

        pointX = 0
        pointY = 0
        for tid in tileIds:
            tid = int(tid)
            if tid != -1:
                tile = tilebar.getTile(tid)
                tilemap.setSelectionTile(tile)
                tilemap.placeTile(pos, pointX, pointY)

            pointX = pointX + 1
            pos.setX(pos.x() + size)
            if pointX >= mapWidth:
                pointX = 0
                pointY = pointY + 1
                pos.setX(1)
                pos.setY(pos.y() + size)

    # Load Objects
    objects = root.find('objects')
    allObjs = objects.findall('object')

    for obj in allObjs:
        path = obj.get('path')
        absPath = subInPath(filename, path)
        objbar.loadObject(absPath)

        instances = obj.findall('inst')
        for inst in instances:
            strip = int(inst.get('strip'))
            x = int(inst.get('x'))
            y = int(inst.get('y'))

            obj = objbar.getObject(absPath, strip)
            if obj:
                tilemap.setSelectionObject(obj)
                # Add 1 to avoid grid lines
                pos = QtCore.QPointF(x * size + 1, y * size + 1)
                tilemap.placeObject(pos, x, y)

    # Load Extras
    music = root.find('music')
    songs = music.findall('song')
    _loadExtras(filename, songs, extras.addSong)

    portals = root.find('portals')
    ports = portals.findall('port')
    _loadExtras(filename, ports, extras.addPortal)

    fonts = root.find('fonts')
    fontList = fonts.findall('font')
    _loadExtras(filename, fonts, extras.addFont)

    # Undo any selections for tilemap
    tilemap.setSelectionObject(None)

def _loadExtras(filename, elemList, extraAddFunc):
    """Calls extraAddFunc on data in elemList.

    Arguments: filename - path to State file being loaded
               elemList - list of elements (should be sub elements of an
                          'extras' type like <music>, <portals>, or <fonts>)
                          to load data from.
               extraAddFunc - function to call on data from each element
    """
    for elem in elemList:
        path = elem.get('path')
        absPath = subInPath(filename, path)
        name = elem.get('name')
        extraAddFunc(absPath, name)


