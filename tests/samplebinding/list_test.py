#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# This file is part of the Shiboken Python Bindings Generator project.
#
# Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#
# Contact: PySide team <contact@pyside.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# version 2.1 as published by the Free Software Foundation. Please
# review the following information to ensure the GNU Lesser General
# Public License version 2.1 requirements will be met:
# http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
# #
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301 USA

'''Test cases for std::list container conversions'''

import sys
import unittest

from sample import ListUser

class ExtendedListUser(ListUser):
    def __init__(self):
        ListUser.__init__(self)
        self.create_list_called = False

    def createList(self):
        self.create_list_called = True
        return [2, 3, 5, 7, 13]

class ListConversionTest(unittest.TestCase):
    '''Test case for std::list container conversions'''

    def testReimplementedVirtualMethodCall(self):
        '''Test if a Python override of a virtual method is correctly called from C++.'''
        lu = ExtendedListUser()
        lst = lu.callCreateList()
        self.assert_(lu.create_list_called)
        self.assertEqual(type(lst), list)
        for item in lst:
            self.assertEqual(type(item), int)

    def testPrimitiveConversionInsideContainer(self):
        '''Test primitive type conversion inside conversible std::list container.'''
        cpx0 = complex(1.2, 3.4)
        cpx1 = complex(5.6, 7.8)
        lst = ListUser.createComplexList(cpx0, cpx1)
        self.assertEqual(type(lst), list)
        for item in lst:
            self.assertEqual(type(item), complex)
        self.assertEqual(lst, [cpx0, cpx1])

    def testSumListIntegers(self):
        '''Test method that sums a list of integer values.'''
        lu = ListUser()
        lst = [3, 5, 7]
        result = lu.sumList(lst)
        self.assertEqual(result, sum(lst))

    def testSumListFloats(self):
        '''Test method that sums a list of float values.'''
        lu = ListUser()
        lst = [3.3, 4.4, 5.5]
        result = lu.sumList(lst)
        self.assertEqual(result, sum(lst))

    def testConversionInBothDirections(self):
        '''Test converting a list from Python to C++ and back again.'''
        lu = ListUser()
        lst = [3, 5, 7]
        lu.setList(lst)
        result = lu.getList()
        self.assertEqual(result, lst)

    def testConversionInBothDirectionsWithSimilarContainer(self):
        '''Test converting a tuple, instead of the expected list, from Python to C++ and back again.'''
        lu = ListUser()
        lst = (3, 5, 7)
        lu.setList(lst)
        result = lu.getList()
        self.assertNotEqual(result, lst)
        self.assertEqual(result, list(lst))

if __name__ == '__main__':
    unittest.main()
