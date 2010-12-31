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


from PyQt4 import QtCore, QtGui
from state_editor.util.commbutton import RemoveButton


class ExtrasTab(QtGui.QWidget):
    """Widget for selecting files for a state and assigning names to them.

    This can be used for Music, Portals, and Fonts because they all have
    the same data processing requirements.
    """
    def __init__(self, fileDialogTitle = "Select Files", fileTypes = "",
                 parent = None):
        """Initialize original components.

        Arguments: fileDialogTitle - Title of file dialog when opened

                   fileTypes -- Allow user to specify file types user is able
                                to select from (ex: "*.ogg *.wav *.flac" )

        """
        QtGui.QWidget.__init__(self, parent)

        self._fileDialogTitle = fileDialogTitle
        self._fileTypes = fileTypes

        # Layout is an n x 3 grid. Starts off with 2 rows. rowCount() in
        # grid layout doesn't suffice because it can't be updated (easily)
        # when rows are removed.
        layout = QtGui.QGridLayout()
        layout.setColumnMinimumWidth(2, 100)
        self._numColumns = 3
        self._numRows = 2

        bold = QtGui.QFont()
        bold.setBold(True)
        path = QtGui.QLabel('Path')
        path.setFont(bold)
        name = QtGui.QLabel('Name')
        name.setFont(bold)

        # dummy widget put in for easier location calculations via indexOf
        layout.addWidget(QtGui.QWidget(), 0, 0)
        layout.addWidget(path, 0, 1)
        layout.addWidget(name, 0, 2)

        self._clear = QtGui.QPushButton()
        self._clear.setText('Clear')
        self._clear.connect(self._clear, QtCore.SIGNAL('clicked()'),
            self.clearData)

        self._add = QtGui.QPushButton()
        self._add.setText('Add')
        self._add.connect(self._add, QtCore.SIGNAL('clicked()'),
            self._selectFiles)

        layout.addWidget(self._clear, 1, 0)
        layout.addWidget(self._add, 1, 1)

        self.setLayout(layout)

    def getDataDict(self):
        """Returns dictionary of (path, name) data currently stored."""
        dataDict = dict()
        for row in range(1, self._numRows - 1):
            # 1st column holds QLabel path names
            pathLabel = self.layout().itemAtPosition(row, 1).widget()
            path = str(pathLabel.text())
            # 2nd column holds optional name user assigned
            nameField = self.layout().itemAtPosition(row, 2).widget()
            name = str(nameField.text())

            dataDict[path] = name

        return dataDict

    def addEntry(self, path, name):
        """Adds an entry (new row) to the tab.

        Arguments: path - path of file for entry
                   name - user-specified name for entry

        """
        remove = RemoveButton()
        label = QtGui.QLabel(path)
        label.setScaledContents(True)
        nameEntry = QtGui.QLineEdit(name)

        self.layout().removeWidget(self._clear)
        self.layout().removeWidget(self._add)

        self.layout().addWidget(remove, self._numRows - 1, 0)
        self.layout().addWidget(label, self._numRows - 1, 1)
        self.layout().addWidget(nameEntry, self._numRows - 1, 2)

        # Remove row that remove button is in when it is clicked
        remove.connect(remove, QtCore.SIGNAL('clicked()'),
            lambda row =
                (self.layout().indexOf(remove) / self._numColumns):
                    self._removeRow(row))


        self.layout().addWidget(self._clear, self._numRows, 0)
        self.layout().addWidget(self._add, self._numRows, 1)
        self._numRows += 1

    def clearData(self):
        """Removes all fields currently loaded in."""
        for row in range(1, self._numRows - 1):
            self._removeRow(row)


    def _selectFiles(self):
        """Brings up file dialog to select and add files."""
        filenames = QtGui.QFileDialog.getOpenFileNames(self,
            self._fileDialogTitle, "", self._fileTypes)

        for path in filenames:
            path = str(path)

            prevLoaded = False
            for row in range(1, self._numRows - 1):
                # 1st column holds QLabel path names
                pathLabel = self.layout().itemAtPosition(row, 1).widget()
                if pathLabel.text() == path:
                    prevLoaded = True
                    break

            if not prevLoaded:
                self.addEntry(path, "")

            prevLoaded = False

    def _removeRow(self, row):
        """Removes row in grid layout corresponding to row number passed.

        Arguments: row -- Number of row (starting at 0) to remove

        """
        for column in range(0, self._numColumns):
            item = self.layout().itemAtPosition(row, column)
            widget = item.widget()
            self.layout().removeItem(item)
            self.layout().removeWidget(widget)

            # otherwise widget will continue to be visible
            widget.setParent(None)

        self._numRows -= 1


class Extras(QtGui.QDialog):
    """A tabbed dialog for specification of music, portals, and fonts."""
    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self, parent)

        self.setWindowTitle("Extras")
        self.setModal(True)

        self._tabs = QtGui.QTabWidget()

        self._tabs.addTab(ExtrasTab('Select music file(s)', """*.ogg *.wav
            *.flac *.aiff *.au *.raw *.paf *.svx *.nist *.voc *.ircam *.w64
            *.mat4 *.mat5 *.pvf *.htk *.sds *.avr *.sd2 *.caf *.wve *.mpc2k
            *.rf64"""), "Music")

        self._tabs.addTab(ExtrasTab('Select state file(s)', '*.xml'), "Portals")

        self._tabs.addTab(ExtrasTab('Select font file(s)', """*.ttf *.ttc *.tte
            *.pfm *.pfb *.pfa *.afm *.otf *.otc *.pcf *.fnt *.bdf *.pfr"""),
            "Fonts")

        self._ok = QtGui.QPushButton()
        self._ok.setText("OK")
        self._ok.connect(self._ok, QtCore.SIGNAL('clicked()'),
            lambda val=1: self.done(val))

        layout = QtGui.QVBoxLayout()
        layout.addWidget(self._tabs)
        layout.addWidget(self._ok)
        self.setLayout(layout)

    def getMusic(self):
        """Returns dictionary containing (path,name) pairs for music."""
        return self._tabs.widget(0).getDataDict()

    def getPortals(self):
        """Returns dictionary containing (path,name) pairs for portals."""
        return self._tabs.widget(1).getDataDict()

    def getFonts(self):
        """Returns dictionary containing (path,name) pairs for fonts."""
        return self._tabs.widget(2).getDataDict()

    def addSong(self, path, name):
        """Adds song to music tab.

        Arguments: path - path to the music file
                   name - shorthand name given to song file

        """
        self._tabs.widget(0).addEntry(path, name)

    def addPortal(self, path, name):
        """Adds portal to portals tab.

        Arguments: path - path to the state file
                   name - shorthand name given to state file

        """
        self._tabs.widget(1).addEntry(path, name)

    def addFont(self, path, name):
        """Adds font to fonts tab.

        Arguments: path - path to the font file
                   name - shorthand name given to font file

        """
        self._tabs.widget(2).addEntry(path, name)

    def clearAll(self):
        """Clears data from all tabs."""
        for i in range(0, self._tabs.count()):
            self._tabs.widget(i).clearData()

