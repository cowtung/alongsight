# Accidentals (Sharp/Flat) Implementation

## Overview
Added support for displaying sharp (♯) and flat (♭) accidental symbols in music notation. Accidentals are automatically detected based on MIDI note values and drawn to the left of notes.

## Changes Made

### 1. NoteGlyph Class Updates

#### **NoteGlyph.h**
- Added `Sharp` and `Flat` to the `Type` enum
- These are accidental symbols that modify note pitch

#### **NoteGlyph.cpp**
- **SMuFL Codes Added:**
  - Sharp: `0xE262` (accidentalSharp)
  - Flat: `0xE260` (accidentalFlat)
  
- **Glyph Setup:**
  - Both accidentals configured with `hasStem = false`
  - Paths and bounds pre-calculated during initialization
  
- **Center Point Calculation:**
  - Accidentals use glyph center like whole notes
  - Special handling in `measureGlyphs()` for Sharp and Flat types

### 2. StaffDisplay Class Updates

#### **StaffDisplay.h**
Added three new helper methods:
```cpp
bool noteHasAccidental(int midiNote) const;
NoteGlyph::Type getAccidentalType(int midiNote) const;
void drawAccidental(juce::Graphics& g, float x, float y, 
                   NoteGlyph::Type accidentalType, float scale);
```

#### **StaffDisplay.cpp**

**`noteHasAccidental()`**
- Checks if a MIDI note is a black key (sharp/flat)
- Returns `true` for C♯, D♯, F♯, G♯, A♯ (MIDI mod 12: 1, 3, 6, 8, 10)

**`getAccidentalType()`**
- Determines which accidental symbol to use
- Currently returns Sharp for all black keys
- Future enhancement: Use key signatures to determine sharp vs flat

**`drawAccidental()`**
- Draws the accidental symbol using pre-created paths from NoteGlyph
- Uses same scaling and positioning logic as notes
- Centers the accidental vertically on the staff line/space

**Note Drawing Loop (paint method)**
- After drawing each note, checks if it needs an accidental
- Positions accidental 1.2 staff spaces to the left of the note
- Draws accidental at the same vertical position as the note

### 3. MainComponent Updates

Updated example notes to demonstrate accidentals:
```cpp
const int notes[] = {64, 66, 67, 69, 71, 72, 73, 74, 76, 77};
// E, F#, G, A, B, C, C#, D, E, F
```

This shows:
- Natural notes: E, G, A, B, C, D, E, F
- Sharps: F♯ (MIDI 66), C♯ (MIDI 73)

## Implementation Details

### Positioning
- **Horizontal**: Accidentals placed `staffLineSpacing * 1.2` to the left of note
- **Vertical**: Centered on the same staff line/space as the note
- **Scaling**: Uses same scale factor as notes (based on whole note height)

### Drawing Order
1. Draw ledger lines (if needed)
2. Draw the note
3. Draw the accidental (if needed) - positioned to the left

### MIDI Note Detection
Black keys (sharps/flats) are identified by MIDI note modulo 12:
- **1**: C♯/D♭
- **3**: D♯/E♭
- **6**: F♯/G♭
- **8**: G♯/A♭
- **10**: A♯/B♭

## Future Enhancements

### 1. Key Signature Support
- Implement key signature detection
- Use context to determine sharp vs flat
- Example: Key of G major → F♯, Key of F major → B♭

### 2. Enharmonic Equivalents
- Add logic to choose between sharp/flat based on musical context
- Consider melodic direction and harmonic function

### 3. Natural Signs
- Add natural (♮) symbol support (SMuFL: 0xE261)
- Display naturals when canceling previous accidentals

### 4. Double Sharps/Flats
- Add double sharp (𝄪) and double flat (𝄫) support
- SMuFL codes: 0xE263 (double sharp), 0xE264 (double flat)

### 5. Accidental Placement Rules
- Implement proper spacing for chords with multiple accidentals
- Follow standard engraving rules for accidental alignment

## Testing

### Test Cases
1. ✅ Sharp symbols display correctly
2. ✅ Accidentals positioned to the left of notes
3. ✅ Accidentals scale with staff size
4. ✅ Accidentals align vertically with note heads
5. ✅ Natural notes display without accidentals

### Example Notes
The current example displays:
- E (natural)
- F♯ (sharp)
- G (natural)
- A (natural)
- B (natural)
- C (natural)
- C♯ (sharp)
- D (natural)
- E (natural)
- F (natural)

## SMuFL Reference

| Symbol | Name | SMuFL Code | Unicode |
|--------|------|------------|---------|
| ♯ | Sharp | 0xE262 | U+E262 |
| ♭ | Flat | 0xE260 | U+E260 |
| ♮ | Natural | 0xE261 | U+E261 |
| 𝄪 | Double Sharp | 0xE263 | U+E263 |
| 𝄫 | Double Flat | 0xE264 | U+E264 |

## Related Files
- `NoteGlyph.h/cpp` - Accidental glyph definitions and measurements
- `StaffDisplay.h/cpp` - Accidental detection and rendering
- `MainComponent.cpp` - Example usage with sharps
- `docs/NOTE_GLYPH_ARCHITECTURE.md` - Overall architecture documentation
