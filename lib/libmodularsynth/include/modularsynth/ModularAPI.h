//
// Created by asorgejr on 4/19/2020.
//

#ifndef MODULARSYNTH_MODULARAPI_H
#define MODULARSYNTH_MODULARAPI_H

#ifdef _WIN32
#define DISABLE_LOD_WARNINGS \
__pragma(warning(disable : 4305)) \
__pragma(warning(disable : 4244))

#else
#define DISABLE_LOD_WARNINGS
#endif

#endif //MODULARSYNTH_MODULARAPI_H
