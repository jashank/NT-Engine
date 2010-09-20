#!/usr/bin/env python


from PyQt4 import QtCore, QtGui


class SoundList(QtGui.QDialog):
    """Window for entering sounds for the state."""
    def __init__(self, parent = None):
        """Initial componenets are buttons to add sound and to finish."""
        QtGui.QDialog.__init__(self, parent)

        self.setModal(True)
        self.setWindowTitle('Sound List')

        addSound = QtGui.QPushButton()
        addSound.setText('Add Sound(s)')
        addSound.connect(addSound, QtCore.SIGNAL('clicked()'),
            self._selectFiles)

        ok = QtGui.QPushButton()
        ok.setText('OK')
        ok.connect(ok, QtCore.SIGNAL('clicked()'), lambda val=1: self.done(val))

        layout = QtGui.QFormLayout()
        layout.addRow(addSound, ok)
        self.setLayout(layout)

    def _selectFiles(self):
        """Brings up file dialog to select and add sound files."""
        filenames = QtGui.QFileDialog.getOpenFileNames(self,
            'Select sound file(s)', "", """*.ogg *.wav *.flac *.aiff *.au *.raw
                *.paf *.svx *.nist *.voc *.ircam *.w64 *.mat4 *.mat5 *.pvf *.htk
                *.sds *.avr *.sd2 *.caf *.wve *.mpc2k *.rf64""")

        for path in filenames:
            label = QtGui.QLabel(path)
            remove = QtGui.QPushButton(QtGui.QIcon("cross.png"), "")
            self.layout().insertRow(self.layout().rowCount() - 1, remove, label)


class SoundButton(QtGui.QPushButton):
    """Brings up held sound list when pressed."""
    def __init__(self, parent = None):
        """Initializes name and connections."""
        QtGui.QPushButton.__init__(self, parent)
        self.setText('Sound')

        self._soundList = SoundList()
        self.connect(self, QtCore.SIGNAL('clicked()'), self._soundList.exec_)

