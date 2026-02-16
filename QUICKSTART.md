# AlongSight Quick Start Guide

Welcome to AlongSight! This guide will help you get started with the music sight-reading tutor.

## Installation

### Windows

1. **Prerequisites**:
   - Install Visual Studio 2019+ with C++ tools, OR
   - Install MinGW-w64 with CMake

2. **Build**:
   ```cmd
   build.bat
   ```

3. **Run**:
   ```cmd
   build\Release\AlongSight.exe
   ```

### Linux

1. **Prerequisites**:
   ```bash
   sudo apt-get install build-essential cmake git \
       libasound2-dev libjack-jackd2-dev \
       libfreetype6-dev libx11-dev libxrandr-dev \
       libxinerama-dev libxcursor-dev
   ```

2. **Build**:
   ```bash
   ./build.sh
   ```

3. **Run**:
   ```bash
   ./build/AlongSight_artefacts/AlongSight
   ```

### macOS

1. **Prerequisites**:
   ```bash
   brew install cmake
   ```

2. **Build**:
   ```bash
   ./build.sh
   ```

3. **Run**:
   ```bash
   ./build/AlongSight_artefacts/AlongSight
   ```

## First Time Setup

1. **Audio Device**:
   - Connect your microphone or audio interface
   - Ensure your device is recognized by your operating system
   - Check audio input settings if needed

2. **Launch AlongSight**:
   - The application will automatically detect available audio devices
   - The main window shows the title, status bar, staff display, and controls

## Using AlongSight

### Basic Operation

1. **Start Audio Capture**:
   - Click the green **Start** button
   - Status changes to "Listening..."
   - The button turns red and shows **Stop**

2. **Monitor Your Playing**:
   - Play your instrument or sing
   - Watch the audio meter on the right side
   - The meter shows input level with color coding:
     - Green: Good level
     - Yellow: Moderate level
     - Red: High level

3. **Stop Audio Capture**:
   - Click the red **Stop** button
   - Audio meter resets
   - Ready for next session

### Interface Components

#### Title Bar
- Displays "AlongSight - Music Sight-Reading Tutor"

#### Status Label
- Shows current application state:
  - "Ready - Click Start to begin"
  - "Listening... Play or sing to see audio levels"
  - "Stopped - Click Start to begin"

#### Staff Display
- Shows a standard 5-line musical staff
- Displays treble clef
- White background with black staff lines
- Future: Will display notes and exercises

#### Audio Meter
- Vertical bar on the right side
- Real-time audio level visualization
- Color gradient from green (bottom) to red (top)
- Smooth decay animation

#### Start/Stop Button
- Green when stopped (click to start)
- Red when running (click to stop)
- Centered at bottom of window

## Tips

- **Optimal Audio Levels**: Aim for green to yellow on the audio meter
- **Too Quiet**: If meter shows no activity, increase input volume
- **Too Loud**: If meter stays in red, reduce input volume
- **No Audio Device**: Ensure microphone/interface is connected before starting

## Troubleshooting

### No Audio Input
- Check microphone is connected and powered
- Verify system audio settings
- Try different USB port (for USB devices)
- Check device permissions (especially on macOS)

### Application Won't Start
- Ensure all dependencies are installed
- Check CMake build completed successfully
- Verify audio drivers are installed

### Build Errors
- Ensure CMake 3.15+ is installed
- Check C++17 compiler support
- Verify all dependencies installed (Linux)

## Keyboard Shortcuts

Currently, the application is controlled via mouse/touch interface. Keyboard shortcuts may be added in future versions.

## What's Next?

Current version focuses on audio input foundation. Future versions will add:

- **Pitch Detection**: Identify notes you play
- **Rhythm Analysis**: Recognize timing patterns
- **Exercise Generation**: Automatic sight-reading exercises
- **Progress Tracking**: Monitor your improvement
- **MIDI Support**: Use MIDI keyboards

## Getting Help

- Read the [README.md](README.md) for detailed information
- Check [ARCHITECTURE.md](ARCHITECTURE.md) for technical details
- See [CONTRIBUTING.md](CONTRIBUTING.md) for development info
- Report issues on GitHub

## License

See LICENSE file for licensing information.

---

**Thank you for using AlongSight!**

Practice regularly and watch your sight-reading skills improve! 🎵
