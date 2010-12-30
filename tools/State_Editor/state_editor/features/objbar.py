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

#! /usr/bin/env python


from collections import defaultdict
from os.path import basename, splitext
from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
from state_editor.features import bar
from state_editor.io.fileop import subInPath


class Object(QtGui.QGraphicsPixmapItem):
    """Represents NT Object. Holds object file path and animation number.

    The animation number is the number of the animation on the Object's
    sprite sheet. For example the first animation is 0, second is 1, etc. This
    is needed for the user to place the Object in different starting positions.

    """
    def __init__(self, parent = None):
        """Default initialization."""
        QtGui.QGraphicsPixmapItem.__init__(self, parent)

        self.animNum = 0
        self.path = ""

    def setAnimNum(self, num):
        """Set animation number."""
        self.animNum = num

    def setPath(self, path):
        """Set path to object file."""
        self.path = path

    def getAnimNum(self):
        """Returns animation number."""
        return self.animNum

    def getPath(self):
        """Return path to object file."""
        return self.path


class LoadObjectsButton(QtGui.QPushButton):
    """When pressed, opens dialog for user to load object(s) from file(s).

    SIGNALS: 'selectedFiles', filenames -- emitted from 'selectFiles'

    """
    def __init__(self, workingPack, parent = None):
        """When initialized, direcotry to load from is set to working pack."""
        QtGui.QPushButton.__init__(self, parent)
        self._workingPack = workingPack

        self.setText('Load Object(s)')

        self.connect(self, QtCore.SIGNAL('clicked()'), self._selectFiles)

    def _selectFiles(self):
        """Opens dialog for user to select object file(s) to load.

        SIGNALS: 'selectedFiles', filenames -- emitted when file(s) is selected,
            passing along paths to all files selected. Is a QStringList.

        """
        filenames = QtGui.QFileDialog.getOpenFileNames(self,
            'Select Object file(s)', self._workingPack, "*.xml")

        self.emit(QtCore.SIGNAL('selectedFiles'), filenames)


class ObjectBar(bar.Bar):
    """Holds objects loaded in, organizing objects in 4 column rows.

    The Object Bar actually loads in a different object representation for all
    of the object's animation strips so the user can choose from the object's
    animations when selecting from the bar. Thus the loading in of one object
    can actually lead to multiple objects being placed on the bar.

    Note that this bar only holds Objects with animation files (i.e. those that
    can be represented visually). Objects without animation files, called mod
    objects, are loaded in separately.

    """
    def __init__(self, parent = None):
        """Default initialization."""
        bar.Bar.__init__(self, parent)

        # Keeps track of greatest height of an item while adding items to row
        self._greatestHeight = 0

        # Signal to emit when object is selected
        self._selectSignal = 'selectedObject'

        # Dictionary of objects via (path, list of Objects) pairs
        # Note that in the editor different strips are different Objects
        self._objDict = defaultdict(list)

    def clear(self):
        """Overrides clear function to set all pos values back to starting."""
        bar.Bar.clear(self)
        self._posX = 0
        self._posY = 0
        self._column = 0
        self._greatestHeight = 0
        self._objDict.clear()

    def getObject(self, path, animNum):
        """Returns object given path it loaded from and anim number.

           Returns None if none found.
        """
        objs = self._objDict.get(path)
        for obj in objs:
            if obj.getAnimNum() == animNum:
                return obj

        return None

    def loadObjects(self, filepaths):
        """Loads Objects from a QStringList of paths to object files."""
        for path in filepaths:
            path = str(path)
            if path not in self._objDict:
                self.loadObject(path)

    def loadObject(self, filepath):
        """Loads object located at filepath.

        If an object doesn't have an animation file, a default "script" image
        is loaded in.

        Arguments: filepath -- path to object's xml file
        """
        objTree = ElementTree()
        objTree.parse(filepath)
        objRoot = objTree.getroot()

        animElem = objRoot.find('animation')
        relAnimPath = animElem.get('path')

        # Empty path means no animation file for object. So represent it
        # via a visual representation for mod objects. 
        objIsAnimated = (relAnimPath != '')

        absAnimPath = ''
        if objIsAnimated:
            absAnimPath = subInPath(filepath, relAnimPath)
        else:
            absAnimPath = "state_editor/media/mod_rep.xml"

        animTree = ElementTree()
        animTree.parse(absAnimPath)
        animRoot = animTree.getroot()

        sheets = animRoot.findall('sheet')

        objFile = basename(filepath)
        objName = splitext(objFile)[0]

        MAX_COLUMNS = 4
        animNum = 0

        for sheet in sheets:
            sheetPath = sheet.get('path')

            sheetImg = None
            if objIsAnimated:
                sheetImg = QtGui.QImage(subInPath(filepath, sheetPath))
            else:
                sheetImg = QtGui.QImage(sheetPath)

            strips = sheet.findall('strip')

            for strip in strips:
                obj = Object()
                self._objDict[filepath].append(obj)
                obj.setPath(filepath)
                obj.setAnimNum(animNum)

                tipSuffix = ''
                if objIsAnimated:
                    tipSuffix = 'Strip ' + str(animNum)
                else:
                    tipSuffix = 'No Anim'
                obj.setToolTip(objName + ': ' + tipSuffix)

                bar.clipFromSheet(sheetImg, strip, obj)

                lnX, lnY = bar.setForBar(self._posX, self._posY,
                    self._defOpacity, obj)
                self.addItem(obj)
                self.addLine(lnX)
                self.addLine(lnY)

                if obj.pixmap().height() > self._greatestHeight:
                    self._greatestHeight = obj.pixmap().height()

                self._posX, self._posY, self._column = bar.updateGridPos(
                    self._posX, self._posY, self._column, MAX_COLUMNS,
                    obj.pixmap().width(), self._greatestHeight)

                # On a new row
                if self._column == 0:
                    self._greatestHeight = 0

                animNum += 1

