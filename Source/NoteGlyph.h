#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <map>

//==============================================================================
/**
 * NoteGlyph - Encapsulates music note glyph information
 * 
 * This class manages SMuFL note glyphs and their positioning information.
 * It pre-calculates center points for each note type based on actual glyph
 * measurements, ensuring consistent note head positioning regardless of stem.
 * 
 * The singleton pattern ensures glyphs are measured once and reused.
 */
class NoteGlyph
{
public:
	enum class Type
	{
		Whole,      // Whole note (no stem)
		Up2,        // Half note, stem up
		Up4,        // Quarter note, stem up
		Up8,        // Eighth note, stem up
		Up16,       // Sixteenth note, stem up
		Down2,      // Half note, stem down
		Down4,      // Quarter note, stem down
		Down8,      // Eighth note, stem down
		Down16,     // Sixteenth note, stem down
		Sharp,      // Sharp accidental
		Flat        // Flat accidental
	};
	
	struct GlyphInfo
	{
		juce::String glyphCode;      // SMuFL Unicode character
		juce::Path glyphPath;        // Pre-created path for the glyph
		juce::Rectangle<float> bounds; // Original bounds of the path
		float centerPointX;          // X position of note head center (relative to glyph bounds)
		float centerPointY;          // Y position of note head center (relative to glyph bounds)
		bool hasStem;                // Whether this note type has a stem
		bool stemUp;                 // Stem direction (true = up, false = down)
	};
	
	// Singleton access
	static NoteGlyph& getInstance();
	
	// Get glyph information for a note type
	const GlyphInfo& getGlyphInfo(Type type) const;
	
	// Get SMuFL character code for a note type
	juce::String getGlyphCode(Type type) const;
	
	// Get the measured whole note head height (one staff space)
	float getWholeNoteHeight() const { return wholeNoteHeight; }

	// Get the measured whole note head width
	float getWholeNoteWidth() const { return wholeNoteWidth; }

	// Calculate the scale factor needed to make note heads one staff space tall
	float getScaleFactor(float targetStaffLineSpacing) const;
	
private:
	NoteGlyph();  // Private constructor for singleton
	~NoteGlyph() = default;
	
	// Delete copy/move constructors and assignment operators
	NoteGlyph(const NoteGlyph&) = delete;
	NoteGlyph& operator=(const NoteGlyph&) = delete;
	NoteGlyph(NoteGlyph&&) = delete;
	NoteGlyph& operator=(NoteGlyph&&) = delete;
	
	// Initialize the glyph map with SMuFL codes
	void setupGlyphCodes();
	
	// Measure glyphs and calculate center points
	void measureGlyphs();
	
	std::map<Type, GlyphInfo> glyphMap;
	bool initialized = false;
	float wholeNoteHeight = 0.0f;
	float wholeNoteWidth = 0.0f;
};
