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

///==== this line controls the automatic file splitting: max.instances=40

#include "polymake/Integer.h"
#include "polymake/Polynomial.h"
#include "polymake/QuadraticExtension.h"
#include "polymake/Rational.h"
#include "polymake/RationalFunction.h"
#include "polymake/TropicalNumber.h"
#include "polymake/client.h"

namespace polymake { namespace common { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0, typename T1>
   FunctionInterface4perl( pow_X_f1, T0,T1 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]);
      WrapperReturn( arg0.get<T0>().pow(arg1.get<T1>()) );
   };

   template <typename T0, typename T1>
   FunctionInterface4perl( Integer__pow_X_X, T0,T1 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]);
      WrapperReturn( (Integer::pow(arg0.get<T0>(), arg1.get<T1>())) );
   };

   template <typename T0, typename T1>
   FunctionInterface4perl( Rational__pow_X_X, T0,T1 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]);
      WrapperReturn( (Rational::pow(arg0.get<T0>(), arg1.get<T1>())) );
   };

   FunctionInstance4perl(Integer__pow_X_X, perl::Canned< const Integer >, long);
   FunctionInstance4perl(Rational__pow_X_X, perl::Canned< const Rational >, long);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const Polynomial< Rational, int > >, int);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const UniPolynomial< Rational, int > >, int);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const UniPolynomial< UniPolynomial< Rational, int >, Rational > >, int);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const UniPolynomial< UniPolynomial< Rational, int >, Rational > >, perl::Canned< const Rational >);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const UniPolynomial< TropicalNumber< Min, Rational >, int > >, int);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const UniPolynomial< TropicalNumber< Max, Rational >, int > >, int);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const UniPolynomial< QuadraticExtension< Rational >, int > >, int);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const Polynomial< QuadraticExtension< Rational >, int > >, perl::Canned< const QuadraticExtension< Rational > >);
   FunctionInstance4perl(pow_X_f1, perl::Canned< const Polynomial< QuadraticExtension< Rational >, int > >, int);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
