#!/bin/env python
# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0

"""
KSeExprPy Python Bindings of KSeExprPy

Getting more help:

help(KSeExprPy._KSeExprPy) 
  - to get info core classes
help(KSeExprPy.utils)
  - interesting helper functions/classes

"""

from __future__ import absolute_import
import sys

from .core import AST as _AST, ASTHandle, ASTType
from .utils import (
    getComment, Edits, traverseCallback, traverseFilter, printTree
)

_PY2 = sys.version_info[0] == 2
try:
    ustr = unicode
except NameError:
    ustr = str


def _encode(obj, encoding='utf-8'):
    # The Python2 Boost.Python API requires byte strings
    if _PY2 and isinstance(obj, ustr):
        value = obj.encode(encoding)
    else:
        value = obj
    return value


# Override AST to handle unicode vs. str constructor values
class AST(_AST):
    def __init__(self, string):
        super(AST, self).__init__(_encode(string))
