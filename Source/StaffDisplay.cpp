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
    
    // Draw a simplified treble clef (G clef) as a stylized 'G'
    // This is a placeholder - in production, you'd use a music font
    juce::Font font(24.0f, juce::Font::bold);
    g.setFont(font);
    
    // Position the 'G' to indicate treble clef (wraps around G line - 2nd line from bottom)
    float gLineY = startY + (3 * staffLineSpacing); // 2nd line from bottom
    g.drawText("𝄞", 
               juce::Rectangle<float>(clefX - 5, gLineY - staffLineSpacing * 2, 30, staffLineSpacing * 4),
               juce::Justification::centredLeft);
}
