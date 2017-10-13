cmake_minimum_required(VERSION 2.6)


# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(FlappyCube)

include_directories("${PROJECT_BINARY_DIR}")
set(PROJECT_SOURCE_DIR "src")

set(EXECUTABLE_NAME "flappy_cube")

file(GLOB ENGINE_SOURCES
    ${PROJECT_SOURCE_DIR}/engine/*.h
    ${PROJECT_SOURCE_DIR}/engine/*.cpp
)

file(GLOB ANIMATION_SOURCES
    ${PROJECT_SOURCE_DIR}/animation/*.h
    ${PROJECT_SOURCE_DIR}/animation/*.cpp
)

file(GLOB RANK_TABLE_SCOUNRES
    ${PROJECT_SOURCE_DIR}/rank_table/*.h
    ${PROJECT_SOURCE_DIR}/rank_table/*.cpp
)

file(GLOB GUI_SOURCES
    ${PROJECT_SOURCE_DIR}/gui/*.h
    ${PROJECT_SOURCE_DIR}/gui/*.cpp
)

file(GLOB SCENES_SOURCES
    ${PROJECT_SOURCE_DIR}/scenes/*.h
    ${PROJECT_SOURCE_DIR}/scenes/*.cpp
)

add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCE_DIR}/main.cpp)
add_library(engine_lib ${ENGINE_SOURCES})
add_library(animation_lib ${ANIMATION_SOURCES})
add_library(rank_table_lib ${RANK_TABLE_SCOUNRES})
add_library(gui_lib ${GUI_SOURCES})
add_library(scenes_lib ${SCENES_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} engine_lib animation_lib rank_table_lib gui_lib scenes_lib)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML 
#See the FindSFML.cmake file for additional details and instructions
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 REQUIRED graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)