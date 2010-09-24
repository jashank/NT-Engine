#!/usr/bin/env python


from PyQt4 import QtCore, QtGui
from commbutton import RemoveButton


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
            self._clearData)

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
            path = pathLabel.text()
            # 2nd column holds optional name user assigned
            nameField = self.layout().itemAtPosition(row, 2).widget()
            name = nameField.text()

            dataDict[path] = name

        return dataDict

    def _selectFiles(self):
        """Brings up file dialog to select and add music files."""
        filenames = QtGui.QFileDialog.getOpenFileNames(self,
            self._fileDialogTitle, "", self._fileTypes)

        for path in filenames:
            prevLoaded = False
            for row in range(1, self._numRows - 1):
                # 1st column holds QLabel path names
                pathLabel = self.layout().itemAtPosition(row, 1).widget()
                if pathLabel.text() == path:
                    prevLoaded = True
                    break

            if not prevLoaded:
                remove = RemoveButton()
                label = QtGui.QLabel(path)
                label.setScaledContents(True)
                nameEntry = QtGui.QLineEdit()

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

    def _clearData(self):
        """Removes all fields currently loaded in."""
        for row in range(1, self._numRows - 1):
            self._removeRow(row)


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

    def currentState(self):
        """Returns data relevant to current state of Extras.

        Returns: (path, name) dictionaries for music, portals, and fonts.

        """
        musicDict = self._tabs.widget(0).getDataDict()
        portalsDict = self._tabs.widget(1).getDataDict()
        fontsDict = self._tabs.widget(2).getDataDict()

        return musicDict, portalsDict, fontsDict

