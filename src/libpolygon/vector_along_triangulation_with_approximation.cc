/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Polygon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Polygon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <optional>
#include <ostream>
#include <vector>

#include "libpolygon/bound.hpp"
#include "libpolygon/flat_triangulation.hpp"
#include "libpolygon/half_edge.hpp"
#include "libpolygon/half_edge_map.hpp"
#include "libpolygon/vector_along_triangulation.hpp"
#include "libpolygon/vector_along_triangulation_with_approximation.hpp"

using std::optional;
using std::ostream;
using std::vector;

namespace flatsurf {
template <typename V, typename Approximation>
struct VectorAlongTriangulationWithApproximation<
    V, Approximation>::Implementation {
  using Surface = FlatTriangulation<V>;

  explicit Implementation(const Surface& surface)
      : surface(surface),
        coefficients(vector<int>(surface.nedges)),
        approximation() {}

  V exact() const {
    V v = static_cast<V>(VectorAlongTriangulation<V>(surface, coefficients));
    approximation = static_cast<const Approximation&>(v);
    return v;
  }

  const Surface& surface;
  HalfEdgeMap<int> coefficients;
  mutable Approximation approximation;
};

template <typename V, typename Approximation>
VectorAlongTriangulationWithApproximation<V, Approximation>::
    VectorAlongTriangulationWithApproximation(const Surface& surface)
    : impl(spimpl::make_impl<Implementation>(surface)) {}

template <typename V, typename Approximation>
VectorAlongTriangulationWithApproximation<V, Approximation>&
VectorAlongTriangulationWithApproximation<V, Approximation>::operator+=(
    const HalfEdge halfEdge) {
  impl->coefficients.set(halfEdge, impl->coefficients.get(halfEdge) + 1);
  impl->approximation +=
      static_cast<const Approximation&>(impl->surface.fromEdge(halfEdge));
  return *this;
}

template <typename V, typename Approximation>
VectorAlongTriangulationWithApproximation<V, Approximation>&
VectorAlongTriangulationWithApproximation<V, Approximation>::operator-=(
    const HalfEdge halfEdge) {
  return this->operator+=(-halfEdge);
}

template <typename V, typename Approximation>
bool VectorAlongTriangulationWithApproximation<V, Approximation>::operator>(
    const Bound bound) const {
  auto maybe = impl->approximation > bound;
  if (maybe.has_value()) {
    return *maybe;
  } else {
    assert(!impl->approximation.isExact());
    return impl->exact().operator>(bound);
  }
}

template <typename V, typename Approximation>
CCW VectorAlongTriangulationWithApproximation<V, Approximation>::ccw(
    const VectorAlongTriangulationWithApproximation<V, Approximation>& rhs)
    const {
  const optional<CCW> inexact =
      impl->approximation.ccw(rhs.impl->approximation);
  if (inexact.has_value()) {
    return *inexact;
  } else {
    assert(!impl->approximation.isExact() ||
           !rhs.impl->approximation.isExact());
    return impl->exact().ccw(rhs.impl->exact());
  }
}

template <typename V, typename Approximation>
VectorAlongTriangulationWithApproximation<V, Approximation>::operator V()
    const {
  return impl->exact();
}

template <typename V, typename Approximation>
ostream& operator<<(
    ostream& os,
    const flatsurf::VectorAlongTriangulationWithApproximation<V, Approximation>&
        self) {
  return os << self.impl->coefficients << " ~ " << self.impl->approximation;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
#include "libpolygon/vector_arb.hpp"
#include "libpolygon/vector_eantic.hpp"
#include "libpolygon/vector_exactreal.hpp"
template struct flatsurf::VectorAlongTriangulationWithApproximation<
    flatsurf::VectorEAntic, flatsurf::VectorArb>;
template ostream& flatsurf::operator<<(
    ostream&, const flatsurf::VectorAlongTriangulationWithApproximation<
                  flatsurf::VectorEAntic, flatsurf::VectorArb>&);
template struct flatsurf::VectorAlongTriangulationWithApproximation<
    flatsurf::VectorExactReal<exactreal::NumberFieldTraits>,
    flatsurf::VectorArb>;
template ostream& flatsurf::operator<<(
    ostream&, const flatsurf::VectorAlongTriangulationWithApproximation<
                  flatsurf::VectorExactReal<exactreal::NumberFieldTraits>,
                  flatsurf::VectorArb>&);
