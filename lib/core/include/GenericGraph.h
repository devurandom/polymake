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

/** @file GenericGraph.h
    @brief Implementation of pm::GenericGraph class
*/

#ifndef POLYMAKE_GENERIC_GRAPH_H
#define POLYMAKE_GENERIC_GRAPH_H

#include "polymake/GenericIncidenceMatrix.h"
#include "polymake/GenericMatrix.h"
#include "polymake/TransformedContainer.h"
#include "polymake/CascadedContainer.h"
#include "polymake/GenericIO.h"

namespace pm {

/** @namespace graph
    @brief traits classes related to graphs
*/

namespace graph {

// the values must be consistent with sparse2d::symmetric
struct Undirected : std::true_type {
   typedef Undirected non_multi_type;
   static const bool multigraph=false;
};
struct Directed : std::false_type {
   typedef Directed non_multi_type;
   static const bool multigraph=false;
};

struct UndirectedMulti : std::true_type {
   typedef Undirected non_multi_type;
   static const bool multigraph=true;
};
struct DirectedMulti : std::false_type {
   typedef Directed non_multi_type;
   static const bool multigraph=true;
};

template <typename dir=Undirected> class Graph;

template <typename IteratorRef>
struct uniq_edge_predicate {
   typedef IteratorRef argument_type;
   typedef bool result_type;

   bool operator() (argument_type it) const
   {
      return it.to_node() <= it.from_node();
   }
};

template <typename EdgeContainer>
class uniq_edge_list
   : public modified_container_impl< uniq_edge_list<EdgeContainer>,
                                     mlist< HiddenTag< EdgeContainer >,
                                            IteratorConstructorTag< input_truncator_constructor >,
                                            OperationTag< BuildUnaryIt<uniq_edge_predicate> > > > {
public:
   int dim() const { return this->hidden().dim(); }
};

template <typename TGraph, bool directed=TGraph::is_directed,
          bool TMasquerade=attrib<typename TGraph::out_edge_list_ref>::is_reference>
struct edge_container_impl;

} // end namespace pm::graph

/** @class GenericGraph
    @brief @ref generic "Generic type" for all graph classes.

    Defines various types and constants.
*/

template <typename TGraph, typename dir_val=typename TGraph::dir>
class GenericGraph
   : public Generic<TGraph> {
protected:
   GenericGraph() {}
   GenericGraph(const GenericGraph&) {}

public:
   typedef dir_val dir;
   static const bool is_directed=dir::value==graph::Directed::value;
   static const bool is_multigraph=dir::multigraph;
   typedef GenericGraph generic_type;
   typedef graph::Graph<dir> persistent_type;
   typedef typename Generic<TGraph>::top_type top_type;

   template <typename Result>
   struct rebind_generic {
      typedef GenericGraph<Result, dir> type;
   };

   int nodes() const
   {
      AssertOVERLOADED(GenericGraph, top_type, nodes);
      return this->top().nodes();
   }

   constexpr bool has_gaps() { return false; }

   // Shortcuts for operations on adjacency matrices.

   template <typename TGraph2>
   top_type& operator+= (const GenericGraph<TGraph2>& g2)
   {
      static_assert(!is_multigraph, "in-place modification of a multigraph is not defined");
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator+= - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator+= - not supported for graphs with deleted nodes");
      }
      adjacency_matrix(this->top()) += adjacency_matrix(g2.top());
      return this->top();
   }

