#include "XyPad.h"

namespace Gui
{

	/*
	 *  XY Pad Thumb section
	 */
	XyPad::Thumb::Thumb(){}

	void XyPad::Thumb::paint(juce::Graphics& g)
	{
		g.setColour(juce::Colours::white);
		g.fillEllipse(getLocalBounds().toFloat());
	}



	/*
	 *  XY Pad section
	*/

	XyPad::XyPad()
	{
		addAndMakeVisible(thumb);
	}


	void XyPad::resized()
	{
		thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbSize,thumbSize));
	}

	void XyPad::paint(juce::Graphics& g)
	{
		g.setColour(juce::Colours::black);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
	}


}
