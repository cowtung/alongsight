# NoteGlyph Class - Architecture Documentation

## Overview
The `NoteGlyph` class is a singleton that encapsulates all music note glyph information and pre-calculates center points for accurate positioning.

## Design Rationale

### Problem Solved
Previously, note positioning logic was scattered throughout `StaffDisplay`, with manual calculations for determining where note heads are located within each glyph. This led to:
- Repeated calculations for the same glyphs
- Complex conditional logic for different note types
- Difficult to maintain and extend

### Solution
`NoteGlyph` centralizes all glyph information and performs measurements once during initialization, storing:
- SMuFL character codes
- Pre-calculated center points (Y position of note head center)
- Stem information (presence and direction)

## Class Structure

### Enum: `NoteGlyph::Type`
```cpp
enum class Type {
    Whole,   // Whole note (no stem)
    Up2,     // Half note, stem up
    Up4,     // Quarter note, stem up
    Up8,     // Eighth note, stem up
    Up16,    // Sixteenth note, stem up
    Down2,   // Half note, stem down
    Down4,   // Quarter note, stem down
    Down8,   // Eighth note, stem down
    Down16   // Sixteenth note, stem down
};
```

### Struct: `GlyphInfo`
```cpp
struct GlyphInfo {
    juce::String glyphCode;  // SMuFL Unicode character
    float centerPointY;      // Y position of note head center
    bool hasStem;            // Whether note has a stem
    bool stemUp;             // Stem direction
};
```

## Initialization Process

### Step 1: Measure Whole Note
The whole note (noteheadWhole, U+E0A2) is measured first because:
- It has no stem, just the note head
- Its height defines "one staff space"
- It provides the reference measurement for all other notes

### Step 2: Calculate Center Points

**Whole Notes:**
```cpp
centerPointY = glyphBounds.getCentreY();
```
The center point is simply the center of the glyph.

**Up-Stem Notes:**
```cpp
centerPointY = glyphBounds.getBottom() - (wholeNoteHeight / 2.0f);
```
- Note head is at the bottom of the glyph
- Center point is half a note head height up from the bottom

**Down-Stem Notes:**
```cpp
centerPointY = glyphBounds.getY() + (wholeNoteHeight / 2.0f);
```
- Note head is at the top of the glyph
- Center point is half a note head height down from the top

## SMuFL Glyph Codes

| Type   | SMuFL Code | Unicode | Description        |
|--------|------------|---------|-------------------|
| Whole  | 0xE0A2     | U+E0A2  | noteheadWhole     |
| Up2    | 0xE1D3     | U+E1D3  | noteHalfUp        |
| Up4    | 0xE1D5     | U+E1D5  | noteQuarterUp     |
| Up8    | 0xE1D7     | U+E1D7  | note8thUp         |
| Up16   | 0xE1D9     | U+E1D9  | note16thUp        |
| Down2  | 0xE1D4     | U+E1D4  | noteHalfDown      |
| Down4  | 0xE1D6     | U+E1D6  | noteQuarterDown   |
| Down8  | 0xE1D8     | U+E1D8  | note8thDown       |
| Down16 | 0xE1DA     | U+E1DA  | note16thDown      |

## Usage in StaffDisplay

### Initialization
```cpp
auto& noteGlyph = NoteGlyph::getInstance();
if (!noteGlyph.isInitialized())
{
    noteGlyph.initialize(staffLineSpacing, typeface);
}
```

### Getting Glyph Information
```cpp
NoteGlyph::Type noteType = durationToNoteGlyph(duration);
const auto& glyphInfo = noteGlyph.getGlyphInfo(noteType);

// Use pre-calculated center point
float noteCenterY = glyphInfo.centerPointY;
```

### Drawing Notes
```cpp
void StaffDisplay::drawNote(Graphics& g, float x, float y, 
                           NoteGlyph::Type noteType, float scale)
{
    auto& glyphInfo = NoteGlyph::getInstance().getGlyphInfo(noteType);
    
    // Use the pre-calculated center point for positioning
    float centeredY = y - (glyphInfo.centerPointY * scale);
    
    // Draw the note...
}
```

## Benefits

### 1. Performance
- Glyphs measured once, not per-frame
- No repeated glyph-to-path conversions during rendering
- Pre-calculated center points avoid runtime calculations

### 2. Maintainability
- All glyph information in one place
- Easy to add new note types (e.g., down-stem notes)
- Clear separation of concerns

### 3. Accuracy
- Center points calculated from actual glyph measurements
- Consistent positioning across all note types
- Based on whole note head height (one staff space)

### 4. Extensibility
- Easy to add new note types
- Simple to support different fonts in the future
- Can extend with additional glyph properties (color, style, etc.)

## Singleton Pattern Rationale

The singleton pattern is appropriate here because:
- **Single Source of Truth**: Only one set of glyph measurements should exist
- **Shared Resource**: All `StaffDisplay` instances use the same glyphs
- **Lazy Initialization**: Only initialized when first used
- **Memory Efficiency**: Measurements stored once, not per instance

## Future Enhancements

### 1. Support Down-Stem Notes
Currently implemented but not used. To use:
```cpp
// For notes on or above the middle line, use down stems
if (staffPosition >= 4) {
    return NoteGlyph::Type::Down4;  // Down-stem quarter
} else {
    return NoteGlyph::Type::Up4;    // Up-stem quarter
}
```

### 2. Dotted Notes
Add types: `WholeD`, `Up2D`, `Up4D`, etc. for dotted notes

### 3. Multiple Fonts
Extend to support different music fonts (Bravura, Petaluma, etc.):
```cpp
void initialize(float staffLineSpacing, 
                const juce::Typeface::Ptr& typeface,
                FontType fontType = FontType::Leland);
```

### 4. Note Head Alternatives
Add support for:
- Cross noteheads (for percussion)
- Diamond noteheads (for harmonics)
- Slash noteheads (for rhythm notation)

## Related Files
- `NoteGlyph.h` - Class declaration
- `NoteGlyph.cpp` - Implementation
- `StaffDisplay.h/cpp` - Main consumer
- `docs/NOTATION_SPACING_SYSTEM.md` - Spacing documentation

## References
- [SMuFL Specification](https://www.smufl.org/)
- [Leland Font Documentation](https://github.com/steinbergmedia/leland)
