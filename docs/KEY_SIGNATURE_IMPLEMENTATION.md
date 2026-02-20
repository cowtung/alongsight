# Key Signature Implementation

## Overview
Added a key signature selector dropdown that allows users to choose different musical keys, which are then displayed on both the treble and bass clefs of the grand staff.

## Features

### 1. Key Signature Selector
- **Location**: Top left corner of the application
- **Type**: ComboBox with label "Key:"
- **Options**: 13 major keys from C Major to Gb Major

### 2. Supported Key Signatures

#### Sharp Keys (Positive Values)
- C Major (0): No accidentals
- G Major (1): F#
- D Major (2): F#, C#
- A Major (3): F#, C#, G#
- E Major (4): F#, C#, G#, D#
- B Major (5): F#, C#, G#, D#, A#
- F# Major (6): F#, C#, G#, D#, A#, E#

#### Flat Keys (Negative Values)
- F Major (-1): Bb
- Bb Major (-2): Bb, Eb
- Eb Major (-3): Bb, Eb, Ab
- Ab Major (-4): Bb, Eb, Ab, Db
- Db Major (-5): Bb, Eb, Ab, Db, Gb
- Gb Major (-6): Bb, Eb, Ab, Db, Gb, Cb

## Implementation Details

### MainComponent Changes

#### UI Elements Added
```cpp
juce::ComboBox keySignatureSelector;
juce::Label keySignatureLabel;
```

#### Layout
- Positioned in top left corner above the title
- Takes up 250 pixels width (40px label + 210px ComboBox)
- 30 pixels height

#### Callback Handler
```cpp
void MainComponent::keySignatureChanged()
{
    // Maps ComboBox ID to accidental count
    // Positive = sharps, Negative = flats
    staffDisplay.setKeySignature(accidentals);
}
```

### StaffDisplay Changes

#### New Members
```cpp
int keySignature = 0;  // Positive = sharps, negative = flats
```

#### New Methods

**`setKeySignature(int accidentals)`**
- Sets the key signature
- Triggers repaint to show changes
- Positive values = sharps, negative = flats, 0 = C major

**`drawKeySignature(Graphics& g, const StaffGeometry& geometry, bool isTreble, float startX, float scale)`**
- Draws sharps or flats at proper staff positions
- Uses standard music notation positioning
- Positions accidentals 5 pixels after clef
- Spaces accidentals 0.8 * staffLineSpacing apart

### Staff Position Mappings

#### Treble Clef
**Sharps** (in order of appearance):
- F# (position 5)
- C# (position 3)
- G# (position 8)
- D# (position 6)
- A# (position 4)
- E# (position 2)
- B# (position 7)

**Flats** (in order of appearance):
- Bb (position 4)
- Eb (position 6)
- Ab (position 3)
- Db (position 5)
- Gb (position 2)
- Cb (position 4)
- Fb (position 1)

#### Bass Clef
**Sharps** (in order of appearance):
- F# (position 4)
- C# (position 2)
- G# (position 7)
- D# (position 5)
- A# (position 3)
- E# (position 1)
- B# (position 6)

**Flats** (in order of appearance):
- Bb (position 3)
- Eb (position 5)
- Ab (position 2)
- Db (position 4)
- Gb (position 1)
- Cb (position 3)
- Fb (position 0)

## Positioning

```
[Brace] --10px-- [Clef] --5px-- [Key Signature] --10px-- [Staff Lines...]
```

### Key Spacing:
- **After Clef**: 5 pixels from right edge of clef
- **Between Accidentals**: 0.8 * staffLineSpacing
- **Before Staff Lines**: Staff lines already positioned 10px from clef right edge

## Visual Representation

The key signature appears in standard notation:
- Immediately after the clef symbol
- Before the first note
- Uses the same Sharp and Flat glyphs from NoteGlyph
- Scaled to match note sizes
- Positioned at traditional staff positions

## User Experience

1. User opens the application
2. Default key is C Major (no accidentals)
3. User selects a different key from dropdown
4. Key signature appears on both staves
5. Key signature persists until changed
6. Notes can be drawn with the key signature visible

## Future Enhancements

### 1. Minor Keys
Add minor key options:
- A minor, E minor, B minor, etc.
- Same accidentals as relative majors

### 2. Key Signature Awareness
- Modify `noteHasAccidental()` to check against key signature
- Only show accidentals when they differ from key
- Add natural signs (♮) when canceling key signature

### 3. Transposition
- Transpose notes based on selected key
- Automatic key detection from MIDI input

### 4. Key Signature Animation
- Smooth fade in/out when changing keys
- Highlight current key in selector

## Code Structure

### Files Modified
- `MainComponent.h` - Added ComboBox and label
- `MainComponent.cpp` - Added selector initialization and callback
- `StaffDisplay.h` - Added setKeySignature() and drawKeySignature()
- `StaffDisplay.cpp` - Implemented key signature drawing

### Dependencies
- Uses existing `NoteGlyph` for Sharp and Flat symbols
- Uses `StaffGeometry` for position calculations
- Integrated with existing `LayoutMetrics` system

## Testing

### Test Cases
1. ✅ Select each key and verify correct number of accidentals
2. ✅ Verify sharps appear on correct staff positions
3. ✅ Verify flats appear on correct staff positions
4. ✅ Verify key signature displays on both staves
5. ✅ Verify key signature scales with window size
6. ✅ Verify key signature positions relative to clef

### Visual Verification
- Compare rendered key signatures to standard music notation
- Verify alignment with staff lines
- Check spacing between accidentals
- Ensure consistent appearance between treble and bass

## References
- Standard music notation conventions
- Circle of fifths ordering
- SMuFL specification for Sharp (U+E262) and Flat (U+E260)
- Traditional engraving spacing guidelines
