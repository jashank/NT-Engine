#! /usr/bin/env python


from xml.etree.ElementTree import ElementTree
from PyQt4 import QtCore, QtGui
import barhelp
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


class LoadObjectDirButton(QtGui.QPushButton):
    """When pressed, opens dialog for user to load all objects in a dir.

    SIGNALS: 'selectedDir', dirpath -- emitted from 'selectDir'

    """
    def __init__(self, parent = None):
        """Button is named and when clicked, calls selectDir."""
        QtGui.QPushButton.__init__(self, parent)

        self.setText('Load Object Dir')

        self.connect(self, QtCore.SIGNAL('clicked()'), self.selectDir)

    def selectDir(self):
        """Opens dialog for user to select directory of object(s).

        SIGNALS: 'selectedDir', dirpath -- emitted when directory is selected,
            passing pathname of directory along.

        """
        dirName = QtGui.QFileDialog.getExistingDirectory(self,
            'Select directory of Object files')
        print dirName

        self.emit(QtCore.SIGNAL('selectedDir'), dirName)


class ObjectBar(QtGui.QGraphicsScene):
    """Holds objects loaded in, organizing objects in 5 self._column rows."""
    def __init__(self, parent = None):
        """Default initialization."""
        QtGui.QGraphicsScene.__init__(self, parent)

        # Default opacity for Objects not selected
        self._defOpacity = 0.45

        # Position and column where next item put into scene will go
        self._posX = 0
        self._posY = 0
        self._column = 0

        # Keeps track of greatest height of an item while adding items to row
        self._greatestHeight = 0

    def loadObjects(self, filepaths):
        """Loads Objects from a QStringList of paths to object files."""
        for path in filepaths:
            self.loadObject(path)

    def loadObject(self, filepath):
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

                barhelp.clipFromSheet(sheetImg, strip, obj)

                lnX, lnY = barhelp.setForBar(self._posX, self._posY,
                    self._defOpacity, obj)
                self.addItem(obj)
                self.addLine(lnX)
                self.addLine(lnY)

                if obj.pixmap().height() > self._greatestHeight:
                    self._greatestHeight = obj.pixmap().height()

                self._posX, self._posY, self._column = barhelp.updateGridPos(
                    self._posX, self._posY, self._column, MAX_COLUMNS,
                    obj.pixmap().width(), self._greatestHeight)

                # On a new row
                if self._column == 0:
                    self._greatestHeight = 0

                animNum += 1
