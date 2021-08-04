cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME LabEngine)
project(${PROJECT_NAME})

add_subdirectory(LabEngineCore)
add_subdirectory(LabEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LabEngineEditor)