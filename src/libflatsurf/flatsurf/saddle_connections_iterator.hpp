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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_HPP

#include <optional>
#include <boost/iterator/iterator_facade.hpp>
#include "external/spimpl/spimpl.h"

#include "flatsurf/ccw.hpp"
#include "flatsurf/flatsurf.hpp"
#include "flatsurf/forward.hpp"
#include "flatsurf/saddle_connection.hpp"

namespace flatsurf {
template <typename Vector, typename VectorAlongTriangulation>
struct SaddleConnectionsIterator
    : boost::iterator_facade<
          SaddleConnectionsIterator<Vector, VectorAlongTriangulation>,
          const std::unique_ptr<SaddleConnection<Vector, VectorAlongTriangulation>>,
          std::forward_iterator_tag,
          const std::unique_ptr<SaddleConnection<Vector, VectorAlongTriangulation>>> {
  friend class boost::iterator_core_access;

  // Advance the iterator to the next saddle connection.
  void increment();
  // Advance the iterator to the next saddle connection or until a HalfEdge is
  // being crossed during the search (in forward direction.) This can be useful
  // if information about the exact path in the surface for a saddle connection
  // needs to be reconstructed.
  std::optional<HalfEdge> incrementWithCrossings();
  bool equal(const SaddleConnectionsIterator<Vector, VectorAlongTriangulation> &other) const;
  std::unique_ptr<SaddleConnection<Vector, VectorAlongTriangulation>>
  dereference() const;

  void skipSector(CCW sector);

private:
  friend struct SaddleConnections<Vector, VectorAlongTriangulation>;
  SaddleConnectionsIterator();

  struct Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
} // namespace flatsurf

#endif
