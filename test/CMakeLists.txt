cmake_minimum_required(VERSION 3.2)

if (CMAKE_BUILD_TYPE)
else()
	set(CMAKE_BUILD_TYPE RELEASE)
	#set(CMAKE_BUILD_TYPE DEBUG)
endif()


if (UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++14 -fPIC -Wall -m64 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 ${CMAKE_CXX_FLAGS}")
elseif (WIN32)
    # windows platform
    #add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /EHsc")
endif()

message("------------ Options -------------")
message("  CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("  CMAKE_COMMAND: ${CMAKE_COMMAND}")
message("  CMAME_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("-------------- Env ---------------")
message("  CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("  CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("  CMAKE_PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("----------------------------------")

# find boost
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost COMPONENTS context)
if(Boost_FOUND)
else()
	message("can not find boost, please install boost first")
	EXIT()	
endif()
# add include and lib other project
#include_directories(${PROJECT_SOURCE_DIR}/../include)
#link_directories(${PROJECT_SOURCE_DIR}/../lib)

aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)

set(LINK_ARGS ${Boost_LIBRARIES} enjoycco)

foreach(var ${SRC_LIST})
	string(REGEX REPLACE ".*/" "" var ${var})
	string(REGEX REPLACE ".cpp" "" tgt ${var})

	set(target ${tgt}.t)
	add_executable(${target} ${var})
	target_link_libraries(${target} ${LINK_ARGS})
endforeach(var)
