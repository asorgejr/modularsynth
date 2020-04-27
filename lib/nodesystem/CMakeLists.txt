#+ Because of issues with sharing nested variable names, you must refactor the
#+ variables at the bottom of this script.

#+ If using JetBrains IDE, a convenient way to refactor is to press Ctrl + r and 
#+ select regex mode. Add this regex statement in the find input field: 
#+ package_name(_\w+) 
#+ your_package_name$1 <--(your_package_name is the unique name of your package.)
#+ Set this to the name of your package.

include(CMakePackageConfigHelpers)
set(PKGNAME "nodesystem")
set(_header_path_name ${PKGNAME})
string(TOUPPER "${PKGNAME}" PKGVARNAME)


if (DEFINED nodesystem_CUSTOM_BINARY_DIR)
  message("DEBUG ${PKGNAME}: Overriding binary dir: ${CMAKE_CURRENT_BINARY_DIR} to ${nodesystem_CUSTOM_BINARY_DIR}")
  set(CMAKE_CURRENT_BINARY_DIR "${nodesystem_CUSTOM_BINARY_DIR}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()


if(NOT CLION_PROJECT)
project(${PKGNAME}
  VERSION "0.1.0"
  DESCRIPTION "A node graph library for use with juce."
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" "${CMAKE_MODULE_PATH}")

set(INCLUDE_INSTALL_DIR include/)
set(LIB_INSTALL_DIR lib/)
set(SYSCONFIG_INSTALL_DIR etc/${PKGNAME}/)



set(_${PKGVARNAME}_include_dir "${CMAKE_CURRENT_LIST_DIR}/include")
set(_${PKGVARNAME}_source_dir "${CMAKE_CURRENT_LIST_DIR}/src")
set(_${PKGVARNAME}_include_prefix "${_${PKGVARNAME}_include_dir}/${_header_path_name}")
set(_${PKGVARNAME}_source_prefix "${_${PKGVARNAME}_source_dir}/${_header_path_name}")


##############? BEGIN DEPENDENCY CUSTOMIZATIONS ###############

if(NOT JUCE_LIBRARIES)
  set(JUCE_COMPONENTS
    juce_audio_basics juce_audio_devices juce_audio_formats juce_audio_processors
    juce_audio_utils juce_core juce_cryptography juce_data_structures juce_events
    juce_graphics juce_gui_basics juce_gui_extra juce_opengl
  )
  set(JUCE_VER 5.4)
  find_package(JUCE "${JUCE_VER}" REQUIRED
    COMPONENTS ${JUCE_COMPONENTS}
  )
endif()

##############? END DEPENDENCY CUSTOMIZATIONS ###############

##############? BEGIN PACKAGE CUSTOMIZATIONS ###############

file(GLOB_RECURSE "_${PKGVARNAME}_include_files" 
  "${_${PKGVARNAME}_include_prefix}/*.h"
  "${_${PKGVARNAME}_include_prefix}/*.hpp"
)
file(GLOB_RECURSE _${PKGVARNAME}_source_files "${_${PKGVARNAME}_source_prefix}/*.cpp")

set(SOURCES 
  "${_${PKGVARNAME}_include_files}"
  "${_${PKGVARNAME}_private_include_files}"
  "${_${PKGVARNAME}_source_files}")

#message("${PKGNAME} FILES: ${SOURCES}")

if (NOT nodesystem_EXTERNAL)
add_library(${PKGNAME} STATIC
  ${SOURCES}
)
endif()

if (NOT CLION_PROJECT)
target_link_libraries(${PKGNAME} PUBLIC ${JUCE_LIBRARIES})
endif()
source_group(Source FILES ${SOURCES})

##############? END PACKAGE CUSTOMIZATIONS ###############


configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/${PKGNAME}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PKGNAME}Config.cmake"
  INSTALL_DESTINATION "${LIB_INSTALL_DIR}/${PKGNAME}/cmake"
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PKGNAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(
  FILES 
    "${CMAKE_CURRENT_BINARY_DIR}/${PKGNAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PKGNAME}ConfigVersion.cmake"
  DESTINATION ${LIB_INSTALL_DIR}/${PKGNAME}/cmake
)

# Nested Dereferenced variables cannot be used as values in set() with PARENT_SCOPE.
set(nodesystem_INCLUDE_DIR "${_${PKGVARNAME}_include_dir}")
set(nodesystem_SOURCE_DIR "${_${PKGVARNAME}_source_dir}")
set(nodesystem_INCLUDES "${_${PKGVARNAME}_include_files}")
set(nodesystem_SOURCES "${_${PKGVARNAME}_source_files}")

set(nodesystem_INCLUDE_DIR ${nodesystem_INCLUDE_DIR} PARENT_SCOPE)
set(nodesystem_SOURCE_DIR ${nodesystem_SOURCE_DIR} PARENT_SCOPE)
set(nodesystem_INCLUDES ${nodesystem_INCLUDES} PARENT_SCOPE)
set(nodesystem_SOURCES ${nodesystem_SOURCES} PARENT_SCOPE)