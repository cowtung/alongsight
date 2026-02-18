#include "StaffDisplay.h"
#include "BinaryData.h"
#include "NoteGlyph.h"

//==============================================================================
// StaffDisplay - Music Notation Rendering System
//
// SPACING SYSTEM:
// - Staff line spacing is the fundamental unit of measurement
// - All note sizes, positions, and spacing are calculated relative to this value
// - Note glyphs are sized using NOTE_GLYPH_SCALE * staffLineSpacing
// - Ledger lines extend LEDGER_LINE_WIDTH * staffLineSpacing on each side
//
// POSITIONING SYSTEM:
// - Staff positions are integer values representing lines and spaces
// - Position 0 = bottom line of treble staff (E4, MIDI 64)
// - Even positions (0,2,4,6,8) = staff lines
// - Odd positions (1,3,5,7) = spaces between lines
// - Negative positions = ledger lines below staff
// - Positions > 8 = ledger lines above staff
// - Each position = 0.5 * staffLineSpacing vertical distance
//
// MIDI TO STAFF MAPPING:
// - Uses diatonic (white key) positions for natural notes
// - Sharps/flats are positioned at their nearest natural note
// - Mapping based on C major scale for simplicity
//
//==============================================================================

StaffDisplay::StaffDisplay() {}

