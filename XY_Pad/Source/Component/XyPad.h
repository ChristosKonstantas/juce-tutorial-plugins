#pragma once

#include <JuceHeader.h>

namespace Gui // for all Gui components
{
	/* Custom JUCE component XyPad*/
	class XyPad : public juce::Component, juce::Slider::Listener
	{
	public:
		

		class Thumb : public Component
		{
		public:
			Thumb();
			void paint(juce::Graphics& g) override;
			// mouse listener members
			void mouseDown(const juce::MouseEvent& event) override;
			void mouseDrag(const juce::MouseEvent& event) override;
			std::function<void(juce::Point<double>)> moveCallback; //(*)
		private:
			// ComponentDragger is a class that allows to drag this component withing its parent
			juce::ComponentDragger dragger;
			// ComponentBoundsConstrainer is a little boundary established within the parent beyond
			// which you would not be able to drag this component. It is important s.t.
			// XY thumb will not move out of XY pad
			juce::ComponentBoundsConstrainer constrainer;
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb);
		};

		XyPad();
		void resized() override;
		void paint(juce::Graphics& g) override;

		enum class Axis { X, Y };
		// registerSlider(): A mechanism to register the slider
		//we will need to have a reference to the slider and not own the slider
		//we need to have context on which Axis of the XyPad is this slider referencing (U/D or L/R)
		void registerSlider(juce::Slider* slider, Axis);

		// deregisterSlider(): A mechanism to de-register the slider
		// We do not need the Axis context here because we can search the slider and delete it
		// from wherever necessary.
		void deregisterSlider(juce::Slider* slider);

	private:
		void sliderValueChanged(juce::Slider*) override;
		// A collection of registered sliders
		std::vector<juce::Slider*> xSliders, ySliders;
		Thumb thumb; // instance of Thumb Class within its parent
		std::mutex vectorMutex;
		// static constexpr int guarantees compile - time evaluation.
		static constexpr int thumbSize = 40; 
		// Below macro is good to have within custom juce component because if someone creates a
		// raw instance of the XyPad and forgets to delete it the developer will get informed that
		// there is a leak. It declares this particular class (XyPad) as non-copyable and non-moveable
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XyPad);
	};
}


//(*):
// xSlider and ySlider vectors are not present in the Thumb class, but they are present in the parent
// class. The xSlider and ySlider vectors are present within the XyPad which the thumb does not
// really have access to. The Thumb should not really be changing and manipulating the vectors
// themselves because it's not really the responsibility of the thumb to change the slider values
// that the parent maintains.
// So we need some sort of Callback method that the parent can pass s.t. whenever the thumb is moved
// the callback method is executed. This way the thumb does not need to know what happens it just
// needs to know that there's a Callback that needs to be called, but it does not really need to
// understand any technicalities of the Callback itself.
// So: we can create a function pointer std::function<void(juce::Point<double>)> moveCallback;
// and this function will return void, and it will accept a point. This point is the XY position
// of the thumb itself. So the signature of the method that the Thumb class will accept is
// a function which returns void and accepts a position parameter. The thumb is perfectly capable
// of providing its own position; whoever passes this function pointer signature to this function
// to move callback will utilize this point (this position) and do something with it.
// We need to do use the moveCallback() whenever the mouse is dragged.
// See (void XyPad::Thumb::mouseDrag(const juce::MouseEvent& event)) inside XyPad.cpp