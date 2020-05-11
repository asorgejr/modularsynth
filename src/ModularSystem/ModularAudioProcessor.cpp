//
// Created by asorgejr on 5/5/2020.
//

#include "ModularAudioProcessor.h"

#include <memory>
#include "ModularGraphView.h"

using namespace std;

#pragma region ModularAudioProcessor

ModularAudioProcessor::
ModularAudioProcessor(const sptr<AudioDeviceManager> &audioDeviceManager)
: AudioProcessor(BusesProperties()
  .withInput("Input", AudioChannelSet::stereo(), true)
  .withOutput("Output", AudioChannelSet::stereo(), true)),
  audioDeviceManager(audioDeviceManager),
  graph(std::make_shared<Graph>()),
  mainProcessor(make_shared<AudioProcessorGraph>()),
  undoManager(make_unique<UndoManager>())
{
  auto inputDevice = MidiInput::getDevices()[MidiInput::getDefaultDeviceIndex()];
  auto outputDevice = MidiOutput::getDevices()[MidiOutput::getDefaultDeviceIndex()];
  mainProcessor->enableAllBuses();
  this->audioDeviceManager->setMidiInputEnabled(inputDevice, true);
  this->audioDeviceManager->setDefaultMidiOutput(outputDevice);
}

#pragma region Audio IO Graph

void ModularAudioProcessor::
initialiseMasterNodes() {
  masterAudioIn = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode)));
  masterAudioOut = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode)));
  masterMidiIn = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode)));
  masterMidiOut = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode)));
}

void ModularAudioProcessor::
initialiseGraph() {
  mainProcessor->clear();

  connectAudioNodes();
  connectMidiNodes();
}

void ModularAudioProcessor::
connectAudioNodes() {
  for (int channel = 0; channel < kChannelCount; ++channel)
    mainProcessor->addConnection({
      {getMasterAudioIn()->nodeID,  channel},
      {getMasterAudioOut()->nodeID, channel}
    });
}

void ModularAudioProcessor::
connectMidiNodes() {
  mainProcessor->addConnection({
    {getMasterMidiIn()->nodeID,  AudioProcessorGraph::midiChannelIndex},
    {getMasterMidiOut()->nodeID, AudioProcessorGraph::midiChannelIndex}
  });
}

AudioNode::Ptr ModularAudioProcessor::
getMasterAudioIn() { 
  if (masterMidiIn == nullptr)
    masterAudioIn = mainProcessor->addNode(
      static_cast<uptr<AudioProcessor>>(
        make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode)));
  return masterAudioIn; 
}

AudioNode::Ptr ModularAudioProcessor::
getMasterAudioOut() {
  if (masterAudioOut == nullptr)
    masterAudioOut = mainProcessor->addNode(
      static_cast<uptr<AudioProcessor>>(
        make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode)));
  return masterAudioOut; 
}

AudioNode::Ptr ModularAudioProcessor::
getMasterMidiIn() {
  if (masterMidiIn == nullptr)
    masterMidiIn = mainProcessor->addNode(
      static_cast<uptr<AudioProcessor>>(
        make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode)));
  return masterMidiIn; 
}

AudioNode::Ptr ModularAudioProcessor::
getMasterMidiOut() {
  if (masterMidiOut == nullptr)
    masterMidiOut = mainProcessor->addNode(
      static_cast<uptr<AudioProcessor>>(
        make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode)));
  return masterMidiOut; 
}


#pragma endregion Audio IO Graph

#pragma region AudioProcessor Implementations

bool ModularAudioProcessor::
isBusesLayoutSupported(const BusesLayout &layouts) const {
  if (layouts.getMainInputChannelSet() == AudioChannelSet::disabled()
    || layouts.getMainOutputChannelSet() == AudioChannelSet::disabled())
    return false;

  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
    && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

  return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}

const String ModularAudioProcessor::
getName() const {
  return String();
}

void ModularAudioProcessor::
prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) {
  auto audioDevice = audioDeviceManager->getCurrentAudioDevice();
  auto spb = audioDevice->getCurrentBufferSizeSamples();
  mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(),
    getMainBusNumOutputChannels(),
    sampleRate, config::samplesPerBlock);

  mainProcessor->prepareToPlay(sampleRate, config::samplesPerBlock);

//  initialiseGraph();
}

void ModularAudioProcessor::
releaseResources() {
  mainProcessor->releaseResources();
}

void ModularAudioProcessor::
processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages) {
  for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // updateGraph();

  mainProcessor->processBlock(buffer, midiMessages);
}

double ModularAudioProcessor::
getTailLengthSeconds() const {
  return 0;
}

bool ModularAudioProcessor::
acceptsMidi() const {
  return true;
}

bool ModularAudioProcessor::
producesMidi() const {
  return true;
}

AudioProcessorEditor * ModularAudioProcessor::
createEditor() {
  return new ModularGraphView(this, graph);
}

bool ModularAudioProcessor::
hasEditor() const {
  return true;
}

int ModularAudioProcessor::
getNumPrograms() {
  return 0;
}

int ModularAudioProcessor::
getCurrentProgram() {
  return 0;
}

void ModularAudioProcessor::
setCurrentProgram(int index) {

}

const String ModularAudioProcessor::
getProgramName(int index) {
  return String();
}

void ModularAudioProcessor::
changeProgramName(int index, const String &newName) {

}

void ModularAudioProcessor::
getStateInformation(MemoryBlock &destData) {

}

void ModularAudioProcessor::
setStateInformation(const void *data, int sizeInBytes) {

}

#pragma endregion AudioProcessor Implementations

#pragma endregion ModularAudioProcessor
