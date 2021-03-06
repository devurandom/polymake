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

/** @file TropicalArithmetic.h
  @brief Implementation of classes relevant for tropical computations.
  */

#ifndef POLYMAKE_TROPICAL_ARITHMETIC_H
#define POLYMAKE_TROPICAL_ARITHMETIC_H

#include "polymake/Rational.h"
#include "polymake/TropicalNumber.h"
#include "polymake/Array.h"
#include "polymake/Matrix.h"
#include "polymake/Vector.h"
#include "polymake/Set.h"
#include "polymake/graph/hungarian_method.h"


namespace pm {
   namespace operations {
      template <typename Addition, typename Scalar>
      struct div_skip_zero {
         typedef TropicalNumber<Addition, Scalar> first_argument_type;
         typedef TropicalNumber<Addition, Scalar> second_argument_type;
         typedef const TropicalNumber<Addition, Scalar> result_type;

         result_type operator() (typename function_argument<first_argument_type>::type a, typename function_argument<second_argument_type>::type b) const
         {
            if (is_zero(b)) {
               if (is_zero(a))
                  return TropicalNumber<Addition, Scalar>::zero();
               else if (std::is_same<Addition, Max>::value)
                  return TropicalNumber<Addition, Scalar>(std::numeric_limits<Scalar>::infinity());
               else
                  return TropicalNumber<Addition, Scalar>::dual_zero();
            }
            return a/b;
         }

         template <typename Iterator2>
         const first_argument_type& operator() (partial_left, typename function_argument<first_argument_type>::type a, const Iterator2&) const
         {
            if (is_zero(a))
               return zero_value<TropicalNumber<Addition, Scalar> >();
            else if (std::is_same<Addition, Max>::value)
               return TropicalNumber<Addition, Scalar>::zero();
            else
               return TropicalNumber<Addition, Scalar>::dual_zero();
         }

         template <typename Iterator1>
         const second_argument_type& operator() (partial_right, const Iterator1&, typename function_argument<second_argument_type>::type b) const
         {
            return zero_value<TropicalNumber<Addition, Scalar> >();
         }

      };
   }  }

namespace polymake {
   namespace operations {
      using pm::operations::div_skip_zero; }

   namespace tropical {


     /*
     *
     * @brief compute the tropical sum w.r.t. Addition and the entries where the extremum is attained
     */
        template <typename Addition, typename Scalar, typename VectorTop>
      std::pair< TropicalNumber<Addition, Scalar>, Set<int> > extreme_value_and_index (const GenericVector<VectorTop, TropicalNumber<Addition,Scalar> >& vector)
      {
	typedef TropicalNumber<Addition, Scalar> TNumber;
	TNumber extremum = accumulate(vector.top(), operations::add());
	Set<int> extremal_entries;
	int td_index = 0;
	for(auto td = entire(vector.top()); !td.at_end(); td++, td_index++) {
	  if(*td == extremum) extremal_entries += td_index;
	}
	return std::make_pair(extremum,extremal_entries);
	}

      /*
       * @brief coordinatewise tropical quotient of two vectors with special treatment for 
       * inf entries
       */

      template <typename Vector1, typename Vector2, typename Addition, typename Scalar>
      pm::LazyVector2<const Vector1&, const Vector2&, operations::div_skip_zero<Addition, Scalar> > 
      rel_coord(const GenericVector<Vector1, TropicalNumber<Addition, Scalar> > &point, 
                const GenericVector<Vector2, TropicalNumber<Addition, Scalar> > &apex) {
         return pm::LazyVector2<const Vector1&, const Vector2&, operations::div_skip_zero<Addition, Scalar> >(point.top(), apex.top());
      }

      /*
       * @brief compute a solution of a tropical linear equality as the tropical
       * nearest point projection on the tropical cone generated by the columns of the matrix
       * @param Matrix A 
       * @param Vector b
       * @return solution of the tropical linear equality if existent;
       * if there is no solution the result yields a 'nearest non-solution'
       */
      template <typename Addition, typename Scalar, typename MatrixTop, typename VectorTop>
      Vector<TropicalNumber<Addition, Scalar> > principal_solution(const GenericMatrix<MatrixTop, TropicalNumber<Addition, Scalar> > &A, const GenericVector<VectorTop, TropicalNumber<Addition, Scalar> > &b) {
         typedef TropicalNumber<Addition, Scalar> TNumber;
         int n(A.cols());
         Vector<TNumber> x(n);
         TNumber t_one(TNumber::one());
         for(typename pm::ensure_features<Cols <MatrixTop >, pm::cons<pm::end_sensitive, pm::indexed> >::const_iterator col=entire(ensure(cols(A.top()),(pm::indexed*)0)); !col.at_end(); col++) {
            x[col.index()] = t_one/accumulate(rel_coord(*col, b.top()), operations::add());
         }
         return x;
      }
     
         
      template <typename Addition, typename Scalar, typename MatrixTop>
      std::pair< TropicalNumber<Addition, Scalar>, Array<int> > tdet_and_perm(const GenericMatrix<MatrixTop, TropicalNumber<Addition,Scalar> >& matrix)
      {
         Scalar value(zero_value<Scalar>()); // empty matrix has tropical determinant zero
         const int d(matrix.rows());
         if (d != matrix.cols())
            throw std::runtime_error("input matrix has to be quadratic");
         
         // Checking for zero columns or rows
         for(typename Entire<Cols <MatrixTop > >::const_iterator c = entire(cols(matrix.top())); !c.at_end(); c++) {
            if (is_zero(*c)) return std::make_pair(zero_value<TropicalNumber<Addition, Scalar> >(), Array<int>(sequence(0,d)));
         }
         for(typename Entire<Rows <MatrixTop > >::const_iterator r = entire(rows(matrix.top())); !r.at_end(); r++) {
            if (is_zero(*r)) return std::make_pair(zero_value<TropicalNumber<Addition, Scalar> >(), Array<int>(sequence(0,d)));
         }

         graph::HungarianMethod<Scalar> HM(Addition::orientation()*Matrix<Scalar>(matrix.top()));
         HM.stage();
         return std::make_pair(TropicalNumber<Addition, Scalar>(Addition::orientation()*HM.get_value()), HM.get_matching());
         //return std::make_pair(TropicalNumber<Addition,Scalar>(value),perm);
      }

      template <typename Addition, typename Scalar, typename MatrixTop>
      TropicalNumber<Addition, Scalar> tdet(const GenericMatrix<MatrixTop, TropicalNumber<Addition,Scalar> >& matrix)
      {
         return tdet_and_perm(matrix).first;
      }

      
      template <typename Addition, typename Scalar, typename MatrixTop>
      std::pair< TropicalNumber<Addition, Scalar>, Array<int> > second_tdet_and_perm(const GenericMatrix<MatrixTop, TropicalNumber<Addition,Scalar> >& matrix)
      {
         typedef TropicalNumber<Addition,Scalar> TNumber;
         TNumber value(zero_value<TNumber>()); // empty matrix has tropical determinant zero
         const int d(matrix.rows());
         if (d != matrix.cols())
            throw std::runtime_error("input matrix has to be quadratic");
         
         // Checking for zero columns or rows
         for(typename Entire<Cols <MatrixTop > >::const_iterator c = entire(cols(matrix.top())); !c.at_end(); c++) {
            if (is_zero(*c)) return std::make_pair(zero_value<TNumber >(), Array<int>(sequence(0,d)));
         }
         for(typename Entire<Rows <MatrixTop > >::const_iterator r = entire(rows(matrix.top())); !r.at_end(); r++) {
            if (is_zero(*r)) return std::make_pair(zero_value<TNumber >(), Array<int>(sequence(0,d)));
         }

         const Array<int> perm(tdet_and_perm(matrix).second);//perm(graph::HungarianMethod<Scalar>(Addition::orientation()*Matrix<Scalar>(matrix.top())).stage());

         // successively setting the entries which form the optimal permutation to tropical zero
         // -- this should be replaced by an incremental change of entries resulting in
         // O(n^2) operations per changed entry and O(n^3) in total --
         Matrix<TNumber> modmatrix(matrix.top());
         Array< Array<int> > modperm(d);
         Vector<TNumber> modval(ones_vector<TNumber>(d));
         TNumber oldentry;
         for(int j = 0; j < d; ++j) {
            oldentry = modmatrix(j, perm[j]);
            modmatrix(j, perm[j]) = zero_value<TNumber>();
            modperm[j] = tdet_and_perm(modmatrix).second; //graph::HungarianMethod<Scalar>(Addition::orientation()*Matrix<Scalar>(modmatrix)).stage();
            for(int k = 0; k < d; ++k) modval[j] *= modmatrix(k,modperm[j][k]);
            modmatrix(j, perm[j]) = oldentry;
         }
         value = extreme_value_and_index(modval).first;
         int index = (extreme_value_and_index(modval).second).front();
         return std::make_pair(value,modperm[index]);
      }

      
      template <typename Addition, typename Scalar, typename MatrixTop>
      Vector<TropicalNumber<Addition, Scalar> > cramer(const GenericMatrix<MatrixTop, TropicalNumber<Addition,Scalar> >& matrix)
      {
         // The runtime of the implementation is about O(d^4). 
         // It could be improved to O(d^3) by an incremental approach which does not also
         // recompute the whole tropical determinant.

         const int d(matrix.cols());
         if (d != matrix.rows()+1)
            throw std::runtime_error("input matrix has to have one row less than the column number");

         
         Vector<TropicalNumber<Addition,Scalar> > solvec(d);
         for(int k = 0; k < d; ++k) {
            solvec[k] = tdet(matrix.top().minor(All, ~scalar2set(k)));
         }
         return solvec;
      }


      template <typename Addition, typename Scalar, typename MatrixTop>
      Vector<TropicalNumber<Addition, Scalar> > subcramer(const GenericMatrix<MatrixTop, TropicalNumber<Addition,Scalar> >& matrix, Set<int> J, Set<int> I)
      {
         if (I.size() != J.size()+1)
            throw std::runtime_error("|I| = |J| + 1 is required.");

         Vector<TropicalNumber<Addition,Scalar> > solvec(matrix.cols());
         for(auto i : I) {
            solvec[i] = tdet(matrix.top().minor(J, (I-scalar2set(i))));
         }
         return solvec;
      }


      // tropical distance function; notice that the tropical Addition is not relevant
      template <typename Addition, typename Scalar, typename VectorTop>
      Scalar tdist(const GenericVector< VectorTop, TropicalNumber<Addition, Scalar> >& v, const GenericVector< VectorTop, TropicalNumber<Addition, Scalar> >& w) {
         Vector<Scalar> diff(Vector<Scalar>(v) - Vector<Scalar>(w)); // this is ordinary subtraction
         Scalar min,max;
         for (int i=0; i<diff.dim(); ++i)
            assign_min_max(min,max,diff[i]);
         return max-min;
      }

      // tropical diameter of a simplex; notice that the tropical Addition is not relevant
      template <typename Addition, typename Scalar, typename MatrixTop>
      Scalar tdiam(const GenericMatrix< MatrixTop, TropicalNumber<Addition,Scalar> >& matrix) {
         const int d(matrix.cols());
         Scalar td(zero_value<Scalar>());
         for (int i=0; i<d-1; ++i) {
            for (int k=i+1; k<d; ++k)
               assign_max(td,tdist(matrix.col(i),matrix.col(k)));
         }
         return td;
      }

      
} }

#endif // POLYMAKE_TROPICAL_ARITHMETIC_H

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
