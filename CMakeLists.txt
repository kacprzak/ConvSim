project( ConvSim )
cmake_minimum_required( VERSION 2.8 )

add_definitions( "-Wall -Wextra" )

set( convsim_SRCS
    src/main.cpp
)

set( convsim_HEADERS 
)

include_directories(
  src
)

add_executable( convsim
  ${convsim_SRCS}
)
