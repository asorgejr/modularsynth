include(CMakePackageConfigHelpers)

#! Set this to the name of your package.
set(PKGNAME "libmodularsynth")
set(_header_path_name "modularsynth")
string(TOUPPER "${PKGNAME}" PKGVARNAME)


if (DEFINED libmodularsynth_CUSTOM_BINARY_DIR)
  message("DEBUG ${PKGNAME}: Overriding binary dir: ${CMAKE_CURRENT_BINARY_DIR} to ${libmodularsynth_CUSTOM_BINARY_DIR}")
  set(CMAKE_CURRENT_BINARY_DIR "${libmodularsynth_CUSTOM_BINARY_DIR}")
endif()


if(NOT CLION_PROJECT)
project(${PKGNAME}
  VERSION "0.1.0"
  DESCRIPTION "The backing library for ModularSynth exe."
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" "${CMAKE_MODULE_PATH}")

set(INCLUDE_INSTALL_DIR include/ CACHE STRING "")
set(LIB_INSTALL_DIR lib/ CACHE STRING "")
set(SYSCONFIG_INSTALL_DIR etc/${PKGNAME}/ CACHE STRING "")



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

set(_${PKGVARNAME}_include_files
  IModularComponent.h
  ModularAPI.h
  Module.h
)

set(_${PKGVARNAME}_source_files
  IModularComponent.cpp
  Module.cpp
)

list(TRANSFORM "_${PKGVARNAME}_include_files" PREPEND "${_${PKGVARNAME}_include_prefix}/")
list(TRANSFORM "_${PKGVARNAME}_source_files" PREPEND "${_${PKGVARNAME}_source_prefix}/")

set(SOURCES "${_${PKGVARNAME}_include_files}" "${_${PKGVARNAME}_source_files}")

#message("${PKGNAME} FILES: ${SOURCES}")

if (NOT libmodularsynth_EXTERNAL)
add_library(${PKGNAME} STATIC
  ${SOURCES}
)
endif()

if (NOT CLION_PROJECT)
target_link_libraries(${PKGNAME} PUBLIC ${JUCE_LIBRARIES})
endif()
source_group(Source FILES ${SOURCES})

###############? END PACKAGE CUSTOMIZATIONS ###############


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

set(libmodularsynth_INCLUDE_DIR "${_${PKGVARNAME}_include_dir}")
set(libmodularsynth_SOURCE_DIR "${_${PKGVARNAME}_source_dir}")
set(libmodularsynth_INCLUDES "${_${PKGVARNAME}_include_files}")
set(libmodularsynth_SOURCES "${_${PKGVARNAME}_source_files}")

set(libmodularsynth_INCLUDE_DIR "${libmodularsynth_INCLUDE_DIR}" PARENT_SCOPE)
set(libmodularsynth_SOURCE_DIR "${libmodularsynth_SOURCE_DIR}" PARENT_SCOPE)
set(libmodularsynth_INCLUDES "${libmodularsynth_INCLUDES}" PARENT_SCOPE)
set(libmodularsynth_SOURCES "${libmodularsynth_SOURCES}" PARENT_SCOPE)