   template <typename TGraph2>
   top_type& operator-= (const GenericGraph<TGraph2>& g2)
   {
      static_assert(!is_multigraph, "in-place modification of a multigraph is not defined");
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator-= - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator-= - not supported for graphs with deleted nodes");
      }
      adjacency_matrix(this->top()) -= adjacency_matrix(g2.top());
      return this->top();
   }

   template <typename TGraph2>
   top_type& operator*= (const GenericGraph<TGraph2>& g2)
   {
      static_assert(!is_multigraph, "in-place modification of a multigraph is not defined");
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator*= - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator*= - not supported for graphs with deleted nodes");
      }
      adjacency_matrix(this->top()) *= adjacency_matrix(g2.top());
      return this->top();
   }

   template <typename TGraph2>
   top_type& operator^= (const GenericGraph<TGraph2>& g2)
   {
      static_assert(!is_multigraph, "in-place modification of a multigraph is not defined");
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator^= - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator^= - not supported for graphs with deleted nodes");
      }
      adjacency_matrix(this->top()) ^= adjacency_matrix(g2.top());
      return this->top();
   }

   template <typename TGraph2>
   using graph_overlay_t = graph::Graph<typename std::conditional<dir::value==TGraph2::dir::value, dir, graph::Directed>::type>;

   template <typename TGraph2>
   graph_overlay_t<TGraph2> operator+ (const GenericGraph<TGraph2>& g2) const
   {
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator+ - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator+ - not supported for graphs with deleted nodes");
      }
      return graph_overlay_t<TGraph2>(this->top()) += g2;
   }

   template <typename TGraph2>
   graph::Graph<typename dir::non_multi_type>
   operator- (const GenericGraph<TGraph2>& g2) const
   {
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator- - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator- - not supported for graphs with deleted nodes");
      }
      return graph::Graph<typename dir::non_multi_type>(this->top()) -= g2;
   }

   template <typename TGraph2>
   graph_overlay_t<TGraph2> operator* (const GenericGraph<TGraph2>& g2) const
   {
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator* - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator* - not supported for graphs with deleted nodes");
      }
      return graph_overlay_t<TGraph2>(this->top()) *= g2;
   }

   template <typename TGraph2>
   graph_overlay_t<TGraph2> operator^ (const GenericGraph<TGraph2>& g2) const
   {
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value || !Unwary<TGraph2>::value) {
         if (this->top().nodes() != g2.top().nodes())
            throw std::runtime_error("GenericGraph::operator^ - dimension mismatch");
         if (this->top().has_gaps() || g2.top().has_gaps())
            throw std::runtime_error("GenericGraph::operator^ - not supported for graphs with deleted nodes");
      }
      return graph_overlay_t<TGraph2>(this->top()) ^= g2;
   }

   graph::Graph<typename dir::non_multi_type>
   operator~ () const
   {
      if (POLYMAKE_DEBUG || !Unwary<TGraph>::value) {
         if (this->top().has_gaps())
            throw std::runtime_error("GenericGraph::operator~ - not supported for graphs with deleted nodes");
      }
      graph::Graph<typename dir::non_multi_type> Gcompl(nodes());
      adjacency_matrix(Gcompl) = ~adjacency_matrix(*this);
      return Gcompl;
   }

   template <typename Input> friend
   Input& operator>> (GenericInput<Input>& in, GenericGraph& me)
   {
      return in.top() >> adjacency_matrix(me);
   }

   template <typename Output> friend
   Output& operator<< (GenericOutput<Output>& out, const GenericGraph& me)
   {
      return out.top() << adjacency_matrix(me);
   }

#if POLYMAKE_DEBUG
   void dump() const __attribute__((used)) { cerr << this->top() << std::flush; }
#endif
};

template <typename TGraph>
class Nodes
   : public redirected_container< Nodes<TGraph>,
                                  mlist< ContainerTag< typename TGraph::node_container_ref >,
                                         MasqueradedTop > >
   , public GenericSet<Nodes<TGraph>, int, operations::cmp> {
protected:
   Nodes();
   ~Nodes();
public:
   typename TGraph::node_container_ref get_container()
   {
      return this->hidden().template pretend<typename TGraph::node_container_ref>();
   }
   typename TGraph::const_node_container_ref get_container() const
   {
      return this->hidden().template pretend<typename TGraph::const_node_container_ref>();
   }
};

template <typename TGraph>
class Edges
   : public cascade_impl< Edges<TGraph>,
                          mlist< ContainerTag< typename graph::edge_container_impl<TGraph>::container >,
                                 CascadeDepth< int_constant<2> >,
                                 MasqueradedTop > >
   , public graph::edge_container_impl<TGraph> {
protected:
   Edges();
   ~Edges();
public:
   using graph::edge_container_impl<TGraph>::get_container;
   int size() const { return this->hidden().edges(); }
   int max_size() const { return size(); }
};

namespace graph {

template <typename Top, typename TGraph>
struct edge_container_access {
   typedef typename TGraph::out_edge_list_container container;
   typedef typename TGraph::out_edge_list_container_ref container_ref;
   typedef typename TGraph::const_out_edge_list_container_ref const_container_ref;

