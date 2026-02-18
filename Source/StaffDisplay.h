#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include "StaffGeometry.h"
#include "NoteGlyph.h"

//==============================================================================
/**
	StaffDisplay renders a piano grand staff (treble and bass clefs) for sight-reading exercises
*/
class StaffDisplay : public juce::Component
{
  public:
	StaffDisplay();
	~StaffDisplay() override = default;

	void paint(juce::Graphics& g) override;
	void resized() override;

	// Note drawing methods
	/** Draw a note in the treble clef
	 * @param midiNote MIDI note number (e.g., 60 = middle C, 64 = E4)
	 * @param duration Duration in 16th notes (1=16th, 2=8th, 4=quarter, 8=half, 16=whole)
	 * @param xOffset Horizontal offset in pixels from the start of the staff (after clef)
	 */
	void drawTrebleNote(int midiNote, int duration, float xOffset);

	/** Draw a note in the bass clef
	 * @param midiNote MIDI note number (e.g., 48 = C3, 43 = G2)
	 * @param duration Duration in 16th notes (1=16th, 2=8th, 4=quarter, 8=half, 16=whole)
	 * @param xOffset Horizontal offset in pixels from the start of the staff (after clef)
	 */
	void drawBassNote(int midiNote, int duration, float xOffset);

	// Set key signature (positive = sharps, negative = flats, 0 = C major)
	void setKeySignature(int accidentals);

	// Live note display (for MIDI input)
	void setLiveNote(int midiNote, bool isTreble);
	void clearLiveNote(int midiNote);
	void clearAllLiveNotes();

	// Future: methods for displaying notes, chords, etc.
	void clearStaff();

  private:
	// Layout metrics structure to hold all positioning information
	struct LayoutMetrics
	{
		float braceX = 0.0f;
		float braceWidth = 0.0f;
		float clefLeftX = 0.0f;        // Left edge of treble clef
		float clefRightX = 0.0f;       // Right edge of both clefs
		float clefWidth = 0.0f;        // Width of clef symbols
		float clefScale = 1.0f;        // Scale factor for clefs
		float staffStartX = 0.0f;      // Where staff lines begin
		juce::Path trebleClefPath;     // Pre-transformed treble clef path
		juce::Path bassClefPath;       // Pre-transformed bass clef path
	};

	// Note structure for storing notes to draw
	struct NoteInfo
	{
		int midiNote;
		int duration;
		float xOffset;
		bool isTreble; // true for treble clef, false for bass
	};

	std::vector<NoteInfo> notes;

	// Live notes being played (MIDI input)
	struct LiveNote
	{
		int midiNote;
		bool isTreble;
	};
	std::vector<LiveNote> liveNotes;
	void drawStaffLines(juce::Graphics& g, const StaffGeometry& geometry);
	void drawTrebleClef(juce::Graphics& g, const StaffGeometry& geometry);
	void drawBassClef(juce::Graphics& g, const StaffGeometry& geometry);
	void drawBrace(juce::Graphics& g, juce::Rectangle<float> bounds, const StaffGeometry& treble, const StaffGeometry& bass);
	void drawNote(juce::Graphics& g, float x, float y, NoteGlyph::Type noteType, float scale);
	void drawLedgerLine(juce::Graphics& g, float x, float y, float width);

	// Key signature drawing
	void drawKeySignature(juce::Graphics& g, const StaffGeometry& geometry, bool isTreble, float startX, float scale);

	// Layout calculation
	LayoutMetrics calculateLayout(const juce::Rectangle<float>& whiteBounds, 
								  const StaffGeometry& trebleGeometry,
								  const StaffGeometry& bassGeometry);

	// Helper methods for note rendering
	int midiToStaffPosition(int midiNote) const;
	int midiToStaffPositionBass(int midiNote) const;
	NoteGlyph::Type durationToNoteGlyph(int duration) const;
	void drawNoteLedgerLines(juce::Graphics& g, float x, int staffPosition, const StaffGeometry& geometry);
	bool noteHasAccidental(int midiNote) const;
	NoteGlyph::Type getAccidentalType(int midiNote) const;
	void drawAccidental(juce::Graphics& g, float x, float y, NoteGlyph::Type accidentalType, float scale);

	static constexpr int numStaffLines = 5;
	float staffLineSpacing{10.0f};
	float staffGap{20.0f}; // Gap between treble and bass staves

	// Music notation spacing constants (relative to staff line spacing)
	static constexpr float NOTE_HEAD_HEIGHT = 1.0f;  // Note head is 1 staff space
	static constexpr float NOTE_GLYPH_SCALE = 3.8f;  // Font size scale for proper note sizing
	static constexpr float LEDGER_LINE_WIDTH = 1.4f; // Ledger line extends beyond note head

	// Geometry objects (own all positioning math)
	std::unique_ptr<StaffGeometry> trebleGeometry;
	std::unique_ptr<StaffGeometry> bassGeometry;
	juce::Rectangle<float> currentStaffBounds;
	LayoutMetrics currentLayout; // Current layout metrics

	// Key signature: positive = sharps, negative = flats, 0 = C major/A minor
	int keySignature = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StaffDisplay)
};
