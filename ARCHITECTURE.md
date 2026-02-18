# AlongSight Architecture

## Overview

AlongSight is a JUCE-based application for teaching music sight-reading. The application is built using modern C++17 and follows a component-based architecture.

## System Architecture

```
┌────────────────────────────────────────────────────────┐
│                   AlongSight Application               │
├────────────────────────────────────────────────────────┤
│                      MainComponent                     │
│  ┌──────────────────────────────────────────────────┐  │
│  │  Title: "AlongSight - Music Sight-Reading Tutor" │  │
│  │  Status: Ready/Listening messages                │  │
│  └──────────────────────────────────────────────────┘  │
│                                                        │
│  ┌──────────────────────────────────────────────────┐  │
│  │            StaffDisplay Component                │  │
│  │  - Renders 5-line musical staff                  │  │
│  │  - Displays treble clef                          │  │
│  │  - Future: Note/chord display                    │  │
│  └──────────────────────────────────────────────────┘  │
│                                                        │
│  ┌──────────┐   ┌──────────────────────────────────┐   │
│  │  Audio   │   │      Start/Stop Button           │   │
│  │  Meter   │   │  - Toggle audio capture          │   │
│  │          │   │  - Green (Start) / Red (Stop)    │   │
│  └──────────┘   └──────────────────────────────────┘   │
└────────────────────────────────────────────────────────┘
                         │
                         ▼
┌────────────────────────────────────────────────────────┐
│                  AudioComponent                        │
│  (AudioAppComponent - JUCE Audio Integration)          │
│                                                        │
│  - Audio input capture (2 channels)                    │
│  - Real-time level calculation (RMS)                   │
│  - Future: FFT for pitch detection                     │
│  - Future: Rhythm pattern analysis                     │
└────────────────────────────────────────────────────────┘
                         │
                         ▼
┌────────────────────────────────────────────────────────┐
│                 Hardware Audio Input                   │
│  (Microphone, Instrument Interface, etc.)              │
└────────────────────────────────────────────────────────┘
```

## Component Responsibilities

### Main.cpp
- **Purpose**: Application entry point
- **Responsibilities**:
  - Initialize JUCE application
  - Create main window
  - Handle application lifecycle

### MainComponent
- **Purpose**: Primary UI container and coordinator
- **Responsibilities**:
  - Layout all UI components
  - Coordinate between audio and visual components
  - Handle start/stop button events
  - Update audio meter based on audio input
  - Display status messages

### AudioComponent
- **Purpose**: Audio input handling
- **Inherits**: `juce::AudioAppComponent`
- **Responsibilities**:
  - Configure audio device (2 input channels)
  - Capture real-time audio samples
  - Calculate RMS audio levels
  - Provide audio level data to UI
  - Future: Pitch detection via FFT
  - Future: Rhythm pattern recognition

### AudioMeter
- **Purpose**: Visual audio level display
- **Responsibilities**:
  - Display audio input levels visually
  - Color-coded gradient (green → yellow → red)
  - Smooth decay animation
  - Update at 30 FPS

### StaffDisplay
- **Purpose**: Musical notation display
- **Responsibilities**:
  - Render standard 5-line musical staff
  - Display treble clef symbol
  - Future: Display notes at correct positions
  - Future: Display chords and rhythms
  - Future: Animate sight-reading exercises

## Data Flow

1. **Audio Input → AudioComponent**
   - Hardware audio device provides samples
   - AudioComponent captures in `getNextAudioBlock()`

2. **AudioComponent → Level Calculation**
   - Calculate RMS from audio samples
   - Store atomic float for thread-safe access

3. **MainComponent Timer → Level Query**
   - Timer callback runs at 30 Hz
   - Queries current audio level from AudioComponent

4. **Level → AudioMeter Display**
   - AudioMeter receives level update
   - Smoothly animates display
   - Renders color-coded bar

## Threading Model

- **Audio Thread**: 
  - Handles audio callbacks (`getNextAudioBlock`)
  - Must be real-time safe
  - No blocking operations
  - Uses atomic variables for communication

- **UI Thread**:
  - Handles user interactions
  - Updates visual components
  - Timer-based polling of audio levels
  - Repaints at 30 FPS

- **Thread Safety**:
  - Atomic variables for audio level
  - Atomic bool for recording state
  - No locks in audio callback

## Future Extensions

### Phase 1: Basic Audio Analysis
- FFT implementation for frequency analysis
- Pitch detection algorithm
- Note onset detection

### Phase 2: Musical Intelligence
- Chord recognition from multiple notes
- Rhythm pattern analysis
- Tempo detection

### Phase 3: Exercise Generation
- Dynamic sight-reading exercise creation
- Difficulty levels
- Progress-based adaptation

### Phase 4: User Experience
- Exercise library
- Progress tracking and statistics
- Custom exercise designer
- MIDI device support

## Build System

- **CMake**: Modern CMake 3.15+ configuration
- **JUCE**: Framework fetched via FetchContent
- **C++17**: Standard library features
- **Cross-platform**: Windows, Linux, macOS support

## Dependencies

### Required JUCE Modules
- `juce_audio_basics` - Core audio types
- `juce_audio_devices` - Audio I/O
- `juce_audio_formats` - Audio file formats
- `juce_audio_processors` - Audio processing
- `juce_audio_utils` - Audio utility components
- `juce_core` - Foundation classes
- `juce_data_structures` - Data structures
- `juce_events` - Event handling
- `juce_graphics` - 2D graphics
- `juce_gui_basics` - GUI components
- `juce_gui_extra` - Additional GUI features

## Performance Considerations

- Audio callback must complete within ~10ms
- UI updates at 30 FPS for smooth animation
- Atomic variables avoid mutex overhead
- Efficient RMS calculation in audio callback
- Display level decay for visual smoothness
