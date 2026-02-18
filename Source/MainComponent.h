#pragma once

#include "AudioComponent.h"
#include "StaffDisplay.h"
#include <JuceHeader.h>

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent : public juce::Component, private juce::Timer
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	//==============================================================================
	void timerCallback() override;
	void startStopButtonClicked();
	void keySignatureChanged();

	AudioComponent audioComponent;
	StaffDisplay staffDisplay;
	juce::TextButton startStopButton;
	juce::Label titleLabel;
	juce::Label statusLabel;
	juce::ComboBox keySignatureSelector;
	juce::Label keySignatureLabel;

	bool isRunning{false};


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
