#!/bin/env python
# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0

import os
import sys
import KSeExprPy
path="/disney/shows/SHOW/sa/shared/paint3d/expressions/"

success=0
failed=0
total=0

def parseFile(filename):
    global success,failed,total
    try:
        KSeExprPy.AST(open(filename).read())
        success+=1
    except RuntimeError as e:
        print "Parse error in %s"%filename
        print "%s"%e
        failed+=1
    total+=1

if len(sys.argv)==2:
    parseFile(sys.argv[1])
else:
    for root,dirs,files in os.walk(path):
        for f in files:
            if f.endswith(".se"):
                #print f
                filename=os.path.join(root,f)
                parseFile(filename)


print "Parsed %d Success %d Failure %d"%(total,success,failed)
