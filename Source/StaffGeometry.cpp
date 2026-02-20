#include "StaffGeometry.h"

StaffGeometry::StaffGeometry(juce::Rectangle<float> bounds, float lineSpacing)
	: lineSpacing(lineSpacing)
{
	// Calculate staff position centered within bounds
	float totalStaffHeight = lineSpacing * (NUM_LINES - 1);

	// Center the staff vertically in the bounds
	float centerY = bounds.getCentreY();
	topLineY = centerY - (totalStaffHeight / 2.0f);
	bottomLineY = topLineY + totalStaffHeight;

	// Staff starts at the left edge of the bounds (no internal margin)
	// The bounds X position is now calculated by the layout system
	staffLeft = bounds.getX();
	staffRight = bounds.getRight();
}

float StaffGeometry::getLineY(int lineIndex) const
{
	// Line 0 = bottom line, Line 4 = top line (musically)
	// Lines are numbered from bottom to top
	// In screen coordinates: topLineY < bottomLineY (Y increases downward)
	// 
	// Line 4 (top line musically) = topLineY (low Y value, top of screen)
	// Line 0 (bottom line musically) = bottomLineY (high Y value, bottom of screen)
	jassert(lineIndex >= 0 && lineIndex < NUM_LINES);

	// Convert from musical line numbering (0=bottom) to screen position
	// Line 0 should be at bottomLineY
	// Line 4 should be at topLineY
	// So: Y = bottomLineY - (lineIndex * lineSpacing)
	return bottomLineY - (lineIndex * lineSpacing);
}

float StaffGeometry::getPositionY(int staffPosition) const
{
	// Staff position 0 = bottom line (E4 in treble clef)
	// Each position represents a diatonic step (half of lineSpacing)
	// Positive positions go UP musically (lower Y values on screen)
	// Negative positions go DOWN musically (higher Y values on screen)
	// 
	// Position mapping:
	// -2 = ledger line below staff (middle C)
	// 0 = bottom line (line 0, E4)
	// 2 = second line (line 1, G4)
	// 4 = middle line (line 2, B4)
	// 6 = fourth line (line 3, D5)
	// 8 = top line (line 4, F5)
	// 
	// In screen coordinates: higher position = lower Y value (higher on screen)
	// So we subtract from bottomLineY
	return bottomLineY - (staffPosition * lineSpacing * 0.5f);
}

std::array<float, 5> StaffGeometry::getAllLineYPositions() const
{
	std::array<float, 5> positions;
	for (int i = 0; i < NUM_LINES; ++i)
	{
		positions[i] = getLineY(i);
	}
	return positions;
}
