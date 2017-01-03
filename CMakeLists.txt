cmake_minimum_required(VERSION 3.2)
project(onthefly)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -lfcgi -lcurl -lmagic" )

set(SOURCE_FILES onthefly.c)

add_executable(onthefly ${SOURCE_FILES})
target_link_libraries(onthefly curl fcgi magic)