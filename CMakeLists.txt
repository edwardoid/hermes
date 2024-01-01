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


cmake_minimum_required(VERSION 3.5)
project(hermes VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

#
# OPTIONS
#

option(BUILD_EXAMPLES "Build examples" ON)
set(BUILD_FOR_LINUX TRUE CACHE BOOL "Are we building for a Linux host?")

add_compile_definitions(HAS_STDINT_H=stdint.h)
add_compile_definitions(HAS_STDTHREAD_H=1)
add_compile_definitions(HAS_STD_MUTEX=1)
add_compile_definitions(HAS_STD_CONDITIONAL_VARIABLE=1)
add_compile_definitions(HM_LOG_WRITE=printf)
add_compile_definitions(LOGGING_HEADER_H=<stdio.h>)
add_compile_definitions(USE_SMART_PTRS=1)

#
# END OF OPTIONS
#

if(${BUILD_FOR_LINUX})
	add_compile_definitions(HAS_LINUX_HEADERS=1)
endif()

file(GLOB SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

if (LIBHERMES_SHARED)
   add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
   add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

set(INTERNAL_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE ${INTERNAL_INCLUDE_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

install(TARGETS ${PROJECT_NAME}
		COMPONENT libhermes	
		DESTINATION lib)

if (BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN YES)

find_package(Doxygen)

if(${DOXYGEN_FOUND})

	find_package(Doxygen
				OPTIONAL_COMPONENTS mscgen dia)

	set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/docs")

	doxygen_add_docs(
		doxygen
		${INTERNAL_INCLUDE_DIR}
		COMMENT "Generate documentstion pages"
)
endif()

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Eduard Sargsyan")
include(CPack)