   container_ref get_container() { return static_cast<Top*>(this)->hidden().template pretend<container_ref>(); }
   const_container_ref get_container() const { return static_cast<const Top*>(this)->hidden().template pretend<const_container_ref>(); }
};

template <typename TGraph, bool _is_masquerade>
struct edge_container_impl<TGraph, true, _is_masquerade> :
   edge_container_access<Edges<TGraph>, TGraph> {};

template <typename TGraph>
struct edge_container_impl<TGraph, false, true> {
   typedef edge_container_impl<TGraph, true, true> base_t;

   class container
      : public modified_container_impl< container,
                                        mlist< HiddenTag< TGraph >,
                                               ContainerTag< typename base_t::container_ref >,
                                               OperationTag< operations::masquerade<uniq_edge_list> > > >
      , public edge_container_access<container, TGraph> {
   public:
      using edge_container_access<container, TGraph>::get_container;
   };

   container& get_container() { return reinterpret_cast<container&>(static_cast<Edges<TGraph>*>(this)->hidden()); }
   const container& get_container() const { return reinterpret_cast<const container&>(static_cast<const Edges<TGraph>*>(this)->hidden()); }
};

template <typename TGraph>
struct edge_container_impl<TGraph, false, false> {
   typedef edge_container_impl<TGraph, true, false> base_t;
   typedef operations::construct_unary2<TruncatedContainer, BuildUnaryIt<uniq_edge_predicate> > operation;

   class container
      : public modified_container_impl< container,
                                        mlist< HiddenTag< TGraph >,
                                               ContainerTag< typename base_t::container_ref >,
                                               OperationTag< operation > > >
      , public edge_container_access<container, TGraph> {
   public:
      using edge_container_access<container, TGraph>::get_container;
   };

   class const_container
      : public modified_container_impl< const_container,
                                        mlist< HiddenTag< TGraph >,
                                               ContainerTag< typename base_t::const_container_ref >,
                                               OperationTag< operation > > >
      , public edge_container_access<const_container, TGraph> {
   public:
      using edge_container_access<const_container, TGraph>::get_container;
   };

   container& get_container() { return reinterpret_cast<container&>(static_cast<Edges<TGraph>*>(this)->hidden()); }
   const const_container& get_container() const { return reinterpret_cast<const const_container&>(static_cast<const Edges<TGraph>*>(this)->hidden()); }
};

} // end namespace graph

template <typename TGraph, bool TMultigraph=TGraph::is_multigraph>
class AdjacencyMatrix;

template <typename TGraph>
class AdjacencyMatrix<TGraph, false>
   : public GenericIncidenceMatrix< AdjacencyMatrix<TGraph, false> > {
protected:
   AdjacencyMatrix();
   ~AdjacencyMatrix();
public:
   typedef bool value_type;
   typedef bool reference;
   typedef const bool const_reference;

   TGraph& hidden() { return reinterpret_cast<TGraph&>(*this); }
   const TGraph& hidden() const { return reinterpret_cast<const TGraph&>(*this); }
};

template <typename TGraph>
class AdjacencyMatrix<TGraph, true>
   : public GenericMatrix< AdjacencyMatrix<TGraph, true>, int > {
protected:
   AdjacencyMatrix();
   ~AdjacencyMatrix();
public:
   typedef int value_type;
   typedef int reference;
   typedef const int const_reference;

   TGraph& hidden() { return reinterpret_cast<TGraph&>(*this); }
   const TGraph& hidden() const { return reinterpret_cast<const TGraph&>(*this); }
};

template <typename TGraph, bool TMultigraph>
struct check_container_feature< AdjacencyMatrix<TGraph, TMultigraph>, Symmetric >
   : bool_constant< !TGraph::is_directed > {};

template <typename TGraph>
struct check_container_feature< AdjacencyMatrix<TGraph, true>, pure_sparse >
   : std::true_type {};

template <typename TGraph>
class matrix_random_access_methods< AdjacencyMatrix<TGraph, false> > {
public:
   bool operator() (int i, int j) const
   {
      return static_cast<const AdjacencyMatrix<TGraph>&>(*this).hidden().edge_exists(i,j);
   }
};

