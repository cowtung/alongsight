#include "AudioMeter.h"

//==============================================================================
AudioMeter::AudioMeter()
{
    startTimerHz(30); // Update display at 30 FPS
}

void AudioMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Draw background
    g.setColour(juce::Colours::darkgrey);
    g.fillRoundedRectangle(bounds, 5.0f);
    
    // Draw border
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(bounds, 5.0f, 2.0f);
    
    // Draw level meter
    if (displayLevel > 0.001f)
    {
        auto levelBounds = bounds.reduced(4.0f);
        float fillHeight = levelBounds.getHeight() * displayLevel;
        
        // Create gradient from green to yellow to red
        juce::ColourGradient gradient(
            juce::Colours::green, levelBounds.getX(), levelBounds.getBottom(),
            juce::Colours::red, levelBounds.getX(), levelBounds.getY(),
            false);
        gradient.addColour(0.5, juce::Colours::yellow);
        
        g.setGradientFill(gradient);
        g.fillRoundedRectangle(
            levelBounds.getX(),
            levelBounds.getBottom() - fillHeight,
            levelBounds.getWidth(),
            fillHeight,
            3.0f);
    }
    
    // Draw scale marks
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    for (int i = 1; i < 4; ++i)
    {
        float y = bounds.getY() + (bounds.getHeight() * i / 4);
        g.drawHorizontalLine((int)y, bounds.getX() + 2, bounds.getRight() - 2);
    }
}

void AudioMeter::resized()
{
    // No child components to layout
}

void AudioMeter::setLevel(float newLevel)
{
    level = juce::jlimit(0.0f, 1.0f, newLevel);
}

void AudioMeter::timerCallback()
{
    // Smoothly decay the display level
    if (level > displayLevel)
        displayLevel = level;
    else
        displayLevel *= decayRate;
    
    repaint();
}
