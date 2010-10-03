#!/usr/bin/env python

from os.path import basename, exists, relpath
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
    origSplit = split(origPath, '/')
    subSplit = split(subPath, '/')

    subRoot = subSplit[0]
    origSplit.reverse()
    rootIndex = origSplit.index(subRoot)

    origSplit[:rootIndex+1] = []
    origSplit.reverse()

    newPath = join(origSplit, '/')
    newPath += ('/' + subPath)

    if not exists(newPath):
        raise Exception, "Path subbed in not found."
    return newPath

def relPathToPack(workingPack, path):
    """Returns path relative to pack being worked on.

    Arguments: workingPack -- Path to Pack being worked on
               path -- should be part of workingPack

    Returns: Example: relPathPack("/home/mypack",
                          "/home/mypack/thisdir/foo.xml") would return
                      "mypack/thisdir/foo.xml"

    """
    pack = basename(workingPack)
    return(pack + '/' + relpath(path, workingPack))

