#!/usr/bin/env python

"""Extract setup information from a distutils based project.

The information is outputted on stdout as a raw JSON object tree.
Currently supports the basic project metadata, and C/C++ extensions."""

from __future__ import print_function # Requires Python 2.6+
import sys, os.path, json

object_tree = {}

class Extension(object):
   def __init__(self, *args, **kwargs):
        self.args = args
        self.kwargs = kwargs

class CustomTypesEncoder(json.JSONEncoder):
    def default(self, obj):

        if isinstance(obj, Extension):
            return {'class': 'Extension', 'args': obj.args, 'kwargs': obj.kwargs}

        return json.JSONEncoder.default(self, obj)

def setup(*args, **kwargs):
    object_tree['setup'] = {'function': 'setup', 'args': args, 'kwargs': kwargs}


ERROR_INVALID_ARGUMENTS = 127

if __name__ == '__main__':
    """ """

    # Augment (erm, monkeypatch) the disutils interface with our customization
    import distutils
    import distutils.core
    distutils.core.setup = setup
    distutils.core.Extension = Extension

    # Retrieve file to extract from, and clean sys.argv
    try:
        setup_file = str(sys.argv[1])
        del sys.argv[1]
    except IndexError:
        print("Error: Invalid number of arguments")
        sys.exit(ERROR_INVALID_ARGUMENTS)

    # Execute the setup file with our custom distutils
    exec(open(setup_file).read(), {'distutils': distutils})

    print(json.dumps(object_tree, cls=CustomTypesEncoder))
    sys.exit(0)
