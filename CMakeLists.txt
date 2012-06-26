project( ConvSim )
cmake_minimum_required( VERSION 2.8 )

if( MSVC )
  #add_definitions( "" )
else()
  add_definitions( "-Wall -Wextra" )
endif()

set( ConvSim_SRCS
  src/atomic.cpp
  src/simulator.cpp
  src/eventlistener.cpp
  src/network.cpp
  src/resultant.cpp

  src/main.cpp
  src/conveyor.cpp
  src/tank.cpp
  src/loadinggrid.cpp
  src/weighingbelt.cpp
)

set( ConvSim_HEADERS
  src/atomic.h
  src/simulator.h
  src/eventlistener.h
  src/network.h
  src/resultant.h

  src/conveyor.h
  src/tank.h
  src/loadinggrid.h
  src/weighingbelt.h
)

configure_file(data/kr.txt ${CMAKE_CURRENT_BINARY_DIR}/kr.txt COPYONLY)
configure_file(data/kr1.txt ${CMAKE_CURRENT_BINARY_DIR}/kr1.txt COPYONLY)
configure_file(data/kr2.txt ${CMAKE_CURRENT_BINARY_DIR}/kr2.txt COPYONLY)
configure_file(data/kr3.txt ${CMAKE_CURRENT_BINARY_DIR}/kr3.txt COPYONLY)
configure_file(data/przenosniki.txt ${CMAKE_CURRENT_BINARY_DIR}/przenosniki.txt COPYONLY)
configure_file(data/wagi.txt ${CMAKE_CURRENT_BINARY_DIR}/wagi.txt COPYONLY)
configure_file(data/zbiorniki.txt ${CMAKE_CURRENT_BINARY_DIR}/zbiorniki.txt COPYONLY)


include_directories(
  src
)

add_executable( convsim
  ${ConvSim_SRCS}
)
