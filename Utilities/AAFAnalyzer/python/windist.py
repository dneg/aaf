
# Generate the analyzerhtml distribution as follows:
#
#      /cygdrive/c/Python25/python ./windist.py py2exe

from distutils.core import setup
import py2exe

setup(console=['analyzerhtml_shortcut.py'])
