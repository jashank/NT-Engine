#!/usr/bin/env python


from PyQt4 import QtGui

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

