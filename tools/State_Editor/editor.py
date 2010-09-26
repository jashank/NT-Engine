#!/usr/bin/env python

from os.path import basename
import sys
from PyQt4 import QtCore, QtGui
import commbutton, extras, filesys, objbar, tilebar, tilemap


class MainWindow(QtGui.QMainWindow):
    """Holds all components of app and connects their interactions."""
    def __init__(self, parent = None):
        """Sets up components of application."""
        QtGui.QMainWindow.__init__(self, parent)

        winWidth = 1024
        winHeight = 768

        screen = QtGui.QDesktopWidget().availableGeometry()
        screenCenterX = (screen.width() - winWidth) / 2
        screenCenterY = (screen.height() - winHeight) / 2
        self.setGeometry(screenCenterX, screenCenterY, winWidth, winHeight)

        self._currentFile = ''
        self._windowTitleBase = ''

        self._packPrompt()
        self._createComponents()
        self._connectComponents()
        self._layoutComponents()

        mainWidget = QtGui.QWidget()
        mainWidget.setLayout(self._layout)
        mainWidget.setMouseTracking(True)
        self.setCentralWidget(mainWidget)

    def closeEvent(self, closeEvent):
        """Overridden to prompt user for confirmation before closing."""
        msgBox = QtGui.QMessageBox()
        msgBox.setWindowTitle('Exiting NT State Editor')
        msgBox.setText('You have a State open.')
        msgBox.setInformativeText('Do you want to save your changes?')
        msgBox.setStandardButtons(QtGui.QMessageBox.Save |
            QtGui.QMessageBox.Discard | QtGui.QMessageBox.Cancel)
        msgBox.setDefaultButton(QtGui.QMessageBox.Save)

        buttonPress = msgBox.exec_()
        if buttonPress == QtGui.QMessageBox.Cancel:
            closeEvent.ignore()
            return
        elif buttonPress == QtGui.QMessageBox.Save:
            self._save()

    def _packPrompt(self):
        """Opens dialog for user to select Pack they will be working on.

        User must select a pack.

        """
        self._workingPack = QtGui.QFileDialog.getExistingDirectory(self,
            'Choose Pack to work with')

        # Set path to a string because QString is annoying
        self._workingPack = str(self._workingPack)
        if self._workingPack == '':
            raise Exception, "No Pack selected."

        self.setWindowTitle('NT State Editor -- Working in ' +
            basename(self._workingPack))
        self._windowTitleBase = str(self.windowTitle())

    def _createComponents(self):
        """Creates components application will hold."""
        # Object Bar
        self._objBar = objbar.ObjectBar()
        self._objView = QtGui.QGraphicsView(self._objBar)
        self._loadObjects = objbar.LoadObjectsButton(self._workingPack)
        self._clearObjects = commbutton.ClearButton()

        # Tile Bar
        self._tileBar = tilebar.TileBar()
        self._tileBarView = QtGui.QGraphicsView(self._tileBar)
        self._loadTiles = tilebar.LoadTilesButton(self._workingPack)

        # Tile/Object Map
        self._tileMap = tilemap.TileMap()
        self._mapView = QtGui.QGraphicsView(self._tileMap)
        self._setMapDims = tilemap.SetMapDimsButton()
        self._fill = tilemap.FillButton()
        self._clearMap = commbutton.ClearButton()

        # Extras
        self._extras = extras.Extras()
        self._extrasButton = QtGui.QPushButton()
        self._extrasButton.setText('Extras')

        # File Menu
        self._fileMenu = self.menuBar().addMenu("File")
        self._newAction = self._fileMenu.addAction('New')
        self._newAction.setShortcut('Ctrl+N')
        self._loadAction = self._fileMenu.addAction('Open')
        self._loadAction.setShortcut('Ctrl+O')
        self._saveAction = self._fileMenu.addAction('Save')
        self._saveAction.setShortcut('Ctrl+S')
        self._saveAsAction = self._fileMenu.addAction('Save As')
        self._saveAsAction.setShortcut('Ctrl+Shift+S')
        self._quitAction = self._fileMenu.addAction('Quit')
        self._quitAction.setShortcut('Ctrl+Q')

    def _connectComponents(self):
        """Connects interactions between components."""
        # Object Bar - Buttons 
        QtCore.QObject.connect(self._loadObjects,
            QtCore.SIGNAL('selectedFiles'), self._objBar.loadObjects)
        QtCore.QObject.connect(self._clearObjects, QtCore.SIGNAL('clear'),
            self._objBar.clear)

        # Tile Bar - Buttons
        QtCore.QObject.connect(self._loadTiles, QtCore.SIGNAL('selectedFile'),
            self._tileBar.loadTiles)

        # Tile/Object Map - Buttons
        QtCore.QObject.connect(self._setMapDims, QtCore.SIGNAL('gotDims'),
            self._tileMap.setDims)
        QtCore.QObject.connect(self._fill, QtCore.SIGNAL('fill'),
            self._tileMap.fill)
        QtCore.QObject.connect(self._clearMap, QtCore.SIGNAL('clear'),
            self._tileMap.clearPlacements)

        # Bars -> Map
        QtCore.QObject.connect(self._tileBar, QtCore.SIGNAL('selectedTile'),
            self._tileMap.setSelectionTile)
        QtCore.QObject.connect(self._objBar, QtCore.SIGNAL('selectedObject'),
            self._tileMap.setSelectionObject)

        # Bars <-> Bars
        QtCore.QObject.connect(self._tileBar, QtCore.SIGNAL('selectedTile'),
            self._objBar.unselect)
        QtCore.QObject.connect(self._objBar, QtCore.SIGNAL('selectedObject'),
            self._tileBar.unselect)

        # Extras
        QtCore.QObject.connect(self._extrasButton, QtCore.SIGNAL('clicked()'),
            self._extras.exec_)

        # Actions
        QtCore.QObject.connect(self._newAction, QtCore.SIGNAL('triggered()'),
            self._new)
        QtCore.QObject.connect(self._saveAction, QtCore.SIGNAL('triggered()'),
            self._save)
        QtCore.QObject.connect(self._saveAsAction,
            QtCore.SIGNAL('triggered()'), self._saveAs)
        QtCore.QObject.connect(self._loadAction, QtCore.SIGNAL('triggered()'),
            self._open)
        QtCore.QObject.connect(self._quitAction, QtCore.SIGNAL('triggered()'),
            self.close)

    def _layoutComponents(self):
        """Layout components onto a grid."""
        self._layout = QtGui.QGridLayout()
        self._layout.setSpacing(15)

        # Object Bar Area
        self._layout.addWidget(self._objView, 0, 0, 1, 2)
        self._layout.addWidget(self._loadObjects, 1, 0)
        self._layout.addWidget(self._clearObjects, 1, 1)

        # Tile/Object Map Area
        self._layout.addWidget(self._mapView, 0, 2, 1, 3)
        self._layout.addWidget(self._setMapDims, 1, 2)
        self._layout.addWidget(self._fill, 1, 3)
        self._layout.addWidget(self._clearMap, 1, 4)

        # Tile Bar Area
        self._layout.addWidget(self._tileBarView, 0, 5, 1, 3)
        self._layout.addWidget(self._loadTiles, 1, 6)

        # Extras Area
        self._layout.addWidget(self._extrasButton, 3, 3)

    def _new(self):
        """Prompts user for confirmation when they select new."""
        msgBox = QtGui.QMessageBox()
        msgBox.setWindowTitle('Create new State')
        msgBox.setText('You have a State open.')
        msgBox.setInformativeText('Do you want to save your changes?')
        msgBox.setStandardButtons(QtGui.QMessageBox.Save |
            QtGui.QMessageBox.Discard | QtGui.QMessageBox.Cancel)
        msgBox.setDefaultButton(QtGui.QMessageBox.Save)

        buttonPress = msgBox.exec_()
        if buttonPress == QtGui.QMessageBox.Cancel:
            return
        elif buttonPress == QtGui.QMessageBox.Save:
            self._save()

        self._tileMap.clearPlacements()
        self._setMapDims.setDims(0, 0, 0)
        self._objBar.clear()
        self._tileBar.clear()
        self._extras.clearAll()
        self._currentFile = ''
        self.setWindowTitle(self._windowTitleBase)

    def _save(self):
        """If user working on existing file, saves over it.

        Otherwise, a file dialog is brought up.

        """
        if self._currentFile == '':
            filename = filesys.saveAs(self._workingPack, self._tileMap,
                self._extras)
            if filename:
                self._currentFile = filename
        else:
            filename = filesys.save(self._workingPack, self._tileMap,
                self._extras, self._currentFile)
            if filename:
                self._currentFile = filename

        self.setWindowTitle(self._windowTitleBase + ' on ' + self._currentFile)

    def _saveAs(self):
        """Calls saveAs no matter what."""
        filename = filesys.saveAs(self._workingPack, self._tileMap,
            self._extras)
        if filename:
            self._currentFile = filename

        self.setWindowTitle(self._windowTitleBase + ' on ' + self._currentFile)

    def _open(self):
        """Opens state file."""
        filename = filesys.load(self._workingPack, self._tileMap,
            self._setMapDims, self._objBar, self._tileBar, self._extras)
        if filename:
            self._currentFile = filename

        self.setWindowTitle(self._windowTitleBase + ' on ' + self._currentFile)


app = QtGui.QApplication(sys.argv)
mainWindow = MainWindow()
mainWindow.show()
sys.exit(app.exec_())
