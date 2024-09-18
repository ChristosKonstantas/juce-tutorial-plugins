/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
XY_PadAudioProcessorEditor::XY_PadAudioProcessorEditor (XY_PadAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
	  gainAttachment(p.getApvts(), "gain", gainSlider),
      panAttachment(p.getApvts(), "pan", panSlider)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(panSlider);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(panLabel);
    addAndMakeVisible(xyPad);

    gainLabel.setJustificationType(juce::Justification::centred);
    panLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainSlider, false);
    panLabel.attachToComponent(&panSlider, false);

    setSize (500, 300);
    setResizable(true, true); //allows to resize window
}

XY_PadAudioProcessorEditor::~XY_PadAudioProcessorEditor()
{
}

//==============================================================================
void XY_PadAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void XY_PadAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    const juce::Rectangle<int> container = getLocalBounds().reduced(20);


    juce::Rectangle<int> bounds = container;
    // <--gainSlider--> 25% | <--XY Pad--> 50% | <--panSlider--> 25%
    gainSlider.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.25f)));
    xyPad.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.5)).reduced(20));
    panSlider.setBounds(bounds);
}