void StaffDisplay::paint(juce::Graphics& g)
{
	auto bounds = getLocalBounds().toFloat().reduced(20.0f);

	// Draw background
	g.setColour(juce::Colours::white);
	g.fillRoundedRectangle(bounds, 5.0f);

	// Draw border
	g.setColour(juce::Colours::black);
	g.drawRoundedRectangle(bounds, 5.0f, 2.0f);

	// Draw grand staff (treble and bass)
	auto staffBounds = bounds.reduced(40.0f, 30.0f);
	currentStaffBounds = staffBounds;

	// Calculate positions for both staves
	float totalStaffHeight = staffLineSpacing * (numStaffLines - 1);
	float totalHeight = (totalStaffHeight * 2) + staffGap;
	float trebleTop = staffBounds.getCentreY() - totalHeight / 2.0f;
	float bassTop = trebleTop + totalStaffHeight + staffGap;

	// Create geometry objects for precise positioning
	// Note: We'll temporarily use staffBounds.getX() and update after layout calculation
	auto trebleBounds = juce::Rectangle<float>(staffBounds.getX(), trebleTop, staffBounds.getWidth(), totalStaffHeight);
	auto bassBounds = juce::Rectangle<float>(staffBounds.getX(), bassTop, staffBounds.getWidth(), totalStaffHeight);

	// Create temporary geometry objects to calculate layout
	StaffGeometry tempTrebleGeometry(trebleBounds, staffLineSpacing);
	StaffGeometry tempBassGeometry(bassBounds, staffLineSpacing);

	// Calculate layout metrics (brace, clef positions, staff start)
	currentLayout = calculateLayout(bounds, tempTrebleGeometry, tempBassGeometry);

	// Now create final geometry objects with correct staff start position
	trebleBounds.setX(currentLayout.staffStartX);
	bassBounds.setX(currentLayout.staffStartX);
	trebleGeometry = std::make_unique<StaffGeometry>(trebleBounds, staffLineSpacing);
	bassGeometry = std::make_unique<StaffGeometry>(bassBounds, staffLineSpacing);

	// Draw brace connecting the staves (uses currentLayout.braceX)
	drawBrace(g, staffBounds, *trebleGeometry, *bassGeometry);

	// Draw vertical barline 4 pixels to the right of the brace
	float barlineX = currentLayout.braceX + currentLayout.braceWidth + 4.0f;
	float barlineTop = trebleGeometry->getStaffTop();
	float barlineBottom = bassGeometry->getStaffBottom();
	g.setColour(juce::Colours::black);
	g.drawLine(barlineX, barlineTop, barlineX, barlineBottom, 2.0f);

	// Load Leland music font for notes
	auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Leland_otf, BinaryData::Leland_otfSize);

	// Get NoteGlyph singleton
	auto& noteGlyph = NoteGlyph::getInstance();

	// Get the scale factor from NoteGlyph
	// This ensures all note heads are the same size (one staff space)
	float noteScale = noteGlyph.getScaleFactor(staffLineSpacing);

	// Draw treble staff and clef (uses currentLayout for positioning)
	drawStaffLines(g, *trebleGeometry);
	drawTrebleClef(g, *trebleGeometry);

	// Draw key signature after clef
	if (keySignature != 0)
	{
		float keySignatureX = currentLayout.clefRightX + 30.0f; // 5 pixels after clef
		drawKeySignature(g, *trebleGeometry, true, keySignatureX, noteScale);
	}

	// Draw bass staff and clef
	drawStaffLines(g, *bassGeometry);
	drawBassClef(g, *bassGeometry);

	// Draw key signature after clef
	if (keySignature != 0)
	{
		float keySignatureX = currentLayout.clefRightX + 30.0f;
		drawKeySignature(g, *bassGeometry, false, keySignatureX, noteScale);
	}

	// Draw all stored notes
	for (const auto& note : notes)
	{
		if (note.isTreble && trebleGeometry)
		{
			// Calculate staff position from MIDI note
			int staffPosition = midiToStaffPosition(note.midiNote);

			// Get Y position from geometry - this ensures perfect alignment
			float noteY = trebleGeometry->getPositionY(staffPosition);

			// Calculate X position
			float noteX = trebleGeometry->getStaffLeft() + note.xOffset;

			// Draw ledger lines if needed
			drawNoteLedgerLines(g, noteX, staffPosition, *trebleGeometry);

			// Get the appropriate note type
			NoteGlyph::Type noteType = durationToNoteGlyph(note.duration);

			// Draw the note using the pre-calculated scale
			drawNote(g, noteX, noteY, noteType, noteScale);

			// Draw accidental (sharp/flat) if needed, to the left of the note
			if (noteHasAccidental(note.midiNote))
			{
				NoteGlyph::Type accidentalType = getAccidentalType(note.midiNote);
				// Position accidental to the left of the note
				// Accidentals are typically placed about 1 staff space to the left
				float accidentalX = noteX - (staffLineSpacing * 0.9f);
				drawAccidental(g, accidentalX, noteY, accidentalType, noteScale);
			}
		}
		else if (!note.isTreble && bassGeometry)
		{
			// Calculate staff position from MIDI note for bass clef
			int staffPosition = midiToStaffPositionBass(note.midiNote);

			// Get Y position from geometry - this ensures perfect alignment
			float noteY = bassGeometry->getPositionY(staffPosition);

			// Calculate X position
			float noteX = bassGeometry->getStaffLeft() + note.xOffset;

			// Draw ledger lines if needed
			drawNoteLedgerLines(g, noteX, staffPosition, *bassGeometry);

			// Get the appropriate note type
			NoteGlyph::Type noteType = durationToNoteGlyph(note.duration);

			// Draw the note using the pre-calculated scale
			drawNote(g, noteX, noteY, noteType, noteScale);

			// Draw accidental (sharp/flat) if needed, to the left of the note
			if (noteHasAccidental(note.midiNote))
			{
				NoteGlyph::Type accidentalType = getAccidentalType(note.midiNote);
				// Position accidental to the left of the note
				float accidentalX = noteX - (staffLineSpacing * 0.9f);
				drawAccidental(g, accidentalX, noteY, accidentalType, noteScale);
			}
		}
	}

	// Draw live notes (MIDI input) at center of staff
	float centerX = (trebleGeometry->getStaffLeft() + trebleGeometry->getStaffRight()) / 2.0f;

	for (const auto& liveNote : liveNotes)
	{
		if (liveNote.isTreble && trebleGeometry)
		{
			int staffPosition = midiToStaffPosition(liveNote.midiNote);
			float noteY = trebleGeometry->getPositionY(staffPosition);

			// Draw ledger lines
			drawNoteLedgerLines(g, centerX, staffPosition, *trebleGeometry);

			// Draw quarter note (always use quarter note for live display)
			drawNote(g, centerX, noteY, NoteGlyph::Type::Up4, noteScale);

			// Draw accidental if needed
			if (noteHasAccidental(liveNote.midiNote))
			{
				NoteGlyph::Type accidentalType = getAccidentalType(liveNote.midiNote);
				float accidentalX = centerX - (staffLineSpacing * 0.9f);
				drawAccidental(g, accidentalX, noteY, accidentalType, noteScale);
			}
		}
		else if (!liveNote.isTreble && bassGeometry)
		{
			int staffPosition = midiToStaffPositionBass(liveNote.midiNote);
			float noteY = bassGeometry->getPositionY(staffPosition);

			// Draw ledger lines
			drawNoteLedgerLines(g, centerX, staffPosition, *bassGeometry);

			// Draw quarter note
			drawNote(g, centerX, noteY, NoteGlyph::Type::Up4, noteScale);

			// Draw accidental if needed
			if (noteHasAccidental(liveNote.midiNote))
			{
				NoteGlyph::Type accidentalType = getAccidentalType(liveNote.midiNote);
				float accidentalX = centerX - (staffLineSpacing * 0.9f);
				drawAccidental(g, accidentalX, noteY, accidentalType, noteScale);
			}
		}
	}
}

