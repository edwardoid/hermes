# Hermes - A RPC for IOT
# Copyright (C) 2023  Eduard Sargsyan and Andrey Ovodov

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

function(add_example exmpl enable_option sources)
    option($enable_option "Build {exmpl}" ON)
    if (${enable_option})
        add_executable(${PROJECT_NAME}_${exmpl} ${sources})
        include_directories(${PROJECT_NAME}_${exmpl} ${INTERNAL_INCLUDE_DIR})
        target_link_libraries (${PROJECT_NAME}_${exmpl} ${PROJECT_NAME})
    endif()
endfunction()

set(BUILD_EXAMPLES_TCP_MASTER ON)
add_example(tcp_master BUILD_EXAMPLES_TCP_MASTER ${CMAKE_CURRENT_LIST_DIR}/tcp/master.cpp)

set(BUILD_EXAMPLES_TCP_SLAVE ON)
add_example(tcp_slave  BUILD_EXAMPLES_TCP_SLAVE ${CMAKE_CURRENT_LIST_DIR}/tcp/slave.cpp)