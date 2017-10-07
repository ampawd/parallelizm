cmake_minimum_required (VERSION 3.1.0)


# enable c++11 support
set (CMAKE_CXX_STANDARD 11)


# project name
project (parallelizm)


# includes
include_directories (${CMAKE_SOURCE_DIR}/suite)

include_directories (${CMAKE_SOURCE_DIR}/profiler)

# set(HEADER_FILES ${CMAKE_SOURCE_DIR}/include/)


# sources
file(GLOB SOURCES suite/*.cpp)

# support for threading
find_package(Threads)

# executable
add_executable (parallelizm 
	${SOURCES} main.cpp 
	${HEADER_FILES}
)

target_link_libraries(parallelizm ${CMAKE_THREAD_LIBS_INIT})

# or
# TARGET_LINK_LIBRARIES( parallelizm pthread )