void StaffDisplay::resized()
{
	auto bounds = getLocalBounds();
	// Calculate spacing for two staves with gap between them
	float availableHeight = bounds.getHeight() - 100.0f; // Margins
	staffLineSpacing = (availableHeight - staffGap) / ((numStaffLines - 1) * 2);
	staffLineSpacing = juce::jlimit(8.0f, 20.0f, staffLineSpacing);

	// Set gap to match treble clef height (4 staff spaces = distance between top and bottom lines)
	staffGap = staffLineSpacing * 4.0f;
}

void StaffDisplay::clearStaff()
{
	notes.clear();
	repaint();
}

void StaffDisplay::drawStaffLines(juce::Graphics& g, const StaffGeometry& geometry)
{
	g.setColour(juce::Colours::black);

	// Get all line positions from geometry - guaranteed to be in sync with note positions
	auto linePositions = geometry.getAllLineYPositions();

	// Draw the 5 staff lines
	for (int i = 0; i < 5; ++i)
	{
		g.drawLine(geometry.getStaffLeft(), linePositions[i], 
				   geometry.getStaffRight(), linePositions[i], 1.5f);
	}
}

void StaffDisplay::drawTrebleClef(juce::Graphics& g, const StaffGeometry& geometry)
{
	g.setColour(juce::Colours::black);
	// Use pre-transformed path from layout
	g.fillPath(currentLayout.trebleClefPath);
}

void StaffDisplay::drawBassClef(juce::Graphics& g, const StaffGeometry& geometry)
{
	g.setColour(juce::Colours::black);
	// Use pre-transformed path from layout
	g.fillPath(currentLayout.bassClefPath);
}

void StaffDisplay::drawBrace(juce::Graphics& g, juce::Rectangle<float> bounds, const StaffGeometry& treble, const StaffGeometry& bass)
{
	g.setColour(juce::Colours::black);

	float topY = treble.getStaffTop();
	float bottomY = bass.getStaffBottom();
	float braceHeight = bottomY - topY;

	// Load Leland music font for brace
	auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Leland_otf, BinaryData::Leland_otfSize);

	// SMuFL has multiple brace glyphs at different sizes (U+E000 - U+E00F)
	// We'll use the largest one and scale it to fit
	// U+E000 = brace (standard size)
	juce::String braceChar = juce::String::charToString(0xE000);

	// Create a path from the glyph so we can transform it
	float baseFontSize = braceHeight; // Start with height matching span
	juce::Font braceFont = juce::Font(juce::FontOptions(typeface).withHeight(baseFontSize));

	// Get glyph arrangement to extract the path
	juce::GlyphArrangement glyphs;
	glyphs.addLineOfText(braceFont, braceChar, 0, 0);

	if (glyphs.getNumGlyphs() > 0)
	{
		juce::Path bracePath;
		glyphs.createPath(bracePath);

		// Get the actual bounds of the glyph
		auto glyphBounds = bracePath.getBounds();

		if (glyphBounds.getHeight() > 0)
		{
			// Calculate the scale needed to make the brace span the full height
			float scaleY = braceHeight / glyphBounds.getHeight();
			// Keep aspect ratio
			float scaleX = scaleY;

			// Use position from currentLayout
			float braceX = currentLayout.braceX;

			// Transform the path to the correct size and position
			auto transform = juce::AffineTransform::scale(scaleX, scaleY)
											.translated(braceX - glyphBounds.getX() * scaleX, 
														topY - glyphBounds.getY() * scaleY);

			bracePath.applyTransform(transform);

			// Draw the transformed path
			g.fillPath(bracePath);
		}
	}
}

