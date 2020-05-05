cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(ModularSynth
  VERSION "0.1.0"
  DESCRIPTION "A badass Synthesizer...someday"
  LANGUAGES CXX
)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20) # MSVC rejects designated initializers below c++20. 
message(STATUS "System: ${CMAKE_SYSTEM}")



####+ Optional CMake Commands ####
option(TESTS_ENABLED "Builds the tests associated with this project." OFF)
option(CLION_PROJECT "Enables libraries to build in a way which CLion is compatible with." OFF)

cmake_policy(SET CMP0079 NEW)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions(-DNODESYSTEM_DEBUG)


####+ Library Import ####
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" "${CMAKE_MODULE_PATH}")
find_package(nameof CONFIG REQUIRED)



####+ WSL Fixes ####
include("linux_tools")
# TODO: See if WSL interferes with actual Linux distro.
if(LINUX)
  include("wsl_tools")
  wsl_graphics()
endif()



####+ Project Library ####
set(JUCE_COMPONENTS
  juce_audio_basics juce_audio_devices juce_audio_formats juce_audio_processors
  juce_audio_utils juce_core juce_cryptography juce_data_structures juce_dsp
  juce_events juce_graphics juce_gui_basics juce_gui_extra juce_opengl
)
set(JUCE_VER 5.4)
find_package(JUCE "${JUCE_VER}" REQUIRED
  COMPONENTS ${JUCE_COMPONENTS}
)


set(nodesystem_EXTERNAL ON)
add_subdirectory(lib/nodesystem)
add_library(nodesystem STATIC
  ${nodesystem_INCLUDES} ${nodesystem_SOURCES}
)
target_link_libraries(nodesystem PUBLIC ${JUCE_LIBRARIES})



if(IOS)
  # TODO: Implement platform-specific targeting.
endif()

if(ANDROID)
  # TODO: Implement platform-specific targeting.
endif()



####+ Project Sources ####
file(GLOB_RECURSE sSOURCES
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE sHEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
set(SOURCES "${sSOURCES}" "${sHEADERS}")



####+ Generate Executables ####
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
add_dependencies(${PROJECT_NAME} nodesystem)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
target_link_libraries(${PROJECT_NAME}
  nodesystem
  nameof::nameof
)
source_group(Source FILES ${SOURCES})
target_include_directories(${PROJECT_NAME} 
  PUBLIC
    "${nodesystem_INCLUDE_DIR}"
)



####+ Install Resources ####
include(resource_tools)
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
set(OUTPUT_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/Resources")
file(GLOB RESOURCE_IMAGES "${RESOURCES_DIR}/images/*.*")
file(GLOB RESOURCE_AUDIO "${RESOURCES_DIR}/audio/*.*")
file(GLOB RESOURCE_ICONS "${RESOURCES_DIR}/icons/*.*")
add_resources(OUT_VAR "${OUTPUT_RESOURCES_DIR}/images" "${RESOURCE_IMAGES}")
add_resources(OUT_VAR "${OUTPUT_RESOURCES_DIR}/audio" "${RESOURCE_AUDIO}")
add_resources(OUT_VAR "${OUTPUT_RESOURCES_DIR}/icons" "${RESOURCE_ICONS}")
target_sources(${PROJECT_NAME} PUBLIC
  "${RESOURCE_IMAGES}"
  "${RESOURCE_AUDIO}"
  #"${RESOURCE_ICONS}"
)



####+ OPTIONAL TESTS ####
if(TESTS_ENABLED)
  find_package(Boost 1.72 REQUIRED
    COMPONENTS filesystem system unit_test_framework
  )
  if(NOT DEFINED JUCE_INCLUDE_DIR OR NOT DEFINED Boost_INCLUDE_DIR)
    message("boost include dir = ${Boost_INCLUDE_DIR}; juce include dir = ${JUCE_INCLUDE_DIR}")
    message(FATAL_ERROR "This test build requires Boost 1.72 and JUCE 5.4 for a successful build.")
  endif()
  
  message("DEV: Tests will be enabled for this build.")
  set(TESTS_DIR "${CMAKE_SOURCE_DIR}/tests")
  add_executable(test_sizes "${TESTS_DIR}/sizes.cpp")
  add_executable(test_juce_defaults "${TESTS_DIR}/JuceDefaults.cpp")

  target_link_libraries(test_juce_defaults 
    "${JUCE_LIBRARIES}" 
    # Boost::filesystem Boost::system Boost::unit_test_framework
  )
  target_include_directories(test_juce_defaults PUBLIC 
    "${JUCE_INCLUDE_DIR}" 
    # "${Boost_INCLUDE_DIR}"
  )
endif()