# Music Notation Spacing System

## Overview

The StaffDisplay component uses a proportional spacing system where all measurements are relative to the staff line spacing. This ensures proper scaling at any display size.

## Key Constants

### `NOTE_GLYPH_SCALE = 3.8f`
- Multiplier for font size relative to staff line spacing
- Ensures note heads are properly sized (approximately 1 staff space height)
- Calculated to match standard music engraving proportions

### `LEDGER_LINE_WIDTH = 1.4f`
- Multiplier for ledger line extension beyond note head
- Ledger lines extend this factor × staffLineSpacing on each side
- Provides proper visual balance

### `NOTE_HEAD_HEIGHT = 1.0f`
- Reference constant indicating note head spans 1 staff space
- Used for documentation and future calculations

## Staff Position System

### Position Numbering
```
Position 8 = F5 (top line)
Position 7 = E5 (space)
Position 6 = D5 (line)
Position 5 = C5 (space)
Position 4 = B4 (middle line)
Position 3 = A4 (space)
Position 2 = G4 (line)
Position 1 = F4 (space)
Position 0 = E4 (bottom line) ← Reference point
Position -1 = D4 (space below staff)
Position -2 = C4 (ledger line) ← Middle C
Position -3 = B3 (space)
...and so on
```

### Position Properties
- **Even positions** (0, 2, 4, 6, 8): Staff lines
- **Odd positions** (1, 3, 5, 7): Spaces between lines
- **Negative positions**: Below the staff (require ledger lines)
- **Positions > 8**: Above the staff (require ledger lines)

### Vertical Spacing
- Each staff position = 0.5 × staffLineSpacing
- This is because positions represent both lines AND spaces
- Total vertical distance between two positions = |pos1 - pos2| × 0.5 × staffLineSpacing

## Note Positioning

### Y-Coordinate Calculation
```cpp
float bottomLineY = trebleStaffStartY + ((numStaffLines - 1) * staffLineSpacing);
float noteY = bottomLineY - (staffPosition * staffLineSpacing * 0.5f);
```

### X-Coordinate Calculation
```cpp
float noteX = staffBounds.getX() + 50.0f + xOffset;
```
- 50.0f = space for clef
- xOffset = user-specified horizontal position

## MIDI to Staff Position Mapping

### Diatonic Mapping
The system maps MIDI notes to staff positions using the white keys (C major scale):

| Note | MIDI % 12 | White Key Position |
|------|-----------|-------------------|
| C    | 0         | 0                 |
| D    | 2         | 1                 |
| E    | 4         | 2                 |
| F    | 5         | 3                 |
| G    | 7         | 4                 |
| A    | 9         | 5                 |
| B    | 11        | 6                 |

### Sharps and Flats
- Sharps/flats are positioned at their base note (e.g., C# uses C position)
- Accidentals can be added in future updates
- This keeps positioning simple while maintaining correct vertical alignment

### Octave Calculation
```cpp
int octave = (midiNote / 12) - 1;  // MIDI octave (C4 = octave 4)
int totalPosition = octave * 7 + diatonicPosition;
int staffPosition = totalPosition - 30;  // E4 reference = 30
```

## Note Duration to Glyph Mapping

| Duration (16ths) | Note Type | SMuFL Code | Description |
|-----------------|-----------|------------|-------------|
| 16+             | Whole     | U+E0A2     | noteheadWhole |
| 8-15            | Half      | U+E1D3     | noteHalfUp |
| 4-7             | Quarter   | U+E1D5     | noteQuarterUp |
| 2-3             | Eighth    | U+E1D7     | note8thUp |
| 1               | Sixteenth | U+E1D9     | note16thUp |

## Ledger Lines

### When to Draw
- Positions < 0: Draw below staff
- Positions > 8: Draw above staff

### Drawing Rules
- Draw at even positions only (actual lines, not spaces)
- Start at first line outside staff
- Continue until reaching (or passing) the note position
- Width = `LEDGER_LINE_WIDTH × staffLineSpacing`
- Centered on note position

### Example: Middle C (Position -2)
```
Bottom line (pos 0)
Space (pos -1) - no ledger line
Ledger line (pos -2) - DRAW THIS
Space (pos -3) - no ledger line
```

## Font Sizing

### Leland Font Heights
- **Clefs**: 
  - Treble: `staffLineSpacing × 7.0f`
  - Bass: `staffLineSpacing × 5.0f`
- **Notes**: `staffLineSpacing × NOTE_GLYPH_SCALE` (3.8f)

### Glyph Positioning
SMuFL glyphs are designed with:
- **Horizontal alignment**: Glyph center = stem position
- **Vertical alignment**: Glyph center = notehead center

The rendering system centers glyphs on their target position:
```cpp
g.drawText(noteChar, 
    x - glyphWidth / 2.0f,   // Center horizontally
    y - glyphHeight / 2.0f,  // Center vertically
    glyphWidth, 
    glyphHeight, 
    juce::Justification::centred);
```

## Dynamic Scaling

The system automatically adapts to different display sizes:

1. **resized()** calculates optimal staffLineSpacing based on available height
2. All other measurements derive from staffLineSpacing
3. Minimum spacing: 8.0f pixels
4. Maximum spacing: 20.0f pixels
5. staffGap scales proportionally (2.5 × staffLineSpacing)

## Future Enhancements

Potential improvements to the spacing system:

- [ ] Add accidentals (sharps, flats, naturals) with proper spacing
- [ ] Implement stem direction rules
- [ ] Add beam grouping for 8th/16th notes
- [ ] Support for different clefs (alto, tenor)
- [ ] Time signatures and bar lines
- [ ] Key signatures
- [ ] Chord notation (multiple simultaneous notes)
- [ ] Rest symbols with proper positioning
- [ ] Dotted notes
- [ ] Tuplets (triplets, etc.)

## References

- [SMuFL Specification](https://www.smufl.org/) - Standard Music Font Layout
- [Leland Font Documentation](https://github.com/steinbergmedia/leland) - Steinberg's music font
- Music notation spacing follows traditional engraving practices

## Code Locations

- **Header**: `Source/StaffDisplay.h`
- **Implementation**: `Source/StaffDisplay.cpp`
- **Constants**: Lines 29-32 in StaffDisplay.h
- **Positioning**: `getStaffSpaceY()` method
- **MIDI Mapping**: `midiToStaffPosition()` method
- **Glyph Selection**: `durationToNoteGlyph()` method
