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

#include "polymake/Rational.h"
#include "polymake/client.h"
#include "polymake/Array.h"
#include "polymake/Set.h"
#include "polymake/Matrix.h"
#include "polymake/Vector.h"
#include "polymake/linalg.h"

namespace polymake { namespace topaz {

Rational volume(perl::Object p)
{
  const int dim=p.give("DIM");
  const Array< Set<int> > F=p.give("FACETS");
  Matrix<Rational> GR=p.give("COORDINATES");
  Vector<Rational> homogenization(GR.rows(),Rational(1));
  GR = homogenization | GR;
  
  Rational volume(0);
  for (Entire< Array< Set<int> > >::const_iterator f=entire(F); !f.at_end(); ++f) {
    const Rational vol = abs(det( GR.minor(*f,All) ));
    if (vol == 0)
      cerr << "volume: WARNING simplex of volume 0." << endl;
    volume += vol;
  }
  volume /= Integer::fac(dim);
  
  return volume;
}

Function4perl(&volume,"volume");

} }

// Local Variables:
// mode:C++
// c-basic-offset:3
// indent-tabs-mode:nil
// End:
