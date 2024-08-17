#include "WavetableSynth.h"

std::vector<float> WavetableSynth::generateSineWaveTable()
{
	// Fix length of wave table to a certain amount of samples (ex: 64)
	constexpr auto WAVETABLE_LENGTH = 64;
	std::vector<float> sineWaveTable(WAVETABLE_LENGTH);

	/* Now we need to loop over the wavetable length and assign each point a value of a sine function and
	 * we want to have just one period of the sine stored.
	 */
	const auto PI = std::atanf(1.f) * 4;

	for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
	{
		sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / static_cast<float>(WAVETABLE_LENGTH));
	}

	return sineWaveTable;
}

void WavetableSynth::initializeOscillators()
{
	/* 128 oscillators initialization because we want to have a polyphonic waveTable synthesizer so that
	 * we can play multiple keys at once, so each oscillator will be assigned to its own unique key.
	 * To initialize them we need to pass them the waveTable thus the waveTable must be generated.
	 */
	constexpr auto OSCILLATORS_COUNT = 128; //the number of midi note numbers that we have available

	const auto waveTable = generateSineWaveTable();
	/* When we initialize our oscillators we need to clear them first because if we change the sampling rate during
	 * processing it may happen that we already had some oscillators. The placed back takes the arguments of the
	 * constructor of the oscillator.
	 */
	oscillators.clear();
	for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
	{
		oscillators.emplace_back(waveTable, sampleRate);
	}
}


void WavetableSynth::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;

	// we need to run this when sampling rate changes because the oscillators will also depend on the sampling rate

	initializeOscillators();
}

/* We have our block of samples and at some points midi messages may have happened. We want to read out these
 * midi messages and render the sound in between these midi messages, because in between no synthesizer parameters
 * were changed. So the environment stays constant and our sound can get generated. We're doing here this
 * top-down approach. This code that we're writing is general to every synthesizer that we may write with JUCE.
 */
void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	// start of processBlock
	auto currentSample = 0;
	// iterate over the midi buffer
	/* midiMessages contains instances of class MidiMetadata and to get the actual message we need to call
	 * getMessage() member function
	 */
	for (const auto midiMessage : midiMessages)
	{
		const auto midiEvent = midiMessage.getMessage();
		// we need to know when the midi event happens and for this we need again an index of the respective sample
		const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
		// we want to render everything between midiEventSample and midiEvent
		render(buffer, currentSample, midiEventSample);
		/* handleMidiEvent(midiEvent) member function will analyze what happened in the midi messages and we'll initiate
		 * playing some note. Afterwards we can update our currentSample to be at the index of the midiEvent.
		 * After all midiEvents have happened we need to render the last portion of the AudioBuffer.
		 */
		handleMidiEvent(midiEvent);
		currentSample = midiEventSample;
	}
	render(buffer, currentSample, buffer.getNumSamples());
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
	auto* firstChannel = buffer.getWritePointer(0);

	for (auto& oscillator : oscillators)
	{
		if (oscillator.isPlaying())
		{
			for (auto sample = startSample; sample < endSample; ++sample)
			{
				firstChannel[sample] += oscillator.getSample();
			}
		}
	}

	for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
	{
		std::copy(firstChannel + startSample, firstChannel + endSample,
			buffer.getWritePointer(channel)
			+ startSample);
	}
}

/* midiEvent gives us information on whether a key was released or there was some other type of control information.
 * We will only handle very basic type of midi note ON event and midi note OFF event. To handle these events what we need
 * is several oscillators, which are quite low-level. Usually we have voices and each voice may consist out of many
 * oscillators. Here for simplicity we have as many oscillators as the keys of a piano keyboard and each of these
 * oscillators will be capable of playing exactly that key.
 */
void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn())
	{
		//midi note number
		const auto oscillatorId = midiEvent.getNoteNumber();
		// retrieve frequency that we want to set to our oscillator
		const auto frequency = midiNoteNumberToFrequency(oscillatorId);
		// pick an oscillator from our oscillator set that we'll initialize with the computed frequency
		oscillators[oscillatorId].setFrequency(frequency);
	}
	else if (midiEvent.isNoteOff())
	{
		const auto oscillatorId = midiEvent.getNoteNumber();
		oscillators[oscillatorId].stop();
	}
	else if (midiEvent.isAllNotesOff())
	{
		for (auto& oscillator : oscillators)
		{
			oscillator.stop();
		}

	}

}

float WavetableSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
	// We want to tune our instrument to be 440Hz at A4
	constexpr auto A4_FREQUENCY = 440.f;
	constexpr auto A4_NOTE_NUMBER = 69.f;
	constexpr auto SEMITONES_IN_AN_OCATAVE = 12.f;

	// return the fundamental frequency shifted relative to its position
	return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER) / SEMITONES_IN_AN_OCATAVE);
}