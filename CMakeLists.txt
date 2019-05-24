cmake_minimum_required(VERSION 3.2)

set(projectName enjoycco)

project(${projectName})

if (CMAKE_BUILD_TYPE)
else()
	#set(CMAKE_BUILD_TYPE RELEASE)
	set(CMAKE_BUILD_TYPE DEBUG)
endif()


if (UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -Wall -m64 ${CMAKE_CXX_FLAGS}")
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
#set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost COMPONENTS context)
if(Boost_FOUND)
else()
	message("can not find boost, please install boost first")
	EXIT()	
endif()

# add include and lib other project
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS}) 

#link_directories(${PROJECT_SOURCE_DIR}/lib)

# add my project .h and .cpp
include_directories(${PROJECT_SOURCE_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR}/enjoycco SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/enjoycco/coroutine SRC_LIST)

set(TARGET ${projectName})
add_library(${TARGET} ${SRC_LIST})

install(TARGETS ${TARGET} LIBRARY DESTINATION "lib" ARCHIVE DESTINATION "lib")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/enjoycco DESTINATION "include" FILES_MATCHING PATTERN "*.h")