template <typename TGraph>
class matrix_random_access_methods< AdjacencyMatrix<TGraph, true> > {
public:
   int operator() (int i, int j) const
   {
      return count_it(static_cast<const AdjacencyMatrix<TGraph>&>(*this).hidden().all_edges(i,j));
   }
};

template <typename TGraph, bool TMultigraph>
class Rows< AdjacencyMatrix<TGraph, TMultigraph> >
   : public redirected_container< Rows< AdjacencyMatrix<TGraph, TMultigraph> >,
                                  mlist< ContainerTag< typename TGraph::adjacency_rows_container_ref >,
                                         HiddenTag< TGraph > > > {
public:
   typename TGraph::adjacency_rows_container_ref get_container()
   {
      return this->hidden().template pretend<typename TGraph::adjacency_rows_container_ref>();
   }
   typename TGraph::const_adjacency_rows_container_ref get_container() const
   {
      return this->hidden().template pretend<typename TGraph::const_adjacency_rows_container_ref>();
   }

   int dim() const { return this->hidden().dim(); }

   bool prefer_sparse_representation() const
   {
      return this->hidden().has_gaps();
   }
};

template <typename TGraph, bool TMultigraph>
class Cols< AdjacencyMatrix<TGraph, TMultigraph> >
   : public redirected_container< Cols< AdjacencyMatrix<TGraph, TMultigraph> >,
                                  mlist< ContainerTag< typename TGraph::adjacency_cols_container >,
                                         HiddenTag< TGraph > > > {
public:
   typename TGraph::adjacency_cols_container_ref get_container()
   {
      return this->hidden().template pretend<typename TGraph::adjacency_cols_container_ref>();
   }
   typename TGraph::const_adjacency_cols_container_ref get_container() const
   {
      return this->hidden().template pretend<typename TGraph::const_adjacency_cols_container_ref>();
   }

   int dim() const { return this->hidden().dim(); }

   bool prefer_sparse_representation() const
   {
      return this->hidden().has_gaps();
   }
};

template <typename TGraph, bool TMultigraph>
struct check_container_feature< Rows<AdjacencyMatrix<TGraph, TMultigraph>>, sparse> : std::true_type {};

template <typename TGraph, bool TMultigraph>
struct check_container_feature< Cols<AdjacencyMatrix<TGraph, TMultigraph>>, sparse> : std::true_type {};


template <typename TGraph> inline
Nodes<unwary_t<TGraph>>&
nodes(GenericGraph<TGraph>& G)
{
   return reinterpret_cast<Nodes<unwary_t<TGraph>>&>(G.top());
}

template <typename TGraph> inline
const Nodes<unwary_t<TGraph>>&
nodes(const GenericGraph<TGraph>& G)
{
   return reinterpret_cast<const Nodes<unwary_t<TGraph>>&>(G.top());
}

template <typename TGraph> inline
Edges<unwary_t<TGraph>>&
edges(GenericGraph<TGraph>& G)
{
   return reinterpret_cast<Edges<unwary_t<TGraph>>&>(G.top());
}

template <typename TGraph> inline
const Edges<unwary_t<TGraph>>&
edges(const GenericGraph<TGraph>& G)
{
   return reinterpret_cast<const Edges<unwary_t<TGraph>>&>(G.top());
}

template <typename TGraph> inline
AdjacencyMatrix<unwary_t<TGraph>>&
adjacency_matrix(GenericGraph<TGraph>& G)
{
   return reinterpret_cast<AdjacencyMatrix<unwary_t<TGraph>>&>(G.top());
}

template <typename TGraph> inline
const AdjacencyMatrix<unwary_t<TGraph>>&
adjacency_matrix(const GenericGraph<TGraph>& G)
{
   return reinterpret_cast<const AdjacencyMatrix<unwary_t<TGraph>>&>(G.top());
}

template <typename TGraph> inline
typename unwary_t<TGraph>::out_edge_list_container_ref
out_edge_lists(GenericGraph<TGraph>& G)
{
   return G.top().template pretend<typename unwary_t<TGraph>::out_edge_list_container_ref>();
}
template <typename TGraph> inline
typename unwary_t<TGraph>::const_out_edge_list_container_ref
out_edge_lists(const GenericGraph<TGraph>& G)
{
   return G.top().template pretend<typename unwary_t<TGraph>::const_out_edge_list_container_ref>();
}

