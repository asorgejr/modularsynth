//
// Created by asorgejr on 4/24/2020.
//
#pragma once
#include <stdio.h>

#define NODEDBG(...) \
do { \
if (NODESYSTEM_DEBUG) { \
  char str[100]; \
  sprintf(str, __VA_ARGS__); \
  std::cout << "[" << __FILE__ << "][" << __FUNCTION__ << "][Line " << __LINE__ << str << std::endl; \
} \
} while (0)

