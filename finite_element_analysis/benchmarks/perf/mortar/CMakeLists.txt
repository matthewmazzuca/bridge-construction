###  CMakeLists.txt ---

#  Author(s): Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
#       Date: 2010-06-12
#
#  Copyright (C) 2010 Universite Joseph Fourier (Grenoble I)
#
# Distributed under the GPL(GNU Public License):
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(gen_feel_perf_mortar ALL )
#foreach(N 2 3 4 5)
#foreach(S "Simplex" "Hypercube")
  foreach(D 2 )
    foreach(N1 2 )
      foreach(N2 2 )
        set( LAG "// this file is automatically generated
#include <mortar.hpp>
namespace Feel {
  template class MortarBench<${D}, ${N1}, ${N2}>\\;
}
")
    OVERWITE_IF_DIFFERENT(gen_feel_perf_mortar "feel_perf_mortar_${D}D_${N1}_${N2}.cpp" "${LAG}" mortar.hpp)
    set(MORTAR "${CMAKE_CURRENT_BINARY_DIR}/feel_perf_mortar_${D}D_${N1}_${N2}.cpp;${MORTAR}")
  endforeach()
endforeach()
endforeach()

add_executable(feel_perf_mortar bench.cpp  ${MORTAR}  )
add_dependencies(feel_perf_mortar gen_feel_perf_mortar)
target_link_libraries(feel_perf_mortar feelpp ${FEELPP_LIBRARIES})


configure_file(mortar.cfg mortar.cfg)

