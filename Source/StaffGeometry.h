#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <array>

//==============================================================================
/**
 * StaffGeometry - Unified positioning system for musical staff
 * 
 * This class encapsulates ALL positioning math for a musical staff, ensuring
 * that staff lines and notes are always perfectly aligned. It takes a bounding
 * box and calculates all positions from a single source of truth.
 * 
 * Key principles:
 * - Single source of truth for all Y positions
 * - Staff lines and note positions use the same coordinate system
 * - Immutable once constructed (prevents drift)
 * - All positions calculated from the same base values
 */
class StaffGeometry
{
public:
	/** Create geometry for a staff within the given bounds */
	StaffGeometry(juce::Rectangle<float> bounds, float lineSpacing);
	
	/** Get Y position for a staff line by index (0 = bottom line, 4 = top line) */
	float getLineY(int lineIndex) const;
	
	/** Get Y position for a staff position (0 = bottom line, negative = below, positive = above)
	 *  Staff positions represent both lines and spaces in a diatonic scale.
	 *  Even positions (0,2,4,6,8) = lines, Odd positions (1,3,5,7) = spaces
	 */
	float getPositionY(int staffPosition) const;
	
	/** Get all 5 staff line Y positions for efficient drawing */
	std::array<float, 5> getAllLineYPositions() const;
	
	/** Get the staff line spacing */
	float getLineSpacing() const { return lineSpacing; }
	
	/** Get the Y coordinate of the top of the staff (top line) */
	float getStaffTop() const { return topLineY; }
	
	/** Get the Y coordinate of the bottom of the staff (bottom line) */
	float getStaffBottom() const { return bottomLineY; }
	
	/** Get the left X coordinate where staff lines begin */
	float getStaffLeft() const { return staffLeft; }
	
	/** Get the right X coordinate where staff lines end */
	float getStaffRight() const { return staffRight; }
	
	/** Check if a staff position requires ledger lines below the staff */
	bool needsLedgerLinesBelow(int staffPosition) const { return staffPosition < 0; }
	
	/** Check if a staff position requires ledger lines above the staff */
	bool needsLedgerLinesAbove(int staffPosition) const { return staffPosition > 8; }

private:
	float topLineY;      // Y coordinate of top staff line (line 4)
	float bottomLineY;   // Y coordinate of bottom staff line (line 0)
	float lineSpacing;   // Distance between adjacent staff lines
	float staffLeft;     // X coordinate where staff begins
	float staffRight;    // X coordinate where staff ends
	
	static constexpr int NUM_LINES = 5;
};
