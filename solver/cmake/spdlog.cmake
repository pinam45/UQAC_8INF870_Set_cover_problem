message(STATUS "Configuring spdlog")

get_filename_component(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/spdlog ABSOLUTE)

# Submodule check
cmutils_directory_is_empty(is_empty "${SPDLOG_DIR}")
if(is_empty)
	message(FATAL_ERROR "Spdlog dependency is missing, maybe you didn't pull the git submodules")
endif()

# Include spdlog
add_subdirectory(${SPDLOG_DIR})
if(NOT TARGET spdlog)
	message(FATAL_ERROR "spdlog target is missing")
endif ()

# Disable warnings on spdlog headers
get_target_property(spdlog_include_directories spdlog INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(spdlog PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")
target_include_directories(spdlog SYSTEM INTERFACE ${spdlog_include_directories})

message(STATUS "Configuring spdlog - Done")
