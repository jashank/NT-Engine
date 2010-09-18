#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
import bar
from fileop import subInPath


class Object(QtGui.QGraphicsPixmapItem):
    """Represents NT Object. Holds object file path and animation number.

    The animation number is the number of the animation on the Object's
    sprite sheet. For example the first animation is 0, second is 1, etc. This
    is needed for the user to place the Object in different starting positions.

    """
    def __init__(self, parent = None):
        """Default initialization."""
        QtGui.QGraphicsPixmapItem.__init__(self, parent)

        self._animNum = 0
        self._path = ""

    def setAnimNum(self, num):
        """Set animation number."""
        self._animNum = num

    def setPath(self, path):
        """Set path to object file."""
        self._path = path

    def getPath(self):
        """Return path to object file."""
        return self._path


class LoadObjectsButton(QtGui.QPushButton):
    """When pressed, opens dialog for user to load object(s) from file(s).

    SIGNALS: 'selectedFiles', filenames -- emitted from 'selectFiles'

    """
    def __init__(self, parent = None):
        """Button is named and when clicked, calls selectFiles."""
        QtGui.QPushButton.__init__(self, parent)

        self.setText('Load Object(s)')

        self.connect(self, QtCore.SIGNAL('clicked()'), self.selectFiles)

    def selectFiles(self):
        """Opens dialog for user to select object file(s) to load.

        SIGNALS: 'selectedFiles', filenames -- emitted when file(s) is selected,
            passing along paths to all files selected.

        """
        filenames = QtGui.QFileDialog.getOpenFileNames(self,
            'Select Object file(s)')

        self.emit(QtCore.SIGNAL('selectedFiles'), filenames)


class ClearButton(QtGui.QPushButton):
    """Emits a 'clear' signal when pressed.

    SIGNALS: 'clear' -- emitted from 'emitClear'

    """
    def __init__(self, parent = None):
        """Set text for button and connect."""
        QtGui.QPushButton.__init__(self, parent)
        self.setText('Clear')
        self.connect(self, QtCore.SIGNAL('clicked()'), self.emitClear)

    def emitClear(self):
       """Emits 'clear' signal.

       SIGNALS: clear

       """
       self.emit(QtCore.SIGNAL('clear'))


class ObjectBar(bar.Bar):
    """Holds objects loaded in, organizing objects in 4 column rows."""
    def __init__(self, parent = None):
        """Default initialization."""
        bar.Bar.__init__(self, parent)

        # Keeps track of greatest height of an item while adding items to row
        self._greatestHeight = 0

        # Signal to emit when object is selected
        self._selectSignal = 'selectedObject'

        # List of paths of objects loaded in
        self._paths = []

    def clear(self):
        """Overrides clear function to set all pos values back to starting."""
        bar.Bar.clear(self)
        self._posX = 0
        self._posY = 0
        self._column = 0
        self._greatestHeight = 0
        del self._paths[:]

    def loadObjects(self, filepaths):
        """Loads Objects from a QStringList of paths to object files."""
        for path in filepaths:
            for loadedPath in self._paths:
                if loadedPath == path:
                    break
            else:
                self.loadObject(path)

    def loadObject(self, filepath):
        self._paths.append(filepath)

        objTree = ElementTree()
        objTree.parse(filepath)
        objRoot = objTree.getroot()

        relAnimPath = objRoot.find('animation').get('path')

        # String cast for QString
        absAnimPath = subInPath(str(filepath), relAnimPath)

        animTree = ElementTree()
        animTree.parse(absAnimPath)
        animRoot = animTree.getroot()

        sheets = animRoot.findall('sheet')

        MAX_COLUMNS = 4
        animNum = 0

        for sheet in sheets:
            sheetPath = sheet.get('path')

            # String cast for QString
            sheetImg = QtGui.QImage(subInPath(str(filepath), sheetPath))

            strips = sheet.findall('strip')

            for strip in strips:
                obj = Object()
                obj.setPath(filepath)
                obj.setAnimNum(animNum)
                obj.setToolTip("Anim Num: " + str(animNum))

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

