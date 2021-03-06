/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBFLATSURF_TRACKED_IMPL_HPP
#define LIBFLATSURF_TRACKED_IMPL_HPP

#include "../../flatsurf/tracked.hpp"
#include "../external/sigslot/include/sigslot/signal.hpp"
#include "flat_triangulation_combinatorial.impl.hpp"
#include "weak_read_only.hpp"

namespace flatsurf {

template <typename T>
class ImplementationOf<Tracked<T>> {
  using Message = typename ImplementationOf<FlatTriangulationCombinatorial>::Message;

 public:
  using FlipHandler = typename Tracked<T>::FlipHandler;
  using CollapseHandler = typename Tracked<T>::CollapseHandler;
  using SwapHandler = typename Tracked<T>::SwapHandler;
  using EraseHandler = typename Tracked<T>::EraseHandler;
  using DestructionHandler = typename Tracked<T>::DestructionHandler;

  ImplementationOf(ImplementationOf<FlatTriangulationCombinatorial>* parent, T&& value, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction);

  static Tracked<T> make(const ImplementationOf<FlatTriangulationCombinatorial>*, T value, const FlipHandler& updateAfterFlip = Tracked<T>::defaultFlip, const CollapseHandler& updateBeforeCollapse = Tracked<T>::defaultCollapse, const SwapHandler& updateBeforeSwap = Tracked<T>::defaultSwap, const EraseHandler& updateBeforeErase = Tracked<T>::defaultErase, const DestructionHandler& updateBeforeDestruction = Tracked<T>::forgetParent);

  ~ImplementationOf();

  void connect();

  WeakReadOnly<FlatTriangulationCombinatorial> parent;
  T value;

  const FlipHandler updateAfterFlip;
  const CollapseHandler updateBeforeCollapse;
  const SwapHandler updateBeforeSwap;
  const EraseHandler updateBeforeErase;
  const DestructionHandler updateBeforeDestruction;

  typename sigslot::connection onChange;
};

}  // namespace flatsurf

#endif
