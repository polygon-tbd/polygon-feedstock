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

#include "libpolygon/flat_triangulation.hpp"
#include "libpolygon/half_edge.hpp"
#include "libpolygon/saddle_connections.hpp"

namespace flatsurf {
template <typename Vector, typename VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::SaddleConnections(
    const Surface& surface, const Bound searchRadius, const Vertex& source,
    const HalfEdge sectorBoundary, const VectorAlongTriangulation& sectorBegin,
    const VectorAlongTriangulation& sectorEnd)
    : surface(surface),
      searchRadius(searchRadius),
      source(source),
      sectorBoundary(sectorBoundary),
      sector{sectorBegin, sectorEnd} {
  // TODO: Check that sectorBoundary points from sectorBegin to sectorEnd.
}

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::SaddleConnections(
    const Surface& surface, const Bound searchRadius,
    const HalfEdge sectorBegin)
    : SaddleConnections(
          surface, searchRadius, Vertex::source(sectorBegin, surface),
          surface.nextInFace(sectorBegin),
          VectorAlongTriangulation(surface) + sectorBegin,
          VectorAlongTriangulation(surface) -
              surface.nextInFace(surface.nextInFace(sectorBegin))) {}

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnectionsIterator<Vector, VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::begin() const {
  return SaddleConnectionsIterator<Vector, VectorAlongTriangulation>(*this);
}

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnectionsIterator<Vector, VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::end() const {
  return SaddleConnectionsIterator<Vector, VectorAlongTriangulation>();
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>

#include "libpolygon/vector_eantic.hpp"
#include "libpolygon/vector_exactreal.hpp"
#include "libpolygon/vector_longlong.hpp"
using namespace flatsurf;

template struct flatsurf::SaddleConnections<VectorLongLong>;
template struct flatsurf::SaddleConnections<VectorEAntic>;
template struct flatsurf::SaddleConnections<
    VectorEAntic, VectorAlongTriangulation<VectorEAntic>>;
template struct flatsurf::SaddleConnections<
    VectorExactReal<exactreal::NumberFieldTraits>>;
