/* Copyright (c) 1997-2018
   Ewgenij Gawrilow, Michael Joswig (Technische Universitaet Berlin, Germany)
   http://www.polymake.org

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version: http://www.gnu.org/licenses/gpl.txt.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
--------------------------------------------------------------------------------
*/

namespace polymake { namespace polytope { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   FunctionWrapper4perl( void (perl::Object, perl::Object, pm::Set<int, pm::operations::cmp> const&, int) ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]), arg3(stack[3]);
      IndirectWrapperReturnVoid( arg0, arg1, arg2.get< perl::TryCanned< const Set< int > > >(), arg3 );
   }
   FunctionWrapperInstance4perl( void (perl::Object, perl::Object, pm::Set<int, pm::operations::cmp> const&, int) );

   FunctionWrapper4perl( void (pm::Array<int> const&, pm::IncidenceMatrix<pm::NonSymmetric> const&, perl::Object) ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]);
      IndirectWrapperReturnVoid( arg0.get< perl::TryCanned< const Array< int > > >(), arg1.get< perl::TryCanned< const IncidenceMatrix< NonSymmetric > > >(), arg2 );
   }
   FunctionWrapperInstance4perl( void (pm::Array<int> const&, pm::IncidenceMatrix<pm::NonSymmetric> const&, perl::Object) );

///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
