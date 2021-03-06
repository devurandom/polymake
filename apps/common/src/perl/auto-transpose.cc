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

#include "polymake/Array.h"
#include "polymake/IncidenceMatrix.h"
#include "polymake/Integer.h"
#include "polymake/Matrix.h"
#include "polymake/QuadraticExtension.h"
#include "polymake/Rational.h"
#include "polymake/Set.h"
#include "polymake/SparseMatrix.h"
#include "polymake/Vector.h"
#include "polymake/client.h"

namespace polymake { namespace common { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0>
   FunctionInterface4perl( transpose_X32, T0 ) {
      perl::Value arg0(stack[0]);
      WrapperReturn( (T(arg0.get<T0>())), arg0 );
   };

   FunctionInstance4perl(transpose_X32, perl::Canned< const Matrix< Rational > >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const Matrix< double > >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const IncidenceMatrix< NonSymmetric > >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const Matrix< Integer > >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const SparseMatrix< Rational, NonSymmetric > >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const SparseMatrix< QuadraticExtension< Rational >, NonSymmetric > >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const pm::MatrixMinor<pm::Matrix<pm::Rational> const&, pm::Array<int> const&, pm::all_selector const&> >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const pm::MatrixMinor<pm::SparseMatrix<pm::QuadraticExtension<pm::Rational>, pm::NonSymmetric>&, pm::Set<int, pm::operations::cmp> const&, pm::all_selector const&> >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const Matrix< QuadraticExtension< Rational > > >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const pm::MatrixMinor<pm::Matrix<pm::QuadraticExtension<pm::Rational> > const&, pm::all_selector const&, pm::Series<int, true> const&> >);
   FunctionInstance4perl(transpose_X32, perl::Canned< const pm::MatrixMinor<pm::IncidenceMatrix<pm::NonSymmetric> const&, pm::Set<int, pm::operations::cmp> const&, pm::all_selector const&> >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
