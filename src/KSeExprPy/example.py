#!/bin/env python
# Copyright Disney Enterprises, Inc.  All rights reserved.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License
# and the following modification to it: Section 6 Trademarks.
# deleted and replaced with:
# 
# 6. Trademarks. This License does not grant permission to use the
# trade names, trademarks, service marks, or product names of the
# Licensor and its affiliates, except as required for reproducing
# the content of the NOTICE file.
# 
# You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0

import KSeExprPy
import sys

if __name__=="__main__":
	s="""
	# a cool expression
	a=3;
	if(a>5){
		b=10;
	}else if(a>10){
		b=20;
	}
	c+=10+P;
		map("lame.png")
		+map("foo.png",
			map("blah.png"))+f(g(3,"test"),h("lame"))
	"""
	expr=KSeExprPy.AST(s)
	edits=KSeExprPy.Edits(s)
	def editAllMapsToHavePrefix(node,childs):
		if node.type==KSeExprPy.ASTType.Call: # check node is a call
			if node.value=="map": # check if the call is map
				if childs[0].type==KSeExprPy.ASTType.String: # check if the first argument is a string
					edits.addEdit(childs[0],"testo/%s"%childs[0].value) # add to the edit structure
	KSeExprPy.traverseCallback(expr.root(),editAllMapsToHavePrefix)
	KSeExprPy.printTree(expr,s)
	print(edits.makeNewString())
	
	print "--All Function calls----------------------------"
	def printAllFunctionsAndTheirArguments(node,childs):
		if node.type==KSeExprPy.ASTType.Call:
			print "%s"%node.value
			idx=0
			for child in childs:
				if child.type==KSeExprPy.ASTType.Num:
					edits.addEdit(child,child.value*4)
				print "  Argument %2d %-15s %r"%(idx,child.type,child.value)
				idx+=1
	KSeExprPy.traverseCallback(expr.root(),printAllFunctionsAndTheirArguments)

	print "--All Variables----------------------------"
	def printAllVariableReferences(node,childs):
		if node.type==KSeExprPy.ASTType.Var:
			print "reference %s"%node.value
		elif node.type==KSeExprPy.ASTType.Assign:
			print "assign    %s"%node.value
	KSeExprPy.traverseCallback(expr.root(),printAllVariableReferences)
