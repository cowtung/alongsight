# Note Position Verification

Based on the debug output, here are the expected positions:

## Staff Line Positions
- Line 0 (E4, bottom): Y = 123.5
- Line 1 (G4): Y = 103.5
- Line 2 (B4, middle): Y = 83.5
- Line 3 (D5): Y = 63.5
- Line 4 (F5, top): Y = 43.5

## Staff Spacing
- Staff line spacing: 20.0 pixels
- Staff position spacing: 10.0 pixels (half line spacing)

## Expected Note Positions (Treble Clef)
| Note | MIDI | Staff Pos | Expected Y | Description |
|------|------|-----------|------------|-------------|
| C4   | 60   | -2        | 143.5      | Ledger line below |
| D4   | 62   | -1        | 133.5      | Space below |
| E4   | 64   | 0         | 123.5      | Bottom line |
| F4   | 65   | 1         | 113.5      | First space |
| G4   | 67   | 2         | 103.5      | Second line |
| A4   | 69   | 3         | 93.5       | Second space |
| B4   | 71   | 4         | 83.5       | Middle line |
| C5   | 72   | 5         | 73.5       | Third space |
| D5   | 74   | 6         | 63.5       | Fourth line |
| E5   | 76   | 7         | 53.5       | Fourth space |
| F5   | 77   | 8         | 43.5       | Top line |

## Calculation
Position Y = bottomLineY - (staffPosition × lineSpacing × 0.5)
Position Y = 123.5 - (staffPosition × 10.0)

Example for C5 (MIDI 72, Position 5):
Y = 123.5 - (5 × 10.0) = 123.5 - 50.0 = 73.5

This should place C5 in the third space, which is correct.
