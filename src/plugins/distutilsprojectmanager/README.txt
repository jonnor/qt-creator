Current status:
Can open and parse simple pure Python distutils projects,
and enumerate its files in the project tree.
Project options for build steps and toolchain is available, but
not yet correct.

== TODO ==

Milestone 1: Decent support for pure Python projects, first merge request
* Fix: Projectfile listing does not show subfolders
* Fix: Build step is for makefiles, not distutils
* Improve setup.py parsing to also do package_data and data_files
* Support different build directories (--build-base mydirectory)
* Remove dialog querying for builddir on startup, the default (build/) is fine for most
* Add a clean step for distutils
* Allow to set up a simple Python "toolchain", at least for specifying the interpreter to use
* Fix: Lookup of distutils_extract.py
* Clean out all spurious debug statements, and references to autotools
* Add statement saying the code was derived from autootools projectmanager?
* Maybe: refactor the common code to build up the node tree
(shared by distutils and autotools at least, maybe more)
* Write some tests for the parsing code
* Decide on copyright holder, me or Openismus
* Create merge request. Daniel Molkentin and Tobias Hunger as reviewers?
* Write blogpost, notify pyside people and request their feedback/testing

Milestone 2: Decent support for pyre Python PySide projects
* Add PySide templates for getting started (separate plugin)?

Milestone X: Decent support for C/C++ extensions
* Improve setup.py parsing to also do C/C++ extension
* Allow to set up C/C++ toolchain for the extensions
* Find out how to do QObject stuff with distutils

Milestone Y: Advanced Python IDE features
* Checking for syntax errors and common mistakes while editing
* Autocompletion of attributes
