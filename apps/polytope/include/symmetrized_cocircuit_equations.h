/* Copyright (c) 1997-2015
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

#ifndef __POLYTOPE_SYMMETRIZED_COCIRCUIT_EQUATIONS_H
#define __POLYTOPE_SYMMETRIZED_COCIRCUIT_EQUATIONS_H

#include "polymake/SparseVector.h"
#include "polymake/ListMatrix.h"
#include "polymake/linalg.h"
#include "polymake/Set.h"
#include "polymake/hash_set"
#include "polymake/group/action_datatypes.h"
#include "polymake/polytope/cocircuit_equations.h"

namespace polymake { namespace polytope {

namespace {

template<typename SparseSet>
auto operator* (const group::SparseSimplexVector<SparseSet>& v,
                const group::SparseSimplexVector<SparseSet>& w)
{
   const auto& smaller = (v.size() < w.size()) ? v : w;
   const auto& larger  = (v.size() < w.size()) ? w : v;

   typename group::SparseSimplexVector<SparseSet>::mapped_type res(0);
   for (const auto& s : smaller) {
      if (!larger.exists(s.first)) continue;
      res += s.second * larger.at(s.first);
   }

   return res;
}

template<typename SetType>
bool is_zero (const group::SparseSimplexVector<SetType>& v)
{
   for (const auto& e : v)
      if (!is_zero(e.second))
         return false;
   return true;
}

template<typename SetType>
auto symmetrize_equation(const group::SparseSimplexVector<SetType>& cocircuit_equation,
                         const group::ConjugacyClasses& conjugacy_classes,
                         const Matrix<Rational>& character_table,
                         const Set<int>& isotypic_components,
                         const group::ActionType<SetType>& h_action)
{
   group::SparseSimplexVector<SetType> new_sparse_eq;
   for (int j=0; j<conjugacy_classes.size(); ++j) {
      Rational coeff_j(0);
      for (const auto& ic : isotypic_components)
         coeff_j += character_table(ic,j) * character_table(ic,0);
      if (is_zero(coeff_j)) continue;

      for (const auto& g : conjugacy_classes[j]) {
         const group::ActionType<SetType> g_action(g);
         for (const auto& e : cocircuit_equation) {
            new_sparse_eq[g_action(h_action(e.first))] += coeff_j * e.second;
         }
      }
   }
   return new_sparse_eq;
}

} // end anonymous namespace

template<typename SetType>
using IndexPair = std::pair<SetType, group::SparseSimplexVector<SetType>>;

template<typename Scalar, typename SetType>
Array<IndexPair<SetType>>
combinatorial_symmetrized_cocircuit_equations_impl(const Matrix<Scalar>& points,
                                                   const Array<SetType>& representative_interior_ridge_simplices,
                                                   const Set<int>& isotypic_components,
                                                   const Matrix<Rational>& character_table,
                                                   const group::ConjugacyClasses& conjugacy_classes,
                                                   const std::string& filename)
{
   std::vector<IndexPair<SetType>> indexed_symmetrized_equations;
   std::ofstream outfile;
   if (filename != "" && filename != "-")
      outfile = std::ofstream(filename.c_str(), std::ios_base::trunc);
   std::ostream& os = (filename == "-" ? perl::cout : outfile);
   for (const auto& ridge_rep : representative_interior_ridge_simplices) {
      const group::SparseSimplexVector<SetType> cocircuit_equation = cocircuit_equation_of_ridge_impl(points, ridge_rep);

      /* 
         for all h in G, calculate the cocircuit equation 

         |Gamma| kappa^{h tau}
         =
         chi_i(id)
         sum_{ Delta in supp(kappa^ tau)} c_Delta
         sum_{j in cC(Gamma)} chi_i(j)
         sum_{g in C} e_{g cdot Delta}

         where  tau = ridge_rep  and  i = *iit, so that  chi_i(j) = character_table(*iit, j)
      */

      hash_set<SetType> ridge_orbit;
      for (const auto& cc : conjugacy_classes) {
         for (const auto& h : cc) {
            const group::ActionType<SetType> h_action(h);
            const SetType tau_prime = h_action(ridge_rep);
            if (ridge_orbit.collect(tau_prime)) continue;

            const group::SparseSimplexVector<SetType> new_sparse_eq(symmetrize_equation(cocircuit_equation, conjugacy_classes, character_table, isotypic_components, h_action));

            if (is_zero(new_sparse_eq)) continue;

            if (filename.length())
               wrap(os) << new_sparse_eq << endl;
            else
               indexed_symmetrized_equations.emplace_back(tau_prime, new_sparse_eq);
         }
      }
   }
   return Array<IndexPair<SetType>>(indexed_symmetrized_equations);
}

template<typename Scalar, typename SetType>
perl::Object
projected_symmetrized_cocircuit_equations_impl_impl(const Matrix<Scalar>& points,
                                                    const Array<SetType>& representative_interior_ridge_simplices,
                                                    const Set<int>& isotypic_components,
                                                    const Matrix<Rational>& character_table,
                                                    const group::ConjugacyClasses& conjugacy_classes,
                                                    const group::SparseIsotypicBasis<SetType>& isotypic_basis,
                                                    bool reduce_equations)
{
   const int isotypic_dim(isotypic_basis.size());
   ListMatrix<SparseVector<Rational>>
      eqs(0, isotypic_dim),
      kernel_so_far(unit_matrix<Rational>(isotypic_dim));
   std::vector<SetType> ridges;

   for (const auto& i_and_e : combinatorial_symmetrized_cocircuit_equations_impl(points, representative_interior_ridge_simplices, isotypic_components, character_table, conjugacy_classes, "")) {

      SparseVector<Rational> new_eq(isotypic_dim);
      for (int i=0; i<isotypic_dim; ++i)
         new_eq[i] = i_and_e.second * isotypic_basis[i];
      if (is_zero(new_eq)) continue;

      if (!reduce_equations) {
         ridges.emplace_back(i_and_e.first);
         eqs /= new_eq;
      } else {
         if (add_row_if_rowspace_increases(eqs, new_eq, kernel_so_far))
            ridges.emplace_back(i_and_e.first);
      }
   }
   perl::Object pce_out("SymmetrizedCocircuitEquations");
   pce_out.take("ISOTYPIC_COMPONENTS") << isotypic_components;
   pce_out.take("RIDGES") << ridges;
   pce_out.take("PROJECTED_EQUATIONS") << eqs;
   return pce_out;
}

} }

#endif // __POLYTOPE_SYMMETRIZED_COCIRCUIT_EQUATIONS_H

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:

 