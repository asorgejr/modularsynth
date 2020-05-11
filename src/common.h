//
// Created by asorgejr on 4/23/2020.
//

#pragma once
#include <cmath>
#include <type_traits>
#include <JuceHeader.h>
#include <nameof.hpp>
#include "ModularAPI.h"
#include "app/config.h"
#include "Utilities.h"

#define NAMEOF_FIELD(...) ((String)((string) NAMEOF(__VA_ARGS__)))

const double epsilon = 1e-5;

template <RefType T>
void printVector(const std::vector<T> &vec) {
  try {
    int i = 0;
    std::cout << "[" << std::endl;
    for (auto &elem : vec) {
      std::string sep = "";
      if (++i != vec.size()) sep = ", ";
      std::cout << elem << sep << std::endl;
    }
    std::cout << "]" << std::endl;
  } catch (...) {
    std::cout << "printVector error: could not stream contents of specified type: '"
    << typeid(T).name() << "'." << std::endl;
  }
}

template<RefType T, RefType U>
void printVector(const std::vector<T> &vec, std::function<U(T)> unpacker) {
  try {
    int i = 0;
    std::cout << "[" << std::endl;
    for (auto &elem : vec) {
      std::string sep = "";
      if (++i != vec.size()) sep = ", ";
      U out = unpacker(elem);
      std::cout << out << sep << endl;
    }
    std::cout << "]" << std::endl;
  } catch(...) {
    std::cout << "printVector error: could not stream contents of specified type: '"
    << typeid(U).name() << "'." << std::endl;
  }
}

/*
 * Generic implementation to search if a given value exists in a map or not.
 * Adds all the keys with given value in the vector
 */
template<typename K, typename V>
bool findByValue(std::vector<K> & vec, std::map<K, V> mapOfElemen, V value)
{
  bool result = false;
  auto it = mapOfElemen.begin();
  // Iterate through the map
  while(it != mapOfElemen.end())
  {
    // Check if value of this entry matches with given value
    if(it->second == value)
    {
      // Yes found
      result = true;
      // Push the key in given map
      vec.push_back(it->first);
    }
    // Go to next entry in map
    it++;
  }
  return result;
}

template <typename T>
bool isZero(T value) {
  return std::abs(value) <= epsilon;
}

template <typename T, typename U>
Rectangle<T> divideRect(const Rectangle<T> &dividend, const Rectangle<U> &divisor) {
  Rectangle<T> result(0, 0, 0, 0);
  if (!(isZero(dividend.getX())))
    result.setX(dividend.getX() / (float)divisor.getX());
  if (!(isZero(dividend.getY())))
    result.setY(dividend.getY() / (float)divisor.getY());
  if (!(isZero(divisor.getWidth())))
    result.setWidth(dividend.getWidth() / (float)divisor.getWidth());
  if (!(isZero(divisor.getHeight())))
    result.setHeight(dividend.getHeight() / (float)divisor.getHeight());
  return result;
}

template<typename T, typename U>
Rectangle<T> multiplyRect(const Rectangle<T> &first, const Rectangle<U> &second) {
  auto result = Rectangle<T>(0,0,0,0);
  result.setX(first.getX() * second.getX());
  result.setY(first.getY() * second.getY());
  result.setWidth(first.getWidth() * second.getWidth());
  result.setHeight(first.getHeight() * second.getHeight());
  return result;
}

template<typename T, typename U>
Rectangle<T> getRemainingRect(const Rectangle<T> &partial, const Rectangle<U> &full) {
  auto result = divideRect(partial, full);
  result.setWidth(1 - result.getWidth());
  result.setHeight(1 - result.getHeight());
  result = multiplyRect(result, full);
  return result;
}

template<typename T, typename U>
Rectangle<T> getRemainingHeight(const Rectangle<T> &partial, const Rectangle<U> &full) {
  auto result = divideRect(partial, full);
  result.setHeight(1 - result.getHeight());
  result = multiplyRect(result, full);
  result.setWidth(partial.getWidth());
  return result;
}