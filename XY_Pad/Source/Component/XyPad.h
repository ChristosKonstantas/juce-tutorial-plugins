#pragma once

#include <JuceHeader.h>

namespace Gui
{
	/* Custom JUCE component XyPad*/
	class XyPad : public juce::Component // this class inherits from juce::Component class
	{
	public:
		enum class Axis{X,Y};

		class Thumb : public Component
		{
		public:
			Thumb();
			void paint(juce::Graphics& g) override;
		private:
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb);
		};

		XyPad();
		void resized() override;
		void paint(juce::Graphics& g) override;

		// registerSlider(): A mechanism to register the slider
		//we will need to have a reference to the slider and not own the slider
		//we need to have context on which axis of the XyPad is this slider referencing (U/D or L/R)
		void registerSlider(juce::Slider* slider, Axis);

		// deregisterSlider(): A mechanism to de-register the slider
		// We do not need the Axis context here because we can search the slider and delete it
		// from wherever necessary.
		void deregisterSlider(juce::Slider* slider);

	private:
		// A collection of registered sliders
		std::vector<juce::Slider*> xSliders, ySliders;
		Thumb thumb;
		static constexpr int thumbSize = 40;
		// Below macro is good to have within custom juce component because if someone creates a
		// raw instance of the XyPad and forgets to delete it the developer will get informed that
		// there is a leak. It declares this particular class (XyPad) as non-copyable and non-moveable
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XyPad);
	};
}