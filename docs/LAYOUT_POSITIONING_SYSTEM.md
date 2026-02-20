# Global Relative Positioning System for Music Notation

## Overview
Implemented a comprehensive layout system that establishes relative positioning between all elements of the music staff (brace, clefs, and staff lines). This ensures consistent spacing and alignment that scales properly with window resizing.

## LayoutMetrics Structure

Added a `LayoutMetrics` struct to `StaffDisplay` that holds all positioning information:

```cpp
struct LayoutMetrics
{
    float braceX = 0.0f;               // Left edge of brace
    float braceWidth = 0.0f;           // Width of brace
    float clefLeftX = 0.0f;            // Left edge of treble clef
    float clefRightX = 0.0f;           // Right edge of both clefs
    float clefWidth = 0.0f;            // Width of clef symbols
    float clefScale = 1.0f;            // Scale factor for clefs
    float staffStartX = 0.0f;          // Where staff lines begin
    juce::Path trebleClefPath;         // Pre-transformed treble clef path
    juce::Path bassClefPath;           // Pre-transformed bass clef path
};
```

## Layout Calculation Method

### `calculateLayout()` Process

1. **Brace Positioning**
   - Position: `whiteBounds.getX() + 2.0f`
   - Measures brace glyph to determine width
   - Stores both position and width

2. **Treble Clef Positioning**
   - Left edge: `braceX + braceWidth + 10.0f` (10 pixels from brace)
   - Height: `4 * staffLineSpacing` (top to bottom lines)
   - Measures glyph to calculate width and scale
   - Pre-transforms path and stores it

3. **Bass Clef Positioning**
   - Right edge aligns with treble clef right edge
   - Uses same scale as treble clef
   - Calculates left position to achieve right-edge alignment
   - Pre-transforms path and stores it

4. **Staff Lines Positioning**
   - Start: `clefRightX + 10.0f` (10 pixels from clef)
   - Used by StaffGeometry for line placement

## Positioning Relationships

```
[Brace] --10px-- [Treble Clef] --10px-- [Staff Lines...]
           ^
           |
           +-- Bass Clef (right-aligned)
```

### Key Measurements:
- **Brace to Treble Clef**: 10 pixels
- **Treble Clef to Staff Lines**: 10 pixels
- **Bass Clef Right Edge**: Aligns with Treble Clef Right Edge
- **Both Clefs**: Same scale factor (based on staff height)

## Implementation Changes

### StaffDisplay.h
1. Added `LayoutMetrics` struct
2. Added `calculateLayout()` method
3. Added `currentLayout` member variable

### StaffDisplay.cpp

#### `paint()` Method
1. Creates temporary geometry objects
2. Calls `calculateLayout()` to compute all positions
3. Creates final geometry objects with correct staff start position
4. Drawing methods use pre-calculated positions and paths

#### `drawTrebleClef()` and `drawBassClef()`
- Simplified to just draw pre-transformed paths from `currentLayout`
- No more position calculation at draw time

#### `drawBrace()`
- Uses `currentLayout.braceX` for positioning
- No more manual calculation of white bounds position

#### `calculateLayout()`
- Central method that calculates all element positions
- Pre-transforms clef paths for efficient rendering
- Returns complete LayoutMetrics structure

## Benefits

### 1. Centralized Control
- All positioning logic in one place
- Easy to adjust spacing relationships
- Single source of truth for layout

### 2. Performance
- Clef paths pre-transformed once per paint
- No redundant glyph measurements at draw time
- Efficient path reuse

### 3. Consistency
- All elements use same reference points
- Guaranteed alignment between clefs
- Predictable spacing that scales properly

### 4. Maintainability
- Clear relationships between elements
- Easy to modify spacing constants
- Self-documenting code structure

### 5. Scalability
- Automatically adjusts to window resizing
- Maintains proportions at all sizes
- Clefs scale uniformly

## Spacing Constants

Current spacing values (easily adjustable):

```cpp
const float BRACE_MARGIN = 2.0f;      // Brace from white edge
const float BRACE_TO_CLEF = 10.0f;    // Brace to treble clef
const float CLEF_TO_STAFF = 10.0f;    // Clef to staff lines
```

## Future Enhancements

### 1. Configurable Spacing
Add properties or settings for:
- Brace-to-clef spacing
- Clef-to-staff spacing
- Clef scale adjustments

### 2. Time Signature and Key Signature
Extend layout system to include:
- Time signature positioning (after clef)
- Key signature positioning (between clef and time signature)
- Proper spacing calculations

### 3. Multiple Staff Systems
Support for:
- Multiple staff systems with consistent alignment
- System spacing calculations
- Brace/bracket positioning for instrument groups

### 4. Dynamic Clef Changes
Handle:
- Mid-staff clef changes
- Different clef types (alto, tenor, etc.)
- Transitional clef sizing and positioning

## Testing

### Verified Behavior:
✅ Brace positioned at left edge of white area
✅ Treble clef 10 pixels from brace right edge
✅ Bass clef right edge aligns with treble clef
✅ Both clefs have same scale
✅ Staff lines 10 pixels from clef right edge
✅ All elements scale properly on resize
✅ Notes positioned correctly relative to new staff start

### Test Cases:
1. Window resize - all elements maintain relationships
2. Different staff spacings - proportions preserved
3. Clef alignment - right edges match exactly
4. Note positioning - correct relative to staff start

## Related Files
- `StaffDisplay.h` - LayoutMetrics struct, calculateLayout() declaration
- `StaffDisplay.cpp` - Layout calculation and usage
- `StaffGeometry.h/cpp` - Staff line positioning (uses layout.staffStartX)
- `docs/NOTATION_SPACING_SYSTEM.md` - Overall spacing documentation

## References
- Standard music notation spacing conventions
- Professional engraving guidelines
- JUCE Path and AffineTransform documentation
