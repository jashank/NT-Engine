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
        self.setIcon(QtGui.QIcon("state_editor/media/cross.png"))


