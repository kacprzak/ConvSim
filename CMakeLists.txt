project( ConvSim )
cmake_minimum_required( VERSION 2.8 )

if( MSVC )
  #add_definitions( "" )
else()
  add_definitions( "-Wall -Wextra -g" )
endif()

set( ConvSim_SRCS
  src/conveyor.cpp
  src/tank.cpp
  src/loadinggrid.cpp
  src/weighingbelt.cpp
)

set( engine_HEADERS
  engine/model.h
  engine/atomic.h
  engine/simulator.h
  engine/eventlistener.h
  engine/network.h
  engine/event.h
)

set( ConvSim_HEADERS
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
  src engine
)

add_executable( convsim
  src/main.cpp ${ConvSim_SRCS} ${ConvSim_HEADERS} ${engine_HEADERS}
)

# TESTY

set( ConvSim_TESTS
  test/simulator_test.cpp
)

add_executable( simulator_test ${ConvSim_TESTS} ${ConvSim_SRCS} )

enable_testing()
add_test( simulator_test ${CMAKE_CURRENT_BINARY_DIR}/simulator_test )
