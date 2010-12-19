"""
(C) Copyright 2010-2011 Alex Shipley

This file is part of the NT State Editor.

The NT State Editor is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The NT State Editor is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the NT State Editor.  If not, see <http://www.gnu.org/licenses/>.
"""

#!/usr/bin/env python


from collections import defaultdict
from xml.dom import minidom
import xml.etree.ElementTree as ElementTree
from state_editor.io.fileop import relPathToPack
from state_editor.features.objbar import Object
from state_editor.features.tilebar import Tile


def createTiles(workingPack, size, path, mapWidth, mapHeight, tileMapping):
    """Returns <tiles> element for NT State file.

    Arguments: workingPack -- Path to pack user is working with
               size -- size of a tile in pixels (i.e. 48 would be 48x48)
               path -- path to animation file for tiles, can be None
               mapWidth -- width of map in tiles
               mapHeight -- height of map in tiles
               tileMapping -- dictionary containing (coord,Tile) pairs. A Tile
                   is defined in the tilebar file, and a coord is a coordinate
                   on the map in the form of a string like '3,5' or '23,2'

    Returns: An Element instance from ElementTree for <tiles>

    """
    root = ElementTree.Element("tiles")

    sz = ElementTree.SubElement(root, "size", {'px':str(size)})

    # Won't necessarily be a path to tile animation file
    relPath = ''
    if path:
        relPath = relPathToPack(workingPack, path)

    anim = ElementTree.SubElement(root, "animation", {'path':relPath})

    layout = ElementTree.SubElement(root, "layout",
        {'width':str(mapWidth), 'height':str(mapHeight)})
    layoutText = []
    hasTiles = False

    # output by row
    for y in range(0, mapHeight):
        for x in range(0, mapWidth):
            key = str(x) + "," + str(y)
            tile = tileMapping.get(key)
            if not tile:
                layoutText.append('-1')
            else:
                layoutText.append(str(tile.getId()))
                hasTiles = True

    # Don't bother outputting a layout unless there is at least one tile
    if hasTiles:
        layout.text = ' '.join(layoutText)

    return root

def createObjects(workingPack, objMapping):
    """Returns <objects> element for NT State file.

    Arguments: workingPack -  Path to pack user is working with
               objMapping -- dictionary containing (coord,list of Objects). An
                   Object is defined in the objbar file, and a coord is a
                   coordinate on the map in the form of a string like '3,5'
                   or '23,2'

    Returns: An Element instance from ElementTree for <objects>

    """
    root = ElementTree.Element('objects')

    # Reverse mapping to get (Object path, coords/object animation strip)
    invMapping = defaultdict(list)
    for coord, objects in objMapping.iteritems():
        for obj in objects:
            invMapping[obj.getPath()].append([coord, str(obj.getAnimNum())])

    for objPath, instances in invMapping.iteritems():
        relPath = relPathToPack(workingPack, objPath)
        objElem = ElementTree.Element('object', {'path':relPath})

        for inst in instances:
            splitCoords = inst[0].split(',')
            instElem = ElementTree.Element('inst',
                {'x':splitCoords[0], 'y':splitCoords[1],
                 'strip':inst[1]})
            objElem.append(instElem)

        root.append(objElem)

    return root


def createPathName(workingPack, pathNameDict, parentStr, subElemStr):
    """Used for music, portals, and fonts part of State file.

    Since the other elements all have the same basic structure, just with
    different names, this function allows one to retrieve the parent element
    for this structure while specifying the name of the parent and the name
    of the sub elements in the parent. For example one may call this function
    like... "createPathName(someDict, 'music', 'song')".

    Arguments:
               workingPack -- path to Pack user is working on
               pathNameDict -- dictionary of (path, name) for data.
               parentStr -- name of parent element
               subElemStr -- what sub elements will be named.

    Returns: Element instance of ElementTree for <parentStr>

    """
    root = ElementTree.Element(parentStr)

    for path, name in pathNameDict.iteritems():
        relPath = relPathToPack(workingPack, path)
        subElem = ElementTree.Element(subElemStr,
            {'path':relPath, 'name':name})
        root.append(subElem)

    return root

def createState(tileElem, objElem, musicElem, portalElem, fontElem):
    """Returns <state> element for NT state file.

    Arguments: Note that all arguments are of the type Element from ElementTree
               tileElem -- <tiles> Element
               objElem -- <objects> Element
               musicElem -- <music> Element
               portalElem -- <portals> Element
               fontElem -- <fonts> Element

    Returns: Element instance of ElementTree for <state>.
    """
    root = ElementTree.Element('state')
    root.append(tileElem)
    root.append(objElem)
    root.append(musicElem)
    root.append(portalElem)
    root.append(fontElem)
    return root

def createStateFile(stateElem, path):
    """Creates state file at 'path' given state element.

    Arguments: stateElem -- Element of ElementTree, <state>
               path -- path to file to write to

    """
    # elementtree outputs in a single line, so need to prettify first
    uglyString = ElementTree.tostring(stateElem)
    reparsed = minidom.parseString(uglyString)
    reparsed = reparsed.toprettyxml(indent="  ")

    xmlFile = open(path, 'w')
    xmlFile.write(reparsed)
    xmlFile.close()

