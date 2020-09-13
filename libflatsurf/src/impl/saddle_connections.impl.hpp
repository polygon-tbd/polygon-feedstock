/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_IMPL_HPP

#include <optional>

#include "../../flatsurf/vector.hpp"
#include "../../flatsurf/bound.hpp"
#include "../../flatsurf/saddle_connections.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<SaddleConnections<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  class ByLength;

  struct Sector {
    Sector(HalfEdge source) : source(source), sector(std::nullopt) {}
    Sector(HalfEdge source, const Vector<T>& sectorBegin, const Vector<T>& sectorEnd) : source(source), sector(std::pair{sectorBegin, sectorEnd}) {}

    HalfEdge source;
    std::optional<std::pair<Vector<T>, Vector<T>>> sector;

    std::vector<Sector> refine(const Surface&, const Vector<T>& sectorBegin, const Vector<T>& sectorEnd) const;
    bool contains(const SaddleConnection<Surface>&) const;
  };

  ImplementationOf(std::shared_ptr<const Surface>);

  std::shared_ptr<const Surface> surface;
  std::vector<Sector> sectors;
  std::optional<Bound> searchRadius;
};

}  // namespace flatsurf

#endif
