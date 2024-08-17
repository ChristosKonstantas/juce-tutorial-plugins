#pragma once
#include <vector>

/*
 * This class holds a waveTable and a samplingRate for the looping of the WaveTable
 */
class WavetableOscillator
{
public:
	// constructor of WavetableOscillator
	WavetableOscillator(std::vector<float> waveTable, double sampleRate);
	void setFrequency(float frequency);
	float getSample();
	void stop();
	bool isPlaying();
private:
	float interpolateLinearly();
	std::vector<float> waveTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;

};