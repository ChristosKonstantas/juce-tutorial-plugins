/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
XY_PadAudioProcessor::XY_PadAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
//*this is for dereferencing the reference for the audio processor class itself
// nullptr is for undo manager, and we do not need that at this point
// "xypad" parent identifier

// in the parameter layout we pass an initializer list of unique audio parameters 1 for pan and 1 for Gain
// ("pan", "Pan") -> 1st argument is a unique identifier and 2nd argument is the name of this parameter and is going to be exposed
// as an automation parameter as well and is going to be displayed withing the list of automation parameters.
parameters(*this, nullptr, "xypad", {
	std::make_unique<juce::AudioParameterFloat>("pan","Pan",juce::NormalisableRange<float>{-1.f,1.f,0.01f},0.f),
    std::make_unique<juce::AudioParameterFloat>("gain","Gain",juce::NormalisableRange<float>{-60.f,0.f,0.01f},0.f)
})
{
    parameters.addParameterListener("gain", this);
    parameters.addParameterListener("pan", this);
}

XY_PadAudioProcessor::~XY_PadAudioProcessor()
{
    parameters.removeParameterListener("gain", this);
    parameters.removeParameterListener("pan", this);
}

//==============================================================================
const juce::String XY_PadAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool XY_PadAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool XY_PadAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool XY_PadAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double XY_PadAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int XY_PadAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int XY_PadAudioProcessor::getCurrentProgram()
{
    return 0;
}

void XY_PadAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String XY_PadAudioProcessor::getProgramName (int index)
{
    return {};
}

void XY_PadAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void XY_PadAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{   // a small enumeration that contains sampleRate, samplesPerBlock, getTotalNumInputChannels() 
    const juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(getTotalNumInputChannels()) };

    gainProcessor.prepare(spec);
    panProcessor.prepare(spec);
    // to make less error prone code is to make these identifiers a sort of static constant values
    // that you can reference across your project because if you have a lot of parameters to tend
    // to get lost with the naming but here we have only Gain and Pan so we are ok
    gainProcessor.setGainDecibels(parameters.getRawParameterValue("gain")->load());
    panProcessor.setPan(parameters.getRawParameterValue("pan")->load());
}

void XY_PadAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

    gainProcessor.reset();
    panProcessor.reset();
}

bool XY_PadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void XY_PadAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // The process method requires a ProcessContextReplacing context meaning that input and
    // output buffers are shared so changing the one would impact the other, and this in turn
    // accepts an AudioBlock which can be constructed from an audio buffer.
    // The audioBlock is a pretty inexpensive wrapper around the audio buffer. It does not actually
    // own any of the data it just acts as a wrapper essentially.
    juce::dsp::AudioBlock<float> audioBlock{ buffer };
    juce::dsp::ProcessContextReplacing<float> context{ audioBlock };
    gainProcessor.process(context);
    panProcessor.process(context);
}

//==============================================================================
bool XY_PadAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* XY_PadAudioProcessor::createEditor()
{
    return new XY_PadAudioProcessorEditor (*this);
}

//==============================================================================
void XY_PadAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void XY_PadAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void XY_PadAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
	// Over here if you pass two arguments: parameterID of the parameter that changed
    // and the newValue that it changed too. So we want to query the parameters here.

    if (parameterID.equalsIgnoreCase("gain"))
        gainProcessor.setGainDecibels(newValue);
    if (parameterID.equalsIgnoreCase("pan"))
        panProcessor.setPan(newValue);
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new XY_PadAudioProcessor();
}
