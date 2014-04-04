/* Copyright (c) 1997-2014
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

#include "polymake/client.h"
#include "polymake/Array.h"
#include "polymake/PowerSet.h"

namespace polymake { namespace matroid {

typedef Array<Set <int> > SArray;

SArray bases_to_circuits(const SArray& bases,const int n)
{
  const int  r=bases[0].size();
  if (r==0) {
    Array<Set<int> > c(n);
    for (int i=0; i<n;++i)
      c[i]=scalar2set(i);
    return c;
  }
  if (r==n) return Array<Set<int> >(0);
  int n_circuits=0;
  std::vector<Set<int> > circuits;
  for (int k=1; k<=r; ++k)
    for (Entire< Subsets_of_k<const sequence&> >::const_iterator j=entire(all_subsets_of_k(sequence(0,n),k));!j.at_end();++j) {
      bool is_circuit=true;
      for (Entire<std::vector<Set<int> > >::const_iterator i=entire(circuits); is_circuit&&!i.at_end();++i)
        if (incl(*i,*j)<=0) is_circuit=false;
      for (Entire<SArray>::const_iterator i = entire(bases); is_circuit&&!i.at_end(); ++i){
        const int l=incl(*i,*j);
        if (l==0||l==1) is_circuit=false;
      }
      if (is_circuit) {
        circuits.push_back(Set<int>(*j));
        ++n_circuits;
      }
    }
  for (Entire< Subsets_of_k<const sequence&> >::const_iterator j=entire(all_subsets_of_k(sequence(0,n),r+1));!j.at_end();++j) {
    bool is_circuit=true;
    for (Entire<std::vector<Set<int> > >::const_iterator i=entire(circuits); is_circuit&&!i.at_end();++i)
      if (incl(*i,*j)<=0) is_circuit=false;
    if (is_circuit) {
      circuits.push_back(Set<int>(*j));
      ++n_circuits;
    }
  }
  return SArray(n_circuits,entire(circuits));
}


SArray bases_to_cocircuits(const SArray& bases,const int n)
{
  const int  r=bases[0].size();
  if (r==0) return Array<Set<int> >(0);
  if (r==n) {
    Array<Set<int> > c(n);
    for (int i=0; i<n;++i)
      c[i]=scalar2set(i);
    return c;
  } 
  int n_cocircuits=0;
  std::vector<Set<int> > cocircuits;
  for (int k=1; k<=n-r+1; ++k)
    for (Entire< Subsets_of_k<const sequence&> >::const_iterator j=entire(all_subsets_of_k(sequence(0,n),k));!j.at_end();++j) {
      bool is_cocircuit=true;
      for (Entire<std::vector<Set<int> > >::const_iterator i=entire(cocircuits); is_cocircuit&&!i.at_end();++i) 
        if (incl(*i,*j)<=0) is_cocircuit=false;
      for (Entire<SArray>::const_iterator i = entire(bases); is_cocircuit&&!i.at_end(); ++i){
        if (((*i)*(*j)).empty()) is_cocircuit=false;
      }
      if (is_cocircuit) {
        cocircuits.push_back(Set<int>(*j));
        ++n_cocircuits;
      }
    }
  return SArray(n_cocircuits,entire(cocircuits));
}


SArray cocircuits_to_bases(const SArray& cocircuits,const int n)
{
  if (cocircuits.empty()) return Array<Set<int> >(1);
  int n_bases=0;
  std::vector<Set<int> > bases;
  int r=n;
  for (int k=1; k<=r; ++k)
    for (Entire< Subsets_of_k<const sequence&> >::const_iterator j=entire(all_subsets_of_k(sequence(0,n),k));!j.at_end();++j) {
      bool is_basis=true;
      for (Entire<SArray>::const_iterator i = entire(cocircuits); is_basis&&!i.at_end(); ++i){
        if (((*i)*(*j)).empty()) is_basis=false;
      }
      if (is_basis) {
        bases.push_back(Set<int>(*j));
        ++n_bases;
        r=k;
      }
    }
  return SArray(n_bases,entire(bases));
}


SArray cocircuits_to_bases_rank(const SArray& cocircuits,const int n,const int rank)
{
  if (cocircuits.empty()) return Array<Set<int> >(1);
  int n_bases=0;
  std::vector<Set<int> > bases;
  for (Entire< Subsets_of_k<const sequence&> >::const_iterator j=entire(all_subsets_of_k(sequence(0,n),rank));!j.at_end();++j) {
    bool is_basis=true;
    for (Entire<SArray>::const_iterator i = entire(cocircuits); is_basis&&!i.at_end(); ++i){
      if (((*i)*(*j)).empty()) is_basis=false;
    }
    if (is_basis) {
      bases.push_back(Set<int>(*j));
      ++n_bases;
    }
  }
  return SArray(n_bases,entire(bases));
}


SArray circuits_to_bases(const SArray& circuits,const int n)
{
  if (circuits.empty()) return Array<Set<int> >(1,Set<int>(sequence(0,n)));
  int n_bases=0;
  std::vector<Set<int> > bases;
  int r=1;
  for (int k=n; k>=r; --k)
    for (Entire< Subsets_of_k<const sequence&> >::const_iterator j=entire(all_subsets_of_k(sequence(0,n),k));!j.at_end();++j) {
      bool is_basis=true;
      for (Entire<SArray>::const_iterator i = entire(circuits); is_basis&&!i.at_end(); ++i){
        if (incl(*i,*j)<=0) is_basis=false;
      }
      if (is_basis) {
        bases.push_back(Set<int>(*j));
        ++n_bases;
        r=k;
      }
    }
  return SArray(n_bases,entire(bases));
}


SArray circuits_to_bases_rank(const SArray& circuits,const int n,const int rank)
{
  if (circuits.empty()) return Array<Set<int> >(1,Set<int>(sequence(0,n)));
  int n_bases=0;
  std::vector<Set<int> > bases;
  for (Entire< Subsets_of_k<const sequence&> >::const_iterator j=entire(all_subsets_of_k(sequence(0,n),rank));!j.at_end();++j) {
    bool is_basis=true;
    for (Entire<SArray>::const_iterator i = entire(circuits); is_basis&&!i.at_end(); ++i){
      if (incl(*i,*j)<=0) is_basis=false;
    }
    if (is_basis) {
      bases.push_back(Set<int>(*j));
      ++n_bases;
    }
  }
  return SArray(n_bases,entire(bases));
}

Function4perl(&bases_to_circuits, "bases_to_circuits");
Function4perl(&bases_to_cocircuits, "bases_to_cocircuits");
Function4perl(&cocircuits_to_bases, "cocircuits_to_bases");
Function4perl(&circuits_to_bases, "circuits_to_bases");
Function4perl(&cocircuits_to_bases_rank, "cocircuits_to_bases_rank");
Function4perl(&circuits_to_bases_rank, "circuits_to_bases_rank");

} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End: