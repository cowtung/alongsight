#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/**
    StaffDisplay renders a basic musical staff for sight-reading exercises
*/
class StaffDisplay : public juce::Component
{
public:
    StaffDisplay();
    ~StaffDisplay() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Future: methods for displaying notes, chords, etc.
    void clearStaff();

private:
    void drawStaffLines(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawClef(juce::Graphics& g, juce::Rectangle<float> bounds);

    static constexpr int numStaffLines = 5;
    float staffLineSpacing{10.0f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StaffDisplay)
};
