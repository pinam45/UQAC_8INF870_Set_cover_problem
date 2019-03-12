message(STATUS "Configuring spdlog")

get_filename_component(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/spdlog ABSOLUTE)

# Submodule check
cmutils_directory_is_empty(is_empty "${SPDLOG_DIR}")
if(is_empty)
	message(FATAL_ERROR "Spdlog dependency is missing, maybe you didn't pull the git submodules")
endif()

add_subdirectory(${SPDLOG_DIR})

message(STATUS "Configuring spdlog - Done")
