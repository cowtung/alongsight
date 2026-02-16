#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "AudioComponent.h"
#include "AudioMeter.h"
#include "StaffDisplay.h"

//==============================================================================
/**
    MainComponent is the primary UI component that integrates:
    - Audio input/analysis
    - Visual audio meter
    - Musical staff display
    - Start/Stop controls
*/
class MainComponent : public juce::Component,
                      private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void startStopButtonClicked();

    AudioComponent audioComponent;
    AudioMeter audioMeter;
    StaffDisplay staffDisplay;
    juce::TextButton startStopButton;
    juce::Label titleLabel;
    juce::Label statusLabel;

    bool isRunning{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
