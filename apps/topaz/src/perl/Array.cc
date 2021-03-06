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
#include "polymake/SparseMatrix.h"
#include "polymake/client.h"
#include "polymake/topaz/Filtration.h"
#include "polymake/topaz/HomologyComplex.h"

namespace polymake { namespace topaz { namespace {
///==== Automatically generated contents follow.    Please do not delete this line. ====
   template <typename T0, typename T1>
   FunctionInterface4perl( new_X, T0,T1 ) {
      perl::Value arg0(stack[1]);
      WrapperReturnNew(T0, (arg0.get<T1>()) );
   };

   template <typename T0>
   FunctionInterface4perl( new, T0 ) {
      WrapperReturnNew(T0, () );
   };

   Class4perl("Polymake::common::Array__HomologyGroup__Integer", Array< HomologyGroup< Integer > >);
   Class4perl("Polymake::common::Array__CycleGroup__Integer", Array< CycleGroup< Integer > >);
   Class4perl("Polymake::common::Array__Cell", Array< Cell >);
   FunctionInstance4perl(new, Array< HomologyGroup< Integer > >);
   FunctionInstance4perl(new, Array< CycleGroup< Integer > >);
   OperatorInstance4perl(Binary__eq, perl::Canned< const Array< HomologyGroup< Integer > > >, perl::Canned< const Array< HomologyGroup< Integer > > >);
   OperatorInstance4perl(Binary__eq, perl::Canned< const Array< CycleGroup< Integer > > >, perl::Canned< const Array< CycleGroup< Integer > > >);
   Class4perl("Polymake::common::Array__Pair_A_HomologyGroup__Integer_I_SparseMatrix_A_Integer_I_NonSymmetric_Z_Z", Array< std::pair< HomologyGroup< Integer >, SparseMatrix< Integer, NonSymmetric > > >);
   FunctionInstance4perl(new, Array< std::pair< HomologyGroup< Integer >, SparseMatrix< Integer, NonSymmetric > > >);
   OperatorInstance4perl(Binary__eq, perl::Canned< const Array< std::pair< HomologyGroup< Integer >, SparseMatrix< Integer, NonSymmetric > > > >, perl::Canned< const Array< std::pair< HomologyGroup< Integer >, SparseMatrix< Integer, NonSymmetric > > > >);
   FunctionInstance4perl(new_X, Array< Cell >, int);
   FunctionInstance4perl(new_X, Array< HomologyGroup< Integer > >, perl::Canned< const Array< HomologyGroup< Integer > > >);
///==== Automatically generated contents end here.  Please do not delete this line. ====
} } }
