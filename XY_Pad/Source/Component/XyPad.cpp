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
		if (moveCallback)
			moveCallback(getPosition().toDouble());
	}


	/*
	 *  XY Pad section
	*/

	XyPad::XyPad()
	{
		addAndMakeVisible(thumb);
		thumb.moveCallback = [&](juce::Point<double> position)
			{
				const std::lock_guard<std::mutex> lock(std::mutex);
				const auto bounds = getLocalBounds().toDouble();
				const auto w = static_cast<double>(thumbSize);
				for (auto* slider : xSliders)
				{
					slider->setValue(juce::jmap(position.getX(), 0.0, bounds.getWidth() - w, slider->getMinimum(), slider->getMaximum()));
				}
				for (auto* slider : ySliders)
				{
					slider->setValue(juce::jmap(position.getY(), bounds.getHeight() - w, 0.0, slider->getMinimum(), slider->getMaximum()));
				}
				repaint();
			};
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

	// connect XyPad thumb with the knobs
	void XyPad::registerSlider(juce::Slider* slider, Axis axis)
	{
		// Implement some sort of Lock so when you're registering a slider the de-registration
		// can't happen.

		/* Lock */
		// Whenever a section of the code hold on to this vector mutex no other section which
		// requires the mutex as well can execute. It will have to wait till whatever piece of
		// code that is using the resource freeze it up or unlocks before it can actually go
		// ahead and use it.
		// The std::lock_guard class is a scoped lock meaning as soon as it loses scope (method ends)
		// and the lock_guard object goes out of scope its destructor is called and is the
		// responsibility of the destructor to release any locks that it may have for the mutex.
		// What we need to remember is that wherever we're going to access the x and y sliders
		// we need to lock that method or at least lock that scope of usage.
		const std::lock_guard<std::mutex> lock(vectorMutex);
		if (axis == Axis::X)
			xSliders.push_back(slider); // push the Slider pointer into the xSliders collection
		if (axis == Axis::Y)
			ySliders.push_back(slider);
	}

	// remove the slider from the vector

	void XyPad::deregisterSlider(juce::Slider* slider)
	{
		/* Lock */
		const std::lock_guard<std::mutex> lock(vectorMutex);

		/* remove/erase idiom*/
		// This particular std::remove() algorithm does not actually remove anything but it returns
		// an iterator. This algorithm is kind of a filter+sort algorithm.
		// It goes through the range that is specified within its collection and finds all sliders
		// that do not match the filter term that we've given here. All the sliders that are not
		// 'slider' are moved to the front of the collection. Any slider that matches the slider that
		// we provide here will be moved to the end of the collection. So we are sorting the
		// collection. In the end it will give you an iterator on the position of where this filtered
		// slider is withing the collection and then you pass this iterator into the erase method of
		// the vector class that accepts an iterator and an endpoint. We'll remove all elements
		// from wherever the std remove iterator points to, to the end of the vector itself.
		xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
		ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
		// Example: Let's say that are plenty of sliders that you registered which are the same
		// slider. So you've registered one slider many number of times and once you deregister
		// that slider it will remove all of the many duplicated registrations as well.
	}


}