void StaffDisplay::drawNote(juce::Graphics& g, float x, float y, NoteGlyph::Type noteType, float scale)
{
	g.setColour(juce::Colours::black);

	// Get glyph info from singleton (includes pre-created path)
	auto& noteGlyph = NoteGlyph::getInstance();
	const auto& glyphInfo = noteGlyph.getGlyphInfo(noteType);

	// Make a copy of the path so we can transform it
	juce::Path notePath = glyphInfo.glyphPath;
	auto glyphBounds = glyphInfo.bounds;

	if (!glyphBounds.isEmpty())
	{
		// Use the pre-calculated center points from NoteGlyph
		float noteCenterX = glyphInfo.centerPointX;
		float noteCenterY = glyphInfo.centerPointY;

		// Position the note using its center point
		float centeredX = x - (noteCenterX * scale);
		float centeredY = y - (noteCenterY * scale);

		// Apply transformation: scale and translate to position
		auto transform = juce::AffineTransform::scale(scale)
											.translated(centeredX, centeredY);

		notePath.applyTransform(transform);

		// Draw the note as a filled path
		g.fillPath(notePath);
	}
}

void StaffDisplay::drawLedgerLine(juce::Graphics& g, float x, float y, float width)
{
	g.setColour(juce::Colours::black);
	g.drawLine(x, y, x + width, y, 1.5f);
}

void StaffDisplay::drawTrebleNote(int midiNote, int duration, float xOffset)
{
	// Add note to the list of notes to draw
	notes.push_back({midiNote, duration, xOffset, true});

	// Trigger repaint to show the note
	repaint();
}

void StaffDisplay::drawBassNote(int midiNote, int duration, float xOffset)
{
	// Add note to the list of notes to draw (isTreble = false for bass clef)
	notes.push_back({midiNote, duration, xOffset, false});

	// Trigger repaint to show the note
	repaint();
}

void StaffDisplay::setKeySignature(int accidentals)
{
	keySignature = accidentals;
	repaint();
}

void StaffDisplay::setLiveNote(int midiNote, bool isTreble)
{
	// Check if note is already in the live notes list
	for (const auto& note : liveNotes)
	{
		if (note.midiNote == midiNote && note.isTreble == isTreble)
			return; // Already displaying this note
	}

	// Add the note
	liveNotes.push_back({midiNote, isTreble});
	repaint();
}

void StaffDisplay::clearLiveNote(int midiNote)
{
	// Remove all instances of this MIDI note
	liveNotes.erase(
		std::remove_if(liveNotes.begin(), liveNotes.end(),
			[midiNote](const LiveNote& note) { return note.midiNote == midiNote; }),
		liveNotes.end());
	repaint();
}

void StaffDisplay::clearAllLiveNotes()
{
	liveNotes.clear();
	repaint();
}

void StaffDisplay::drawKeySignature(juce::Graphics& g, const StaffGeometry& geometry, bool isTreble, float startX, float scale)
{
	auto& noteGlyph = NoteGlyph::getInstance();

	// Determine if we're drawing sharps or flats
	bool isSharp = (keySignature > 0);
	int count = std::abs(keySignature);

	// Standard key signature positions for sharps and flats
	// These follow traditional music notation positioning

	// Treble clef sharp positions: F# C# G# D# A# E# B#
	// F#=top line, C#=3rd space, G#=above staff, D#=4th line, A#=2nd space, E#=4th space, B#=middle line
	static const int trebleSharpPositions[] = {8, 5, 9, 6, 3, 7, 4};

	// Bass clef sharp positions: F# C# G# D# A# E# B#
	// F#=4th line (2nd from top), C#=2nd space, G#=4th space, D#=middle line, A#=1st space, E#=3rd space, B#=2nd line
	static const int bassSharpPositions[] = {6, 3, 7, 4, 1, 5, 2};

	// Treble clef flat positions: Bb Eb Ab Db Gb Cb Fb
	// Bb=middle line, Eb=4th space, Ab=2nd space, Db=4th line, Gb=2nd line, Cb=3rd space, Fb=1st space
	static const int trebleFlatPositions[] = {4, 7, 3, 6, 2, 5, 1};

	// Bass clef flat positions: Bb Eb Ab Db Gb Cb Fb
	// Bb=2nd line, Eb=3rd space, Ab=1st space, Db=middle line, Gb=bottom line, Cb=2nd space, Fb=4th line
	static const int bassFlatPositions[] = {2, 5, 1, 4, 0, 3, 6};

	const int* positions = nullptr;
	if (isTreble)
		positions = isSharp ? trebleSharpPositions : trebleFlatPositions;
	else
		positions = isSharp ? bassSharpPositions : bassFlatPositions;

	NoteGlyph::Type accidentalType = isSharp ? NoteGlyph::Type::Sharp : NoteGlyph::Type::Flat;

	// Draw each accidental with spacing
	float accidentalSpacing = staffLineSpacing * 0.6f; // Space between accidentals
	float currentX = startX;

	for (int i = 0; i < count && i < 7; ++i)
	{
		int staffPosition = positions[i];
		float noteY = geometry.getPositionY(staffPosition);

		drawAccidental(g, currentX, noteY, accidentalType, scale);
		currentX += accidentalSpacing;
	}
}

