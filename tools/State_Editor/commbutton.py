#! /usr/bin/env python 


from PyQt4 import QtCore, QtGui

class ClearButton(QtGui.QPushButton):
    """Emits a 'clear' signal when pressed.

    SIGNALS: 'clear' -- emitted from 'emitClear'

    """
    def __init__(self, parent = None):
        """Set text for button and connect."""
        QtGui.QPushButton.__init__(self, parent)
        self.setText('Clear')
        self.connect(self, QtCore.SIGNAL('clicked()'), self._emitClear)

    def _emitClear(self):
       """Emits 'clear' signal.

       SIGNALS: clear

       """
       self.emit(QtCore.SIGNAL('clear'))


class RemoveButton(QtGui.QPushButton):
    """Button with an X icon on it. Should be used for removal."""
    def __init__(self, parent = None):
        "Sets button icon to X."
        QtGui.QPushButton.__init__(self, parent)
        self.setIcon(QtGui.QIcon("cross.png"))


