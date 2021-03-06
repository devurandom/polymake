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

#include "polymake/group/group_tools.h"
#include "polymake/group/orbit.h"
#include <algorithm>

namespace polymake { namespace group {

namespace {

template<typename Element>
GroupIndex<Element> make_group_classes(perl::Object g,
                                       perl::OptionSet options,
                                       Array<Array<Element>>& group_classes)
{
   const std::string action = options["action"];
   try {
      g.give(action + ".CONJUGACY_CLASSES") >> group_classes;
   } catch (const perl::undefined&) {
      const Array<Element> gens = g.give(action + ".GENERATORS");
      group_classes = Array<Array<Element>>(1);
      group_classes[0] = orbit<on_container>(gens, identity(degree(gens[0]), Element()));
      std::sort(group_classes[0].begin(), group_classes[0].end(), pm::operations::lt<const Array<int>&, const Array<int>&>());
   }
   return group_index(group_classes);
}

} // end anonymous namespace

auto
group_right_multiplication_table(perl::Object g, perl::OptionSet options)
{
   Array<Array<Array<int>>> group_classes;
   const GroupIndex<Array<int>> group_index = make_group_classes(g, options, group_classes);
   return group_right_multiplication_table_impl(group_classes, group_index);
}

auto
group_left_multiplication_table(perl::Object g, perl::OptionSet options)
{
   Array<Array<Array<int>>> group_classes;
   const GroupIndex<Array<int>> group_index = make_group_classes(g, options, group_classes);
   return group_left_multiplication_table_impl(group_classes, group_index);
}
      
UserFunction4perl("# @category Symmetry"
                  "# Calculate the right multiplication table of a group action, in which GMT[g][h] = gh"
                  "# @param Group G"
                  "# @option String action which action to take for the calculation; default PERMUTATION_ACTION"
                  "# @return Array<Array<Int>> GMT the multiplication table, where the elements of //G// are"
                  "# ordered by conjugacy classes (if available), else in generated order",
                  &group_right_multiplication_table,
                  "group_right_multiplication_table(Group { action=>'PERMUTATION_ACTION' })");

UserFunction4perl("# @category Symmetry"
                  "# Calculate the left multiplication table of a group action, in which GMT[g][h] = hg"
                  "# @param Group G"
                  "# @option String action which action to take for the calculation; default PERMUTATION_ACTION"
                  "# @return Array<Array<Int>> GMT the multiplication table, where the elements of //G// are"
                  "# ordered by conjugacy classes (if available), else in generated order",
                  &group_left_multiplication_table,
                  "group_left_multiplication_table(Group { action=>'PERMUTATION_ACTION' })");
 
    
} }


// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:

