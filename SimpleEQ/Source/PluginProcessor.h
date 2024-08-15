/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SimpleEQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleEQAudioProcessor();
    ~SimpleEQAudioProcessor() override;

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

    /* Audio plugins depend on parameters to control the various parts of the DSP.
     * JUCE uses an object called the AudioProcessorValueTreeState to co-ordinate
     * sync these parameters with the knobs on the GUI and the variables in the DSP.
     * Therefore, we need them in our audio processor that needs to be public so that
     * the GUI can attach all of its knobs, sliders, etc -- apvts --
     */

    /* *this is the AudioProcessor to connect to, nullptr for no undo manager
     */
    static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };
     
private:

    // creation of filter alias
    using Filter = juce::dsp::IIR::Filter<float>;
  
    /* have set the slopes of our cut filters to be multiples of 12
     * each of the filter types in the IIR filter class has a response of
     * 12 dB per octave when it is configures as a low pass or high pass filter.
     * If we want to have a chain with a response of 48 dB per octave we are going to need
     * 4 of those filters.
     * A central concept of the DSP namespace in JUCE framework is to define a chain and pass in a
     * processing context which will run through each element of the chain automatically
     */

    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;

    /* We can use one filter to represent the parametric filter so now that we have
     * the cut filter and the peak filter represented as aliases we can define a chain
     * to represent the whole mono signal path
     */

    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;

    /* we want two instances of the mono chain to do stereo processing
     * and to have access to the filter instances in order to adjust their cutoff gain,
     * quality or slope
     */

     // Declare the DSP chains for the left and right channels
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> leftChain;
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> rightChain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEQAudioProcessor)
};
