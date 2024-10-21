Cmake_minimum_required(VERSION 2.8)
project(Desherbage)
find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(projet app/main.cpp
                      src/LaserDetection.cpp


                )
target_include_directories(projet PRIVATE
                                include)

target_link_libraries( projet ${OpenCV_LIBS} )

