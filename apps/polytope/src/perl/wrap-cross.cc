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

#include "polymake/QuadraticExtension.h"
#include "polymake/Rational.h"

namespace polymake { namespace polytope { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0, typename T1>
   FunctionInterface4perl( cross_T_int_C_o, T0,T1 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]);
      WrapperReturn( (cross<T0>(arg0.get<int>(), arg1.get<T1, T0>(), arg2)) );
   };

   FunctionWrapper4perl( perl::Object (int, Rational const&) ) {
      perl::Value arg0(stack[0]), arg1(stack[1]);
      IndirectWrapperReturn(arg0, arg1.get< perl::TryCanned< const Rational > >());
   }
   FunctionWrapperInstance4perl( perl::Object (int, Rational const&) );

   FunctionWrapper4perl( perl::Object (int, pm::Rational const&, perl::OptionSet) ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]);
      IndirectWrapperReturn( arg0, arg1.get< perl::TryCanned< const Rational > >(), arg2 );
   }
   FunctionWrapperInstance4perl( perl::Object (int, pm::Rational const&, perl::OptionSet) );

   FunctionInstance4perl(cross_T_int_C_o, QuadraticExtension< Rational >, perl::Canned< const QuadraticExtension< Rational > >);
   FunctionInstance4perl(cross_T_int_C_o, Rational, perl::Canned< const Rational >);
   FunctionInstance4perl(cross_T_int_C_o, Rational, int);
   FunctionInstance4perl(cross_T_int_C_o, QuadraticExtension< Rational >, int);
   FunctionInstance4perl(cross_T_int_C_o, double, double);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
