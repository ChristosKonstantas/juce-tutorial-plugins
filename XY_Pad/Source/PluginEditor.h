/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Component/XyPad.h"
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

    juce::Slider gainSlider {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };

    juce::Slider panSlider{ juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };

    // An easy way to maintain state within the processor and the editor is to leverage apvts
    // slider attachment s.t. there's synchronicity between the editor and the processor
    // The value of the gain and the pan parameters could change directly from the editor
    // when the user when the user moves the slider or it can change from some automation curve
    // that the user has defined. Either way it will be reflected in both the processor and the
    // editor so what we need to do is to attach these sliders to apvts
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment, panAttachment;

    juce::Label gainLabel{ "gainLabel", "Gain" };
    juce::Label panLabel{ "panLabel", "Pan" };
    Gui::XyPad xyPad;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XY_PadAudioProcessorEditor)
};
