cmake_minimum_required(VERSION 3.10) # For CXX_STANDARD 17 property on Visual Studio

# Project declaration
project(SCPSolver)

# By default build in Release mode
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Release, RelWithDebInfo or Debug" FORCE)
endif()

# CMake utilities
get_filename_component(CMUTILS_PATH "cmake/CMutils/cmutils.cmake" ABSOLUTE)
if(EXISTS "${CMUTILS_PATH}")
	include(${CMUTILS_PATH})
else()
	message(FATAL_ERROR "CMutils is missing, maybe you didn't pull the git submodules")
endif()

# Global config
cmutils_disable_in_sources_build()
cmutils_enable_ide_folders()
cmutils_set_classic_output_folders()

# Libraries
include(cmake/dynamic_bitset.cmake)
include(cmake/spdlog.cmake)

# Declare SCPSolver
add_executable(SCPSolver)

# Add sources
cmutils_target_sources_folders(
  SCPSolver PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Add includes
target_include_directories(
  SCPSolver PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link dependencies
target_link_libraries(
  SCPSolver PRIVATE
  dynamic_bitset
  spdlog
)

# Link threads library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(SCPSolver PRIVATE Threads::Threads)

# Link filesystem library for clang and gcc < 9.0
cmutils_define_compiler_variables()
if(COMPILER_CLANG OR (COMPILER_GCC AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)))
	target_link_libraries(SCPSolver PRIVATE stdc++fs)
endif()

# Configure compile options
cmutils_target_configure_compile_options(SCPSolver)

# Enable cmutils default warnings
cmutils_target_enable_warnings(SCPSolver)

# Build in C++17
cmutils_target_set_standard(SCPSolver CXX 17)

# Set static runtime
cmutils_target_set_runtime(SCPSolver STATIC)

# Enable sanitizers in debug
cmutils_target_enable_sanitizers(
  SCPSolver
  ADDRESS LEAK UNDEFINED MEMORY
  DEBUG
)

# Set target IDE folder
cmutils_target_set_ide_folder(SCPSolver "SCPSolver")

# Group sources for IDEs
cmutils_target_source_group(SCPSolver "${CMAKE_CURRENT_SOURCE_DIR}")

# Visual studio config
get_target_property(scpsolver_binary_folder SCPSolver RUNTIME_OUTPUT_DIRECTORY)
set_target_properties(
  SCPSolver PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${scpsolver_binary_folder}"
)
set_property(
  DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
  VS_STARTUP_PROJECT SCPSolver
)

# Resources
cmutils_configure_folder(
  "${CMAKE_CURRENT_SOURCE_DIR}/resources"
  "${scpsolver_binary_folder}/resources"
  COPYONLY
)

# Use ccache
cmutils_target_use_ccache(SCPSolver)

# Generate format target
cmutils_target_generate_clang_format(SCPSolver)

# special optimisation flags
cmutils_define_compiler_variables()
if(COMPILER_GCC OR COMPILER_CLANG)
	set(flags
	  # https://gcc.gnu.org/onlinedocs/gcc-4.5.3/gcc/i386-and-x86_002d64-Options.html
	  "-march=native"
	  "-mtune=native"
	  # https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
	  "-fmodulo-sched"
	  "-fmodulo-sched-allow-regmoves"
	  "-fgcse-sm"
	  "-fgcse-las"
	  "-fgcse-after-reload"
	  "-fdevirtualize-speculatively"
	  "-fdevirtualize-at-ltrans"
	  "-floop-nest-optimize"
	  "-floop-parallelize-all"
	  "-fsplit-loops"
	  "-funswitch-loops"
	  "-floop-unroll-and-jam"
	)

	foreach(flag ${flags})
		cmutils_target_add_cxx_compiler_flag(SCPSolver ${flag} RELEASE RELWITHDEBINFO)
		cmutils_target_add_cxx_linker_flag(SCPSolver ${flag} RELEASE RELWITHDEBINFO)
	endforeach()
endif()

# Verbose makefile
#set(CMAKE_VERBOSE_MAKEFILE ON)
