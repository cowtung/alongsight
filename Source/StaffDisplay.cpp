#include "StaffDisplay.h"

//==============================================================================
StaffDisplay::StaffDisplay()
{
}

void StaffDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(20.0f);
    
    // Draw background
    g.setColour(juce::Colours::white);
    g.fillRoundedRectangle(bounds, 5.0f);
    
    // Draw border
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(bounds, 5.0f, 2.0f);
    
    // Draw staff
    auto staffBounds = bounds.reduced(40.0f, 60.0f);
    drawStaffLines(g, staffBounds);
    drawClef(g, staffBounds);
}

void StaffDisplay::resized()
{
    auto bounds = getLocalBounds();
    staffLineSpacing = (bounds.getHeight() - 160.0f) / (numStaffLines - 1);
    staffLineSpacing = juce::jlimit(8.0f, 25.0f, staffLineSpacing);
}

void StaffDisplay::clearStaff()
{
    repaint();
}

void StaffDisplay::drawStaffLines(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setColour(juce::Colours::black);
    
    // Calculate staff positioning
    float totalStaffHeight = staffLineSpacing * (numStaffLines - 1);
    float startY = bounds.getCentreY() - totalStaffHeight / 2.0f;
    
    // Draw the 5 staff lines
    for (int i = 0; i < numStaffLines; ++i)
    {
        float y = startY + (i * staffLineSpacing);
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 1.5f);
    }
}

void StaffDisplay::drawClef(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setColour(juce::Colours::black);
    
    // Calculate staff positioning
    float totalStaffHeight = staffLineSpacing * (numStaffLines - 1);
    float startY = bounds.getCentreY() - totalStaffHeight / 2.0f;
    float clefX = bounds.getX() + 10.0f;
    
    // Draw a simplified treble clef using vector paths for cross-platform consistency
    // The treble clef wraps around the G line (2nd line from bottom)
    float gLineY = startY + (3 * staffLineSpacing);
    
    juce::Path clefPath;
    float clefWidth = staffLineSpacing * 1.5f;
    float clefHeight = staffLineSpacing * 3.5f;
    
    // Create a stylized treble clef shape using bezier curves
    // Starting from bottom, curving up and around
    float x = clefX;
    float y = gLineY;
    
    // Simplified clef shape - a spiral that wraps around G line
    clefPath.startNewSubPath(x + clefWidth * 0.5f, y + clefHeight * 0.3f);
    clefPath.quadraticTo(
        x + clefWidth * 0.2f, y + clefHeight * 0.1f,
        x + clefWidth * 0.4f, y - clefHeight * 0.2f);
    clefPath.quadraticTo(
        x + clefWidth * 0.7f, y - clefHeight * 0.4f,
        x + clefWidth * 0.5f, y - clefHeight * 0.1f);
    clefPath.quadraticTo(
        x + clefWidth * 0.3f, y + clefHeight * 0.1f,
        x + clefWidth * 0.5f, y + clefHeight * 0.3f);
    
    // Draw the path with a stroke
    g.strokePath(clefPath, juce::PathStrokeType(2.0f));
    
    // Add a dot for the bottom of the clef
    g.fillEllipse(x + clefWidth * 0.4f, y + clefHeight * 0.35f, 3.0f, 3.0f);
}
