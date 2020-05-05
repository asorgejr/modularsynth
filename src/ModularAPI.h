//
// Created by asorgejr on 4/19/2020.
//

#pragma once
#include <nodesystem/NodeSystemAPI.h>

#pragma region Disable Warning Macros

#if defined(_MSC_VER)
#define DISABLE_WARNING_PUSH           __pragma(warning( push ))
#define DISABLE_WARNING_POP            __pragma(warning( pop ))
#define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(4100)
#define DISABLE_WARNING_UNREFERENCED_FUNCTION            DISABLE_WARNING(4505)
#define DISABLE_WARNING_LOSS_OF_DATA                     DISABLE_WARNING(4244) DISABLE_WARNING(4267)
// other warnings you want to deactivate...

#elif defined(__GNUC__) || defined(__clang__)
#define DO_PRAGMA(X) _Pragma(#X)
  #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
  #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop) 
  #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)
  
  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(-Wunused-parameter)
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION            DISABLE_WARNING(-Wunused-function)
  #define DISABLE_WARNING_LOSS_OF_DATA
 // other warnings you want to deactivate... 
    
#else
  #define DISABLE_WARNING_PUSH
  #define DISABLE_WARNING_POP
  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION
  #define DISABLE_WARNING_LOSS_OF_DATA
  // other warnings you want to deactivate...
#endif

#pragma endregion Disable Warning Macros

