#include "WavetableOscillator.h"
#include <cmath>
WavetableOscillator::WavetableOscillator(std::vector<float> waveTable, double sampleRate)
	:waveTable{ std::move(waveTable) },
	sampleRate{ sampleRate }
{

}

/* We need 2 functions here.
 * 1st one that sets the frequency of this particular oscillator.
 * 2nd one that loops over the waveTable. This is the core of the Wavetable Synthesis algorithm.
 */

 // Here we calculate the indexIncrement. 
void WavetableOscillator::setFrequency(float frequency)
{
	indexIncrement = frequency * static_cast<float>(waveTable.size()) / static_cast <float>(sampleRate);
}


float WavetableOscillator::getSample()
{
	// We need to retrieve the sample using linear interpolation
	const auto sample = interpolateLinearly();
	// We need it to be a member function to have access to the waveTable index and waveTable size which are
	// necessary for the linear interpolation.
	index += indexIncrement;
	// fmod to take our index and bring it back to the wave table size range
	index = std::fmod(index, static_cast<float>(waveTable.size()));
	return sample;
}

float WavetableOscillator::interpolateLinearly()
{
	/* if we have an index between two integer indices then we return the weighted sum of the
	 * waveTable values that correspond to these nearest integer indices.
	 * The weights that we take between those 2 values are the distances from the index to the next integer
	 * index. We may weight the sample that the index is nearer to with a larger weight. To truncate the index we just
	 * use the static cast to the integer because it needs to be a floating point number. The range is (0,1) and then is
	 * normalized to the range from defined by waveTable size.
	 */
	const auto truncatedIndex = static_cast<int>(index);
	const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(waveTable.size());
	const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
	const auto truncatedIndexWeight = 1.f - nextIndexWeight;

	return truncatedIndexWeight * waveTable[truncatedIndex] + nextIndexWeight * waveTable[nextIndex];
}

void WavetableOscillator::stop()
{
	/* Will reset the index and index increment to 0. In this way even if the getSample() member function will be
	 * called, we'll be sure that it won't return anything meaningful. It is also important that the calling code
	 * won't call getSample() if the oscillator is stopped.
	 */
	index = 0.f;
	indexIncrement = 0.f;
}

bool WavetableOscillator::isPlaying()
{
	/* The calling code should ideally 'ask' the oscillator if it is playing and determine
	 * if its index increment is not equal to 0. If it is, there is no looping on the waveTable.
	 */
	return indexIncrement != 0.f;
}
