project( ConvSim )
cmake_minimum_required( VERSION 2.8 )

if( MSVC )
  #add_definitions( "" )
else()
  add_definitions( "-Wall -Wextra -g -std=c++0x" )
endif()

set( ConvSim_SRCS
  src/conveyor.cpp
  src/tank.cpp
  src/loadinggrid.cpp
  src/weighingbelt.cpp
  src/transportationsystem.cpp
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
  src/transportationsystem.h
  src/utils.h
  src/connection.h
)

configure_file(data/connections.txt ${CMAKE_CURRENT_BINARY_DIR}/connections.txt COPYONLY)
configure_file(data/kr.txt ${CMAKE_CURRENT_BINARY_DIR}/kr.txt COPYONLY)
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

add_executable( simulator_test test/simulator_test.cpp ${ConvSim_SRCS} )
add_executable( transportationsystem_test test/transportationsystem_test.cpp ${ConvSim_SRCS} )

enable_testing()
add_test( simulator_test ${CMAKE_CURRENT_BINARY_DIR}/simulator_test )
add_test( transportationsystem_test ${CMAKE_CURRENT_BINARY_DIR}/transportationsystem_test )