template <typename TGraph> inline
typename unwary_t<TGraph>::in_edge_list_container_ref
in_edge_lists(GenericGraph<TGraph>& G)
{
   return G.top().template pretend<typename unwary_t<TGraph>::in_edge_list_container_ref>();
}
template <typename TGraph> inline
typename unwary_t<TGraph>::const_in_edge_list_container_ref
in_edge_lists(const GenericGraph<TGraph>& G)
{
   return G.top().template pretend<typename unwary_t<TGraph>::const_in_edge_list_container_ref>();
}

template <typename TGraph1, typename TGraph2> inline
bool operator== (const GenericGraph<TGraph1>& G1, const GenericGraph<TGraph2>& G2)
{
   if (G1.nodes() != G2.nodes()) return false;
   return adjacency_matrix(G1)==adjacency_matrix(G2);
}

template <typename TGraph1, typename TGraph2> inline
bool operator!= (const GenericGraph<TGraph1>& G1, const GenericGraph<TGraph2>& G2)
{
   return !(G1==G2);
}

struct is_graph;

template <typename TGraph, typename dir>
struct spec_object_traits< GenericGraph<TGraph, dir> >
   : spec_or_model_traits<TGraph, is_opaque> {
   typedef is_graph generic_tag;
   static const bool IO_ends_with_eol=true;
};

template <typename TGraph>
struct spec_object_traits< Nodes<TGraph> >
   : spec_object_traits<is_container> {
   typedef TGraph masquerade_for;
   static const bool is_always_const=true;
};

template <typename TGraph>
struct spec_object_traits< Edges<TGraph> >
   : spec_object_traits<is_container> {
   typedef TGraph masquerade_for;
   static const bool is_always_const=true;
};

template <typename TGraph, bool TMultigraph>
struct spec_object_traits< AdjacencyMatrix<TGraph, TMultigraph> >
   : spec_object_traits<is_container> {
   typedef TGraph masquerade_for;
   static const bool is_always_const=object_traits<TGraph>::is_always_const || TMultigraph;
   static const int is_resizeable=object_traits<TGraph>::is_resizeable;
};

template <typename TGraph>
class WaryGraph : public GenericGraph<Wary<TGraph>, typename TGraph::dir> {
public:
   int out_degree(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::out_degree - node id out of range or deleted");
      return this->top().out_degree(n);
   }
   int in_degree(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::in_degree - node id out of range or deleted");
      return this->top().in_degree(n);
   }
   int degree(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::degree - node id out of range or deleted");
      return this->top().degree(n);
   }

