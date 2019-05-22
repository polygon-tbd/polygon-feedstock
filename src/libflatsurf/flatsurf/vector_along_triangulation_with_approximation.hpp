/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBFLATSURF_VECTOR_ALONG_TRIANGULATION_WITH_APPROXIMATION
#define LIBFLATSURF_VECTOR_ALONG_TRIANGULATION_WITH_APPROXIMATION

#include <boost/operators.hpp>
#include <iosfwd>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"

namespace flatsurf {
template <typename V, typename Approximation>
class VectorAlongTriangulationWithApproximation
    : boost::additive<
          VectorAlongTriangulationWithApproximation<V, Approximation>>,
      boost::additive<
          VectorAlongTriangulationWithApproximation<V, Approximation>,
          HalfEdge>,
      boost::less_than_comparable<
          VectorAlongTriangulationWithApproximation<V, Approximation>, Bound>,
      boost::equality_comparable<
          VectorAlongTriangulationWithApproximation<V, Approximation>> {
 public:
  using Surface = FlatTriangulation<V>;

  VectorAlongTriangulationWithApproximation(const Surface &surface);

  CCW ccw(const VectorAlongTriangulationWithApproximation<V, Approximation>
              &rhs) const;

  template <typename W, typename Approx>
  friend std::ostream &
  operator<<(std::ostream &,
             const VectorAlongTriangulationWithApproximation<W, Approx> &);
  VectorAlongTriangulationWithApproximation<V, Approximation> operator-() const;
  bool operator>(const Bound) const;
  bool operator<(const Bound) const;
  VectorAlongTriangulationWithApproximation<V, Approximation> &
  operator+=(const HalfEdge edge);
  VectorAlongTriangulationWithApproximation<V, Approximation> &
  operator-=(const HalfEdge edge);
  bool operator==(
      const VectorAlongTriangulationWithApproximation<V, Approximation> &)
      const;

  explicit operator V() const;

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif