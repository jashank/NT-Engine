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
