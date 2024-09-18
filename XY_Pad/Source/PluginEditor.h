/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class XY_PadAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    XY_PadAudioProcessorEditor (XY_PadAudioProcessor&);
    ~XY_PadAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    XY_PadAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XY_PadAudioProcessorEditor)
};
