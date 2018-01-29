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

#include "polymake/Array.h"
#include "polymake/IncidenceMatrix.h"
#include "polymake/Matrix.h"
#include "polymake/Rational.h"
#include "polymake/Set.h"
#include "polymake/TropicalNumber.h"
#include "polymake/Vector.h"
#include "polymake/linalg.h"

namespace polymake { namespace tropical { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0>
   FunctionInterface4perl( dual_description_X, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturn( (dual_description(arg0.get<T0>())) );
   };

   template <typename T0, typename T1, typename T2>
   FunctionInterface4perl( is_contained_X_X_X, T0,T1,T2 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]);
      WrapperReturn( (is_contained(arg0.get<T0>(), arg1.get<T1>(), arg2.get<T2>())) );
   };

   template <typename T0, typename T1, typename T2>
   FunctionInterface4perl( monoextremals_X_X_X, T0,T1,T2 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]);
      WrapperReturn( (monoextremals(arg0.get<T0>(), arg1.get<T1>(), arg2.get<T2>())) );
   };

   template <typename T0>
   FunctionInterface4perl( extremals_from_generators_X, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturn( (extremals_from_generators(arg0.get<T0>())) );
   };

   template <typename T0, typename T1, typename T2>
   FunctionInterface4perl( intersection_extremals_X_X_X, T0,T1,T2 ) {
      perl::Value arg0(stack[0]), arg1(stack[1]), arg2(stack[2]);
      WrapperReturn( (intersection_extremals(arg0.get<T0>(), arg1.get<T1>(), arg2.get<T2>())) );
   };

   FunctionInstance4perl(intersection_extremals_X_X_X, perl::Canned< const Matrix< TropicalNumber< Min, Rational > > >, perl::Canned< const Vector< TropicalNumber< Min, Rational > > >, perl::Canned< const Vector< TropicalNumber< Min, Rational > > >);
   FunctionInstance4perl(extremals_from_generators_X, perl::Canned< const Matrix< TropicalNumber< Min, Rational > > >);
   FunctionInstance4perl(monoextremals_X_X_X, perl::Canned< const pm::DiagMatrix<pm::SameElementVector<pm::TropicalNumber<pm::Min, pm::Rational> const&>, true> >, perl::Canned< const Matrix< TropicalNumber< Min, Rational > > >, perl::Canned< const Vector< Rational > >);
   FunctionInstance4perl(is_contained_X_X_X, perl::Canned< const Vector< TropicalNumber< Max, Rational > > >, perl::Canned< const Matrix< TropicalNumber< Max, Rational > > >, perl::Canned< const Array< Set< int > > >);
   FunctionInstance4perl(dual_description_X, perl::Canned< const Matrix< Rational > >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }