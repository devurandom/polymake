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

#ifndef POLYMAKE_GRAPH_DECORATION_H
#define POLYMAKE_GRAPH_DECORATION_H

#include "polymake/client.h"
#include "polymake/Graph.h"
#include "polymake/Set.h"
#include "polymake/Array.h"
#include "polymake/vector"
#include "polymake/list"
#include <algorithm>

namespace polymake { namespace graph { namespace lattice {

   // A sequential lattice is one in which all nodes are sorted to rank (forwards or backwards).
   // In this case, the list of nodes of given rank is a sequence and can be stored more efficiently.
   struct Sequential : std::true_type {
      typedef std::pair<int,int> map_value_type;
      typedef sequence nodes_of_rank_type;
      typedef const nodes_of_rank_type nodes_of_rank_ref_type; //Intentionally not a reference, as we convert pairs to sequences
      static map_value_type make_map_value_type( const Sequential::map_value_type& x) { return x;}
      static map_value_type make_map_value_type( int a, int b) { return map_value_type(a,b);}
      static bool trivial(const map_value_type& x) { return x.second < x.first; }
      static nodes_of_rank_ref_type map_value_as_container(const map_value_type& x) {
         return nodes_of_rank_ref_type(x.first, x.second - x.first+1);
      }
   };
   // In a nonsequential lattice, no guarantee can be made as to the order of the nodes with respect to their
   // rank.
   struct Nonsequential : std::false_type {
      typedef std::list<int> map_value_type;
      typedef std::list<int> nodes_of_rank_type;
      typedef const nodes_of_rank_type& nodes_of_rank_ref_type;
      static map_value_type make_map_value_type(const Sequential::map_value_type& x) {
         Sequential::nodes_of_rank_ref_type lseq = Sequential::map_value_as_container(x);
         return map_value_type( lseq.begin(), lseq.end() );
      }
      static map_value_type make_map_value_type(int a, int b) {
         return make_map_value_type(Sequential::map_value_type(a,b));
      }
      static bool trivial(const map_value_type& x) { return x.empty(); }
      static nodes_of_rank_ref_type map_value_as_container(const map_value_type& x) { return x; }
   };


   /*
    * This stores, for a given lattice, the inverse map from rank to corresponding set of nodes.
    * It is assumed that nodes are only added or deleted, never modified.
    */
   template <typename SeqType>
   class InverseRankMap{
      protected:

         Map<int, typename SeqType::map_value_type > inverse_rank_map;

         template <typename>
            friend struct pm::spec_object_traits;

      public:
         InverseRankMap() {}
         InverseRankMap(const InverseRankMap& other) : inverse_rank_map(other.inverse_rank_map) {}

         typename SeqType::nodes_of_rank_ref_type nodes_of_rank(int d) const;
         const typename SeqType::nodes_of_rank_type nodes_of_rank_range(int d1, int d2) const;

         void set_rank(int n, int r);

         template <typename NodeList>
            void set_rank_list(int r, const NodeList& l) { inverse_rank_map[r] = l;}

         //void delete_node(int n, int r);
         void delete_node_and_squeeze(int n, int r);

         template <typename Output> friend
            Output& operator<< (GenericOutput<Output>& out, const InverseRankMap& me) {
               out.top() << me.inverse_rank_map;
               return out.top();
            }

         bool operator==(const InverseRankMap& other) const {
            return inverse_rank_map == other.inverse_rank_map;
         }

         const Map<int, typename SeqType::map_value_type >& get_map() const { return inverse_rank_map; }
   };


   // This is the basic data attached to a lattice node: Its face and its rank.
   struct BasicDecoration : public GenericStruct<BasicDecoration> {
      DeclSTRUCT( DeclFIELD(face, Set<int>)
                  DeclFIELD(rank,int) );
		BasicDecoration() {}
		BasicDecoration(Set<int> f, int r) : face(f), rank(r) {}
   };


}}}

namespace pm {


   template <typename SeqType>
      struct spec_object_traits< Serialized< polymake::graph::lattice::InverseRankMap<SeqType> > > : spec_object_traits<is_composite> {

         typedef polymake::graph::lattice::InverseRankMap<SeqType> masquerade_for;

         typedef Map<int, typename SeqType::map_value_type > elements;

         template <typename Me, typename Visitor>
            static void visit_elements(Me &me, Visitor& v) {
               v << me.inverse_rank_map;
            }
      };
}

#endif
