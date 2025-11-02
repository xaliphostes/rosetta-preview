# Findrosetta.cmake
# Provides:
#   rosetta_FOUND
#   ROSETTA_INCLUDE_DIR
#   rosetta::rosetta  (INTERFACE target)

include(FindPackageHandleStandardArgs)

# Pick a header that definitely exists in include/rosetta/
set(_ROSETTA_HEADER "rosetta.h")

find_path(ROSETTA_INCLUDE_DIR
  NAMES rosetta/${_ROSETTA_HEADER}
  HINTS
    "${CMAKE_CURRENT_LIST_DIR}/../include"     # root/cmake -> root/include
    "${CMAKE_SOURCE_DIR}/../include"           # if invoked from a subdir
    "$ENV{ROSETTA_ROOT}/include"               # optional env hint
  PATH_SUFFIXES include
)

find_package_handle_standard_args(rosetta
  REQUIRED_VARS ROSETTA_INCLUDE_DIR
)

if(rosetta_FOUND AND NOT TARGET rosetta::rosetta)
  add_library(rosetta::rosetta INTERFACE IMPORTED)
  set_target_properties(rosetta::rosetta PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ROSETTA_INCLUDE_DIR}"
  )
endif()
