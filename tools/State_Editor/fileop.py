#!/usr/bin/env python

from os import sep
from os.path import exists
from string import join, split


def subInPath(origPath, subPath):
    """Given original path, find subPath in it, and returns new path.

    An example is best here. If 'origPath' were /foo/bar/a/b/c.xml and
    'subPath' were a/e/f.png, the function would try to locate a/e/f.png in
    the original path, starting from the end (so searching backwards). If
    found it will return the full path (as far back as origPath goes),
    otherwise throwing an exception that 'subPath' wasn't found.

    Arguments: origPath -- original path to search.
               subPath -- path to substitute into original path.

    Returns: New path if subPath subbed into origPath. If the path doesn't
        exist, a "Path subbed in not found." exception will be raised.

    """
    origSplit = split(origPath, sep)
    subSplit = split(subPath, sep)

    subRoot = subSplit[0]
    origSplit.reverse()
    rootIndex = origSplit.index(subRoot)

    origSplit[:rootIndex+1] = []
    origSplit.reverse()

    newPath = join(origSplit, sep)
    newPath += (sep + subPath)

    if not exists(newPath):
        raise Exception, "Path subbed in not found."
    return newPath

