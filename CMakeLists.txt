cmake_minimum_required(VERSION 2.8) 

# The Project lobby is user management system 
SET(PROJ_NAME "LobbyServer")
SET(PROJ_SOURCES "")
SET(PROJ_HEADERS "")
SET(PROJ_INCLUDE "include" "thirdparty/inc")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project (PROJ_NAME) 
FILE(GLOB_RECURSE PROJ_HEADERS include/*.h)
FILE(GLOB_RECURSE PROJ_SOURCES source/*.cpp)

if (MSVC)
set(Boost_USE_STATIC_LIBS "On")
find_package( Boost 1.52 COMPONENTS system thread regex REQUIRED )
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -pedantic -Wextra")
find_package( Boost 1.52 COMPONENTS system thread REQUIRED )
endif()

include_directories( ${Boost_INCLUDE_DIR} )

# Configure libpq
find_package(PostgreSQL REQUIRED)
include_directories( ${PostgreSQL_INCLUDE_DIRS} )
link_directories( ${PostgreSQL_LIBRARY_DIRS} )

# Configure libpqxx 
find_package(PQXX REQUIRED)
include_directories( ${PQXX_INCLUDE_DIRS} )

# Configure crow
include_directories( ${PROJ_INCLUDE} )

add_executable(${PROJ_NAME} ${PROJ_SOURCES})
target_link_libraries(LobbyServer ${Boost_LIBRARIES})
target_link_libraries(${PROJ_NAME} ${PQXX_LIBRARIES})
target_link_libraries(${PROJ_NAME} ${PostgreSQL_LIBRARIES})
target_link_libraries(LobbyServer ${CMAKE_THREAD_LIBS_INIT})
