//
// Created by asorgejr on 4/23/2020.
//

#pragma once
#include <JuceHeader.h>

#define NAMEOF_FIELD(...) ((String)((string) NAMEOF(__VA_ARGS__)))

#define LOG(...) Logger::writeToLog(__VA_ARGS__);


template<class T> using sptr = std::shared_ptr<T>;
template<class T> using uptr = std::unique_ptr<T>;
template<class T> using Shareable = std::enable_shared_from_this<T>;

typedef AudioDeviceManager::AudioDeviceSetup AudioDeviceSetup;
