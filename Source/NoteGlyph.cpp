#include "NoteGlyph.h"
#include "BinaryData.h"

NoteGlyph::NoteGlyph()
{
	setupGlyphCodes();
}

NoteGlyph& NoteGlyph::getInstance()
{
	static NoteGlyph instance;
	return instance;
}

void NoteGlyph::setupGlyphCodes()
{
	// Initialize glyph map with SMuFL character codes
	// SMuFL (Standard Music Font Layout) standard codes for Leland font

	GlyphInfo wholeInfo;
	wholeInfo.glyphCode = juce::String::charToString(0xE0A2);  // noteheadWhole
	wholeInfo.centerPointY = 0.0f;
	wholeInfo.hasStem = false;
	wholeInfo.stemUp = true;  // N/A for whole notes
	glyphMap[Type::Whole] = wholeInfo;

	GlyphInfo up2Info;
	up2Info.glyphCode = juce::String::charToString(0xE1D3);  // noteHalfUp
	up2Info.centerPointY = 0.0f;
	up2Info.hasStem = true;
	up2Info.stemUp = true;
	glyphMap[Type::Up2] = up2Info;

	GlyphInfo up4Info;
	up4Info.glyphCode = juce::String::charToString(0xE1D5);  // noteQuarterUp
	up4Info.centerPointY = 0.0f;
	up4Info.hasStem = true;
	up4Info.stemUp = true;
	glyphMap[Type::Up4] = up4Info;

	GlyphInfo up8Info;
	up8Info.glyphCode = juce::String::charToString(0xE1D7);  // note8thUp
	up8Info.centerPointY = 0.0f;
	up8Info.hasStem = true;
	up8Info.stemUp = true;
	glyphMap[Type::Up8] = up8Info;

	GlyphInfo up16Info;
	up16Info.glyphCode = juce::String::charToString(0xE1D9);  // note16thUp
	up16Info.centerPointY = 0.0f;
	up16Info.hasStem = true;
	up16Info.stemUp = true;
	glyphMap[Type::Up16] = up16Info;

	GlyphInfo down2Info;
	down2Info.glyphCode = juce::String::charToString(0xE1D4);  // noteHalfDown
	down2Info.centerPointY = 0.0f;
	down2Info.hasStem = true;
	down2Info.stemUp = false;
	glyphMap[Type::Down2] = down2Info;

	GlyphInfo down4Info;
	down4Info.glyphCode = juce::String::charToString(0xE1D6);  // noteQuarterDown
	down4Info.centerPointY = 0.0f;
	down4Info.hasStem = true;
	down4Info.stemUp = false;
	glyphMap[Type::Down4] = down4Info;

	GlyphInfo down8Info;
	down8Info.glyphCode = juce::String::charToString(0xE1D8);  // note8thDown
	down8Info.centerPointY = 0.0f;
	down8Info.hasStem = true;
	down8Info.stemUp = false;
	glyphMap[Type::Down8] = down8Info;

	GlyphInfo down16Info;
	down16Info.glyphCode = juce::String::charToString(0xE1DA);  // note16thDown
	down16Info.centerPointY = 0.0f;
	down16Info.hasStem = true;
	down16Info.stemUp = false;
	glyphMap[Type::Down16] = down16Info;

	GlyphInfo sharpInfo;
	sharpInfo.glyphCode = juce::String::charToString(0xE262);  // accidentalSharp
	sharpInfo.centerPointY = 0.0f;
	sharpInfo.hasStem = false;
	sharpInfo.stemUp = false;
	glyphMap[Type::Sharp] = sharpInfo;

	GlyphInfo flatInfo;
	flatInfo.glyphCode = juce::String::charToString(0xE260);  // accidentalFlat
	flatInfo.centerPointY = 0.0f;
	flatInfo.hasStem = false;
	flatInfo.stemUp = false;
	glyphMap[Type::Flat] = flatInfo;

	measureGlyphs();
}

void NoteGlyph::measureGlyphs()
{
	float staffLineSpacing = 20.0f; // Default value if not provided
	auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Leland_otf, BinaryData::Leland_otfSize);

	// Use a base font size for measurement
	float baseFontSize = staffLineSpacing * 4.0f;
	juce::Font measureFont = juce::Font(juce::FontOptions(typeface).withHeight(baseFontSize));

	// Step 1: Measure the whole note to get the reference note head height and width
	auto& wholeInfo = glyphMap[Type::Whole];
	juce::GlyphArrangement wholeGlyphs;
	wholeGlyphs.addLineOfText(measureFont, wholeInfo.glyphCode, 0, 0);

	if (wholeGlyphs.getNumGlyphs() > 0)
	{
		wholeGlyphs.createPath(wholeInfo.glyphPath);
		wholeInfo.bounds = wholeInfo.glyphPath.getBounds();

		// Store the whole note head dimensions
		wholeNoteHeight = wholeInfo.bounds.getHeight();
		wholeNoteWidth = wholeInfo.bounds.getWidth();

		// For whole notes, the center points are at the glyph center
		wholeInfo.centerPointX = wholeInfo.bounds.getCentreX();
		wholeInfo.centerPointY = wholeInfo.bounds.getCentreY();
	}

	// Step 2: Measure all other glyphs and calculate their center points
	for (auto& [type, info] : glyphMap)
	{
		if (type == Type::Whole)
			continue;  // Already handled

		juce::GlyphArrangement glyphs;
		glyphs.addLineOfText(measureFont, info.glyphCode, 0, 0);

		if (glyphs.getNumGlyphs() > 0)
		{
			glyphs.createPath(info.glyphPath);
			info.bounds = info.glyphPath.getBounds();

			// Calculate horizontal center point based on whole note width
			// All notes should align horizontally by their note head center
			info.centerPointX = info.bounds.getX() + (wholeNoteWidth / 2.0f);

			// For accidentals (Sharp, Flat), use center like whole notes
			if (type == Type::Sharp || type == Type::Flat)
			{
				info.centerPointY = info.bounds.getCentreY();
			}
			else if (info.stemUp)
			{
				// Up-stem notes: note head is at the bottom
				// Center point = bottom - (whole note head height / 2)
				info.centerPointY = info.bounds.getBottom() - (wholeNoteHeight / 2.0f);
			}
			else
			{
				// Down-stem notes: note head is at the top
				// Center point = top + (whole note head height / 2)
				info.centerPointY = info.bounds.getY() + (wholeNoteHeight / 2.0f);
			}
		}
	}
}

float NoteGlyph::getScaleFactor(float targetStaffLineSpacing) const
{
	// Calculate the scale factor needed to make the whole note head
	// exactly one staff space tall (targetStaffLineSpacing)
	if (wholeNoteHeight > 0.0f)
	{
		return targetStaffLineSpacing / wholeNoteHeight;
	}
	return 1.0f;
}

const NoteGlyph::GlyphInfo& NoteGlyph::getGlyphInfo(Type type) const
{
	static GlyphInfo dummy;
	auto it = glyphMap.find(type);
	return (it != glyphMap.end()) ? it->second : dummy;
}

juce::String NoteGlyph::getGlyphCode(Type type) const
{
	return getGlyphInfo(type).glyphCode;
}
