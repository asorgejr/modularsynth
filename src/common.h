//
// Created by asorgejr on 4/23/2020.
//

#pragma once
#include <type_traits>
#include <nameof.hpp>
#include <JuceHeader.h>
#include "ModularAPI.h"
#include "app/config.h"
#include "Utilities.h"

#define NAMEOF_FIELD(...) ((String)((string) NAMEOF(__VA_ARGS__)))

#define LOG(...) Logger::writeToLog(__VA_ARGS__);


template<class T> using sptr = std::shared_ptr<T>;
template<class T> using uptr = std::unique_ptr<T>;
template<class T> using Shareable = std::enable_shared_from_this<T>;

typedef AudioDeviceManager::AudioDeviceSetup AudioDeviceSetup;

typedef AudioProcessorGraph::AudioGraphIOProcessor AudioGraphIOProcessor;
typedef AudioProcessorGraph::Node AudioNode;
typedef AudioProcessorGraph::NodeID NodeID;

template <typename T>
concept RefType = requires { std::is_reference<T>::value; };

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
    std::cout << "printUnpackVector error: could not stream contents of specified type: '"
    << typeid(U).name() << "'." << std::endl;
  }
}