# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RogalunaRouteCore_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RogalunaRouteCore_autogen.dir\\ParseCache.txt"
  "RogalunaRouteCore_autogen"
  )
endif()
