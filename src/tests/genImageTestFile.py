#!/usr/bin/env python
# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0


import sys
import os
import re

###################################
def printUsage():
    print("Usage: genImageTestFile.py <dir> <outfile>")
    print("       traverse given dir for expr examples, write tests to outfile\n")
    print("       ex: genImageTestFile.py ./src/demos/imageSynth/examples src/tests/testSeExprExamples.cpp")
    sys.exit()


def translate(string):
    """strip out invalid chars"""
    return string.replace('&', '').replace('.', '').replace('#', '')

###################################

### MAIN ###

if len(sys.argv) < 3:
    printUsage()

# get args
rootdir = sys.argv[1]
outfile = sys.argv[2]

# open outfile
f = open(outfile, 'w')
f.write("#include <gtest/gtest.h>\n")
f.write("void evalExpressionFile(const char *filepath);\n")

for dir_name, sub_dirs, se_files in os.walk(rootdir):
    for se_file in se_files:
        fullpath = os.path.join(dir_name, se_file)
        # use parent_dir and gparent_dir for test name to avoid duplicates
        (head, parent_dir) = os.path.split(dir_name)
        (head, gparent_dir) = os.path.split(head)
        (filename,ext) = os.path.splitext(se_file)
        filename = translate(filename)
        if(re.match('\.se$', ext)):
            f.write("TEST(" + parent_dir.lstrip('.') +'_' + gparent_dir.lstrip('.') + ", " + filename + ")")
            f.write("{")
            f.write("    evalExpressionFile(\"" + fullpath + "\");")
            f.write("}\n")

f.close()
