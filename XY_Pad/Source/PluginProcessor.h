/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * We want to respond to changes when the AudioProcessorValueTreeState parameters change.
 * A simple way to do this in the processor itself is by adding a Listener to each one of the
 * parameters that we've defined withing the AudioProcessorValueTreeState instance.
 * We'll inherit from the class juce::AudioProcessorValueTreeState::Listener class
*/
class XY_PadAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    XY_PadAudioProcessor();
    ~XY_PadAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getApvts();
private:
    //==============================================================================
    // declare the parameters that will be available for this application in the apvts class
    juce::AudioProcessorValueTreeState parameters;

    // the dsp processors implement the same methods that are part of the processor base class
    // withing the dsp module; 3 methods that you need to call in the .cpp file
    // (1) prepare() , (2) process() and (3) reset()
    juce::dsp::Gain<float> gainProcessor;
    juce::dsp::Panner<float> panProcessor;

    // A pure virtual function that we need to override and its called parameterChanged()
    void parameterChanged(const juce::String& parameterID, float newValue) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XY_PadAudioProcessor)
};
