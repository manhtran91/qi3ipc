# Thanks to Félix C. Morency for his awesome template
# https://github.com/fmorency/cmake-template_simple-library

# The minimum CMake version required to build this project
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

# Set a new CMake project
project(qi3ipc)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.1.0")

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Network X11Extras)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# This was apparently needed for finding KF5
find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS WindowSystem)

include(FindXCB)

include(CheckIncludeFiles)
set(I3IPC_H "i3/ipc.h")
check_include_files (${I3IPC_H} HAVE_I3IPC)
if(NOT HAVE_I3IPC)
  message(FATAL_ERROR "${I3IPC_H} is not found" )
endif()

# If the build script is called from a parent project,
# use the configuration from there.
if(NOT COMMAND if_defined_set)
    set(cmake_directory ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endif()

# Set the CMake module path to the project cmake directory. This location will
# first be searched for external CMake packages.
set(CMAKE_MODULE_PATH ${cmake_directory})

# Include the library build configuration. This will also include the custom
# macro defined for the project.
include(build_config)

# Set the location of the library configuration file if it has not already been
# set. This allows the library to be used by an external project without
# overwritting the variable.
if_defined_set(${PROJECT_NAME}_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Load the project configuration file. CMake will search in the directory setted
# above for a module file named qi3ipc-config.cmake. The configuration
# file will set the different directories and libraries required by the library
find_package(${PROJECT_NAME} REQUIRED)

# Add the project include directory
include_directories(${${PROJECT_NAME}_include_dir} )

# Set the source files required to build the library
include(fileList.cmake)

# Build the library
add_library(${PROJECT_NAME} STATIC ${srcs} ${hdrs})

target_link_libraries(${PROJECT_NAME}
    KF5::WindowSystem
    Qt5::Network
    Qt5::X11Extras
    ${XCB_XCB_LIBRARIES}
)
