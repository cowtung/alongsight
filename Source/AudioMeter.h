#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/**
    AudioMeter provides visual feedback of audio input levels
*/
class AudioMeter : public juce::Component,
                   private juce::Timer
{
public:
    AudioMeter();
    ~AudioMeter() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setLevel(float newLevel);
    float getLevel() const { return level; }

private:
    void timerCallback() override;

    float level{0.0f};
    float displayLevel{0.0f};
    static constexpr float decayRate = 0.95f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioMeter)
};