int StaffDisplay::midiToStaffPosition(int midiNote) const
{
	// Map MIDI notes to treble staff positions
	// 
	// STAFF POSITION REFERENCE (Treble Clef):
	// Position 0 = E4 (bottom line, MIDI 64)
	// Position 1 = F4 (first space)
	// Position 2 = G4 (second line)
	// Position 3 = A4 (second space)
	// Position 4 = B4 (middle line)
	// Position 5 = C5 (third space)
	// Position 6 = D5 (fourth line)
	// Position 7 = E5 (fourth space)
	// Position 8 = F5 (top line)
	//
	// Each staff position = one diatonic step in C major scale
	// Sharps/flats use the position of their nearest natural note

	// Note positions in an octave (white keys only, relative to C)
	// C=0, D=1, E=2, F=3, G=4, A=5, B=6
	static const int whiteKeyPositions[12] = {
		0,  // C
		0,  // C# (use C position)
		1,  // D
		1,  // D# (use D position)
		2,  // E
		3,  // F
		3,  // F# (use F position)
		4,  // G
		4,  // G# (use G position)
		5,  // A
		5,  // A# (use A position)
		6   // B
	};

	// Calculate octave and note within octave
	int octave = (midiNote / 12) - 1; // MIDI octave (C4 = octave 4)
	int noteInOctave = midiNote % 12;
	int diatonicPosition = whiteKeyPositions[noteInOctave];

	// Total position in white key scale (7 notes per octave)
	int totalPosition = octave * 7 + diatonicPosition;

	// E4 (MIDI 64) is the reference point (bottom line = position 0)
	// E4 = octave 4, note E = position 2, so E4 = 4*7 + 2 = 30
	// Staff position = totalPosition - 30
	return totalPosition - 30;
}

int StaffDisplay::midiToStaffPositionBass(int midiNote) const
{
	// Map MIDI notes to bass staff positions
	// 
	// STAFF POSITION REFERENCE (Bass Clef):
	// Position 0 = G2 (bottom line, MIDI 43)
	// Position 1 = A2 (first space)
	// Position 2 = B2 (second line)
	// Position 3 = C3 (second space)
	// Position 4 = D3 (middle line)
	// Position 5 = E3 (third space)
	// Position 6 = F3 (fourth line)
	// Position 7 = G3 (fourth space)
	// Position 8 = A3 (top line)
	//
	// Each staff position = one diatonic step in C major scale
	// Sharps/flats use the position of their nearest natural note

	// Note positions in an octave (white keys only, relative to C)
	// C=0, D=1, E=2, F=3, G=4, A=5, B=6
	static const int whiteKeyPositions[12] = {
		0,  // C
		0,  // C# (use C position)
		1,  // D
		1,  // D# (use D position)
		2,  // E
		3,  // F
		3,  // F# (use F position)
		4,  // G
		4,  // G# (use G position)
		5,  // A
		5,  // A# (use A position)
		6   // B
	};

	// Calculate octave and note within octave
	int octave = (midiNote / 12) - 1; // MIDI octave (C4 = octave 4)
	int noteInOctave = midiNote % 12;
	int diatonicPosition = whiteKeyPositions[noteInOctave];

	// Total position in white key scale (7 notes per octave)
	int totalPosition = octave * 7 + diatonicPosition;

	// G2 (MIDI 43) is the reference point (bottom line = position 0)
	// G2 = octave 2, note G = position 4, so G2 = 2*7 + 4 = 18
	// Staff position = totalPosition - 18
	return totalPosition - 18;
}

