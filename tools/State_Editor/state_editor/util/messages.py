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

class ConfirmMessage(QtGui.QMessageBox):
    """Box that confirms user choice with various buttons.

    Choices are "save", "don't save", "cancel".

    """
    def __init__(self, title, text, infoText, parent = None):
        """Window is initalized with title, main text, and informative text.

        Arguments: title -- (string) title of window
                   text -- (string) displayed in window as main text
                   infoText -- (string) displayed as extra information in
                               addendum to the main text

        """
        QtGui.QMessageBox.__init__(self, parent)

        self.setWindowTitle(title)
        self.setText(text)
        self.setInformativeText(infoText)
        self.setStandardButtons(QtGui.QMessageBox.Save |
            QtGui.QMessageBox.Discard | QtGui.QMessageBox.Cancel)
        self.setDefaultButton(QtGui.QMessageBox.Save)


class License(QtGui.QDialog):
    """Window showing GPL license."""
    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self, parent)

        self.setWindowTitle("GPL v3")
        self.setMinimumSize(600,800)
        self.setModal(True)

        self._license = QtGui.QTextEdit()
        self._license.insertPlainText(self._loadLicense())
        self._license.setReadOnly(True)
        layout = QtGui.QHBoxLayout()
        layout.addWidget(self._license)
        self.setLayout(layout)

    def _loadLicense(self):
        """Returns string containing GPL v3."""
        try:
            f = open("LICENSE", "r")
            license = ""
            for line in f:
                license += line
            f.close()
            return license
        except:
            return("""
            LICENSE file not found.
            Go to http://www.gnu.org/licenses/gpl.html""")

