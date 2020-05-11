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
option(NODESYSTEM_DEBUG "Enables verbose logging for the nodesystem." OFF)
option(STANDALONE "Compiles the project as a standalone executable." ON)

cmake_policy(SET CMP0079 NEW)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NODESYSTEM_DEBUG)
  add_definitions(-DNODESYSTEM_DEBUG)
endif()

#if (STANDALONE)
#  add_definitions(-DJucePlugin_Build_Standalone
#    -DJUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1
#    -DJucePlugin_IsSynth=1
#    -DJucePlugin_ManufacturerCode
#    -DJucePlugin_PluginCode
#    -DJucePlugin_ProducesMidiOutput=0
#    -DJucePlugin_WantsMidiInput=1
#    -DJucePlugin_EditorRequiresKeyboardFocus=1
#  )
#endif()


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
  juce_audio_basics
  juce_audio_devices
  juce_audio_formats
  #juce_audio_plugin_client
  juce_audio_processors
  juce_audio_utils
  juce_core
  juce_cryptography
  juce_data_structures
  juce_dsp
  juce_events
  juce_graphics
  juce_gui_basics
  juce_gui_extra
)
set(JUCE_VER 5.4)
find_package(JUCE "${JUCE_VER}" REQUIRED
  COMPONENTS ${JUCE_COMPONENTS}
)


#set(nodesystem_EXTERNAL ON)
#add_subdirectory(lib/nodesystem)
#add_library(nodesystem STATIC
#  ${nodesystem_INCLUDES} ${nodesystem_SOURCES}
#)
#target_link_libraries(nodesystem PUBLIC ${JUCE_LIBRARIES})



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



####+ Juce Plugin Config ####
#set(common_definitions
#  JUCE_QUICKTIME=0
#  JUCE_PLUGINHOST_VST=0
#  JUCE_PLUGINHOST_AU=0
#  )
#
#juce_add_audio_plugin(
#  PRODUCT_NAME                ${PROJECT_NAME}
#  VERSION                     1.0.0
#  PLUGIN_NAME                 "ModularSynth"
#  PLUGIN_DESC                 "ModularSynth"
#  PLUGIN_MANUFACTURER         "asorgejr"
#  PLUGIN_MANUFACTURER_EMAIL   "aaa@example.com"
#  PLUGIN_MANUFACTURER_CODE    ROLI
#  PLUGIN_CODE                 Jcdm
#  PLUGIN_IS_SYNTH             1
#  PLUGIN_IS_MIDI_EFFECT       0
#  PLUGIN_WANTS_MIDI_IN        1
#  PLUGIN_PRODUCES_MIDI_OUT    1
#  PLUGIN_EDITOR_REQUIRES_KEYS 1
#  ENABLE_IAA                  1
#  PLUGIN_AU_EXPORT_PREFIX     ModularSynthAU
#  PLUGIN_AU_VIEW_CLASS        ModularSynthAU_V1
#  COMPANY_NAME                "asorgejr"
#  COMPANY_WEBSITE             www.asorgejr.com
#  AAX_IDENTIFIER              com.asorgejr.ModularSynth
#  AAX_CATEGORY                AAX_ePlugInCategory_Dynamics
#  VST3_CATEGORY               "Fx"
#  BUNDLE_IDENTIFIER           com.asorgejr.ModularSynth
#  FORMATS                     Standalone AU # AAX VST VST3 # AUv3
#  DEFINITIONS                 ${common_definitions}
#  SOURCES                     ${SOURCES}
#  LIBRARIES                   ${JUCE_LIBRARIES}
#)



####+ Generate Executables ####
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
#add_dependencies(${PROJECT_NAME} nodesystem)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
target_link_libraries(${PROJECT_NAME}
  nameof::nameof
  ${JUCE_LIBRARIES}
  #  nodesystem
)
source_group(Source FILES ${SOURCES})
target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${JUCE_INCLUDE_DIR}"
)
#target_include_directories(${PROJECT_NAME} 
#  PUBLIC
#    "${nodesystem_INCLUDE_DIR}"
#)



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