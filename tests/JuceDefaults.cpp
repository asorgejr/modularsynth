// JuceDefaults.cpp
// Tests the default values of Juce libraries included in ModularSynth.
#include <iostream>
#include <string>
#include <JuceHeader.h>
#include <boost/assert.hpp>

namespace modular_synth_tests {
  
using namespace std;

int main() {
  cout << "JuceDefaults Test Program: " << endl;
  auto LF = LookAndFeel_V4();
  cout << &LF << endl;
  return 0;
}


}