NoteGlyph::Type StaffDisplay::durationToNoteGlyph(int duration) const
{
	// Map duration in 16ths to NoteGlyph types
	if (duration >= 16)
	{
		// Whole note
		return NoteGlyph::Type::Whole;
	}
	else if (duration >= 8)
	{
		// Half note
		return NoteGlyph::Type::Up2;
	}
	else if (duration >= 4)
	{
		// Quarter note  
		return NoteGlyph::Type::Up4;
	}
	else if (duration >= 2)
	{
		// Eighth note
		return NoteGlyph::Type::Up8;
	}
	else
	{
		// Sixteenth note
		return NoteGlyph::Type::Up16;
	}
}

void StaffDisplay::drawNoteLedgerLines(juce::Graphics& g, float x, int staffPosition, const StaffGeometry& geometry)
{
	// Draw ledger lines for notes outside the staff
	// Use geometry to determine if ledger lines are needed and where to draw them

	float ledgerWidth = staffLineSpacing * LEDGER_LINE_WIDTH;
	float ledgerX = x - ledgerWidth / 2.0f;

	// Ledger lines below the staff
	if (geometry.needsLedgerLinesBelow(staffPosition))
	{
		// Draw ledger lines at even positions (lines, not spaces)
		for (int pos = -2; pos >= staffPosition; pos -= 2)
		{
			float y = geometry.getPositionY(pos);
			drawLedgerLine(g, ledgerX, y, ledgerWidth);
		}
	}
	// Ledger lines above the staff
	else if (geometry.needsLedgerLinesAbove(staffPosition))
	{
		// Draw ledger lines at even positions
		for (int pos = 10; pos <= staffPosition; pos += 2)
		{
			float y = geometry.getPositionY(pos);
			drawLedgerLine(g, ledgerX, y, ledgerWidth);
		}
	}
}

bool StaffDisplay::noteHasAccidental(int midiNote) const
{
	// Check if the MIDI note is a sharp or flat (black key on piano)
	int noteInOctave = midiNote % 12;

	// Black keys: C#(1), D#(3), F#(6), G#(8), A#(10)
	return (noteInOctave == 1 || noteInOctave == 3 || noteInOctave == 6 || 
			noteInOctave == 8 || noteInOctave == 10);
}

NoteGlyph::Type StaffDisplay::getAccidentalType(int midiNote) const
{
	// For now, we'll use sharps for all black keys
	// In a real implementation, you'd use key signatures to determine sharp vs flat
	int noteInOctave = midiNote % 12;

	if (noteInOctave == 1 || noteInOctave == 3 || noteInOctave == 6 || 
		noteInOctave == 8 || noteInOctave == 10)
	{
		return NoteGlyph::Type::Sharp;
	}

	return NoteGlyph::Type::Sharp; // Default (shouldn't reach here)
}

void StaffDisplay::drawAccidental(juce::Graphics& g, float x, float y, NoteGlyph::Type accidentalType, float scale)
{
	g.setColour(juce::Colours::black);

	// Get glyph info from singleton (includes pre-created path)
	auto& noteGlyph = NoteGlyph::getInstance();
	const auto& glyphInfo = noteGlyph.getGlyphInfo(accidentalType);

	// Make a copy of the path so we can transform it
	juce::Path accidentalPath = glyphInfo.glyphPath;
	auto glyphBounds = glyphInfo.bounds;

	if (!glyphBounds.isEmpty())
	{
		// Use the pre-calculated center points from NoteGlyph
		float centerX = glyphInfo.centerPointX;
		float centerY = glyphInfo.centerPointY;

		// Position the accidental centered on the staff line/space
		float centeredX = x - (centerX * scale);
		float centeredY = y - (centerY * scale);

		// Apply transformation: scale and translate to position
		auto transform = juce::AffineTransform::scale(scale * 0.7).translated(centeredX, centeredY);

		accidentalPath.applyTransform(transform);

		// Draw the accidental as a filled path
		g.fillPath(accidentalPath);
	}
}

