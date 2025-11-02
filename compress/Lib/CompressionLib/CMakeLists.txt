# Include project-wide components here

# add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/MyComponent")
set(SOURCE_FILES
  HuffmanCompressor.cpp
  LzssCompressor.cpp
  DctCompressor.cpp
  DctUtils.cpp
)

# You have a header CompressionAlgo.hpp, no need to list it
register_fprime_module()