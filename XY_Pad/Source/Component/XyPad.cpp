#include "XyPad.h"

namespace Gui
{

	/*
	 *  XY Pad Thumb section
	 */
	XyPad::Thumb::Thumb()
	{
		// Set the values on which the component is allowed to go off-screen ,i.e,
		// how many pixels should remain on the screen when the component is dragged off one
		// of the parent's edges.
		constrainer.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize);
	}

	void XyPad::Thumb::paint(juce::Graphics& g)
	{
		g.setColour(juce::Colours::white);
		g.fillEllipse(getLocalBounds().toFloat());
	}

	// to inform the dragger class that something is happening
	void XyPad::Thumb::mouseDown(const juce::MouseEvent& event)
	{
		dragger.startDraggingComponent(this, event);
	}

	// to inform the dragger class that something is happening
	void XyPad::Thumb::mouseDrag(const juce::MouseEvent& event)
	{
		dragger.dragComponent(this, event, &constrainer);
	}


	/*
	 *  XY Pad section
	*/

	XyPad::XyPad()
	{
		addAndMakeVisible(thumb);
	}

	// Contain XyPad in the center
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