StaffDisplay::LayoutMetrics StaffDisplay::calculateLayout(
	const juce::Rectangle<float>& whiteBounds,
	const StaffGeometry& trebleGeometry,
	const StaffGeometry& bassGeometry)
{
	LayoutMetrics layout;
	auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Leland_otf, BinaryData::Leland_otfSize);

	// Calculate brace position and width
	layout.braceX = whiteBounds.getX() + 2.0f;

	float braceTopY = trebleGeometry.getStaffTop();
	float braceBottomY = bassGeometry.getStaffBottom();
	float braceHeight = braceBottomY - braceTopY;

	// Measure brace to get its width
	juce::String braceChar = juce::String::charToString(0xE000);
	float braceFontSize = braceHeight;
	juce::Font braceFont = juce::Font(juce::FontOptions(typeface).withHeight(braceFontSize));
	juce::GlyphArrangement braceGlyphs;
	braceGlyphs.addLineOfText(braceFont, braceChar, 0, 0);

	if (braceGlyphs.getNumGlyphs() > 0)
	{
		juce::Path bracePath;
		braceGlyphs.createPath(bracePath);
		auto braceBounds = bracePath.getBounds();
		float braceScale = braceHeight / braceBounds.getHeight();
		layout.braceWidth = braceBounds.getWidth() * braceScale;
	}

	// Calculate clef positioning
	// Treble clef left edge is 10 pixels from brace right edge
	layout.clefLeftX = layout.braceX + layout.braceWidth + 10.0f;

	// Calculate clef dimensions
	// Target height: distance between top and bottom lines of staff (4 staff spaces)
	float targetClefHeight = staffLineSpacing * 4.0f;

	// Measure treble clef to get its width
	juce::String clefChar = juce::String::charToString(0xE050);
	float clefFontSize = staffLineSpacing * 8.0f;
	juce::Font clefFont = juce::Font(juce::FontOptions(typeface).withHeight(clefFontSize));
	juce::GlyphArrangement clefGlyphs;
	clefGlyphs.addLineOfText(clefFont, clefChar, 0, 0);

	if (clefGlyphs.getNumGlyphs() > 0)
	{
		juce::Path clefPath;
		clefGlyphs.createPath(clefPath);
		auto clefBounds = clefPath.getBounds();

		layout.clefScale = targetClefHeight / clefBounds.getHeight();
		layout.clefWidth = clefBounds.getWidth() * layout.clefScale;

		// Pre-transform treble clef path
		// Position the clef so its left edge is at clefLeftX
		float centerLineY = trebleGeometry.getLineY(2);

		// Translate so the left edge of the glyph is at clefLeftX
		// and the center is vertically aligned with the middle staff line
		float positionedX = layout.clefLeftX - (clefBounds.getX() * layout.clefScale);
		float centeredY = centerLineY - (clefBounds.getCentreY() * layout.clefScale);

		auto trebleTransform = juce::AffineTransform::scale(layout.clefScale)
											.translated(positionedX, centeredY);
		layout.trebleClefPath = clefPath;
		layout.trebleClefPath.applyTransform(trebleTransform);

		// NOW get the actual right edge of the transformed treble clef
		auto transformedTrebleBounds = layout.trebleClefPath.getBounds();
		layout.clefRightX = transformedTrebleBounds.getRight();

		// Pre-transform bass clef path (same scale, aligned right edge)
		juce::String bassClefChar = juce::String::charToString(0xE062);
		juce::GlyphArrangement bassClefGlyphs;
		bassClefGlyphs.addLineOfText(clefFont, bassClefChar, 0, 0);

		if (bassClefGlyphs.getNumGlyphs() > 0)
		{
			juce::Path bassClefPath;
			bassClefGlyphs.createPath(bassClefPath);
			auto bassClefBounds = bassClefPath.getBounds();

			// Position bass clef so its right edge aligns with treble clef's right edge
			float bassCenterLineY = bassGeometry.getLineY(2);

			// Calculate where the bass clef's left edge should be
			// so its right edge aligns with layout.clefRightX
			float bassClefWidth = bassClefBounds.getWidth() * layout.clefScale;
			float bassClefLeftX = layout.clefRightX - bassClefWidth;

			float bassPositionedX = bassClefLeftX - (bassClefBounds.getX() * layout.clefScale);
			float bassCenteredY = bassCenterLineY - (bassClefBounds.getCentreY() * layout.clefScale);

			auto bassTransform = juce::AffineTransform::scale(layout.clefScale)
												.translated(bassPositionedX, bassCenteredY);
			layout.bassClefPath = bassClefPath;
			layout.bassClefPath.applyTransform(bassTransform);
		}
	}

	// Staff lines start at the vertical barline position (after brace + 4px + small gap)
	layout.staffStartX = layout.braceX + layout.braceWidth + 4.0f - 1.0f; // 4px for barline position minus 1px to align with barline center

	return layout;
}
