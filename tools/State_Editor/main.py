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


import sys
from PyQt4 import QtGui
from state_editor.features.editor import MainWindow


def main(argv=None):
    if argv is None:
        argv = sys.argv
    app = QtGui.QApplication(argv)
    mainWindow = MainWindow()
    mainWindow.show()
    return app.exec_()

if __name__ == "__main__":
    sys.exit(main())