   bool node_exists(int n) const
   {
      if (this->top().node_out_of_range(n))
         throw std::runtime_error("Graph::node_exists - node id out of range");
      return this->top().node_exists(n);
   }
   void delete_node(int n)
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::delete_node - node id out of range or already deleted");
      this->top().delete_node(n);
   }

   int edge(int n1, int n2)
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::edge - node id out of range or deleted");
      return this->top().edge(n1,n2);
   }
   int edge(int n1, int n2) const
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::edge - node id out of range or deleted");
      return this->top().edge(n1,n2);
   }
   int add_edge(int n1, int n2)
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::add_edge - node id out of range or deleted");
      return this->top().add_edge(n1,n2);
   }

   bool edge_exists(int n1, int n2) const
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::edge_exists - node id out of range or deleted");
      return this->top().edge_exists(n1,n2);
   }
   typename TGraph::parallel_edge_iterator all_edges(int n1, int n2)
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::all_edges - node id out of range or deleted");
      return this->top().all_edges(n1,n2);
   }
   typename TGraph::parallel_edge_const_iterator all_edges(int n1, int n2) const
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::all_edges - node id out of range or deleted");
      return this->top().all_edges(n1,n2);
   }

   void delete_edge(int n1, int n2)
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::delete_edge - node id out of range or deleted");
      this->top().delete_edge(n1,n2);
   }
   void delete_all_edges(int n1, int n2)
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::delete_all_edges - node id out of range or deleted");
      this->top().delete_all_edges(n1,n2);
   }

   void contract_edge(int n1, int n2)
   {
      if (this->top().invalid_node(n1) || this->top().invalid_node(n2))
         throw std::runtime_error("Graph::contract_edge - node id out of range or deleted");
      if (n1==n2)
         throw std::runtime_error("Graph::contract_edge - can't contract a loop");
      this->top().contract_edge(n1,n2);
   }

   typename TGraph::out_edge_list_ref out_edges(int n)
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::out_edges - node id out of range or deleted");
      return this->top().out_edges(n);
   }
   typename TGraph::const_out_edge_list_ref out_edges(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::out_edges - node id out of range or deleted");
      return this->top().out_edges(n);
   }

   typename TGraph::in_edge_list_ref in_edges(int n)
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::in_edges - node id out of range or deleted");
      return this->top().in_edges(n);
   }
   typename TGraph::const_in_edge_list_ref in_edges(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::in_edges - node id out of range or deleted");
      return this->top().in_edges(n);
   }

   typename TGraph::out_adjacent_node_list_ref out_adjacent_nodes(int n)
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::out_adjacent_nodes - node id out of range or deleted");
      return this->top().out_adjacent_nodes(n);
   }
   typename TGraph::const_out_adjacent_node_list_ref out_adjacent_nodes(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::out_adjacent_nodes - node id out of range or deleted");
      return this->top().out_adjacent_nodes(n);
   }
   typename TGraph::in_adjacent_node_list_ref in_adjacent_nodes(int n)
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::in_adjacent_nodes - node id out of range or deleted");
      return this->top().in_adjacent_nodes(n);
   }
   typename TGraph::const_in_adjacent_node_list_ref in_adjacent_nodes(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::in_adjacent_nodes - node id out of range or deleted");
      return this->top().in_adjacent_nodes(n);
   }
   typename TGraph::adjacent_node_list_ref adjacent_nodes(int n)
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::adjacent_nodes - node id out of range or deleted");
      return this->top().adjacent_nodes(n);
   }
   typename TGraph::const_adjacent_node_list_ref adjacent_nodes(int n) const
   {
      if (this->top().invalid_node(n))
         throw std::runtime_error("Graph::adjacent_nodes - node id out of range or deleted");
      return this->top().adjacent_nodes(n);
   }

   template <typename TPerm>
   typename std::enable_if<isomorphic_to_container_of<TPerm, int>::value>::type
   permute_nodes(const TPerm& perm)
   {
      if (perm.size() != this->top().dim())
         throw std::runtime_error("Graph::permute_nodes - dimension mismatch");
      this->top().permute_nodes(perm);
   }
};

template <typename GraphRef, template <typename> class Iterator>
class generic_of_GraphComponents {};

template <typename GraphRef, template <typename> class Iterator>
class GraphComponents : public generic_of_GraphComponents<GraphRef, Iterator> {
protected:
   alias<GraphRef> graph;
public:
   typedef typename alias<GraphRef>::arg_type arg_type;
   typedef typename deref<GraphRef>::type graph_t;
   typename alias<GraphRef>::const_reference get_graph() const { return *graph; }

   GraphComponents(arg_type G) : graph(G) {}

   typedef Iterator<graph_t> iterator;
   typedef iterator const_iterator;
   typedef typename iterator::value_type value_type;
   typedef const value_type reference;
   typedef reference const_reference;

   iterator begin() const { return get_graph(); }
   iterator end() const { return iterator(); }
   reference front() const { return *begin(); }
   int size() const { return count_it(begin()); }
   bool empty() const { return get_graph().nodes()==0; }
};

template <typename GraphRef, template <typename> class Iterator>
struct spec_object_traits< GraphComponents<GraphRef, Iterator> >
   : spec_object_traits<is_container> {
   static const bool is_temporary=true, is_lazy=true, is_always_const=true;
};

} // end namespace pm

namespace polymake {
   using pm::GenericGraph;
   using pm::graph::Directed;
   using pm::graph::Undirected;
   using pm::graph::DirectedMulti;
   using pm::graph::UndirectedMulti;
   using pm::Nodes;
   using pm::Edges;
   using pm::AdjacencyMatrix;
}

#endif // POLYMAKE_GENERIC_GRAPH_H

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
