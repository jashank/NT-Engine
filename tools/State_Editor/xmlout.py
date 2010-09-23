#!/usr/bin/env python


from collections import defaultdict
from xml.dom import minidom
from xml.etree.ElementTree import ElementTree
from objbar import Object
from tilebar import Tile


def createTiles(size, path, mapWidth, mapHeight, tileMapping):
    """Returns <tiles> element for NT State file.

    Arguments: size -- size of a tile in pixels (i.e. 48 would be 48x48)
               path -- path to animation file for tiles
               mapWidth -- width of map in tiles
               mapHeight -- height of map in tiles
               tileMapping -- dictionary containing (coord,Tile) pairs. A Tile
                   is defined in the tilebar file, and a coord is a coordinate
                   on the map in the form of a string like '3,5' or '23,2'

    Returns: An Element instance from ElementTree for <tiles>

    """
    root = ElementTree.Element("tiles")

    sz = ElementTree.SubElement(root, "size", {'px':size})
    anim = ElementTree.SubElement(root, "animation", {'path':path})

    layout = ElementTree.SubElement(root, "layout",
        {'width':mapWidth, 'height':mapHeight})
    layoutText = []
    for x in range(0, mapWidth):
        for y in range(0, mapHeight):
            key = str(x) + "," + str(y)
            tile = tileMapping.get(key)
            if not tile:
                layoutText.append(-1)
            else:
                layoutText.append(tile.getId())
    layout.text.join(layoutText)

    return root

def createObjects(objMapping):
    """Returns <objects> element for NT State file.

    Arguments: objMapping -- dictionary containing (coord,list of Objects). An
                   Object is defined in the objbar file, and a coord is a
                   coordinate on the map in the form of a string like '3,5'
                   or '23,2'

    Returns: An Element instance from ElementTree for <objects>

    """
    root = ElementTree.Element('objects')

    # Reverse mapping to get (Object path, coords)
    invMapping = defaultdict(list)
    for coord, objects in objMapping.iteritems():
        for obj in objects:
            invMapping[obj.getPath()].append(coord)

    for objPath, coords in invMapping.iterItems():
        objElem = ElementTree.Element('object', {'path':objPath})

        for coord in coords:
            splitCoords = coord.split(',')
            coordElem = ElementTree.Element(objElem, 'inst',
                {'x':splitCoords[0], 'y':splitCoords[1]})
            objElem.append(coordElem)

        root.append(objElem)

    return root


def createPathName(pathNameDict, parentStr, subElemStr):
    """Used for music, portals, and fonts part of State file.

    Since the other elements all have the same basic structure, just with
    different names, this function allows one to retrieve the parent element
    for this structure while specifying the name of the parent and the name
    of the sub elements in the parent. For example one may call this function
    like... "createPathName(someDict, 'music', 'song')".

    Arguments: pathNameDict -- dictionary of (path, name) for data.
               parentStr -- name of parent element
               subElemStr -- what sub elements will be named.

    Returns: Element instance of ElementTree for <parentStr>

    """
    root = ElementTree.Element(parentStr)

    for path, name in musicDict.iteritems():
        subElem = ElementTree.Element(subElemStr,
            {'path':path, 'name':name})
        root.append(subElem)

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
    reparsed.toprettyxml(indent="  ")

    xmlFile = open(path, 'w')
    reparsed.writexml(xmlFile)
    xmlFile.close()

