# AlongSight Quick Start Guide

Welcome to AlongSight! This guide will help you get started with the music sight-reading tutor.

## Installation

### Prerequisites

**All Platforms:**
- Download and install [JUCE](https://juce.com/get-juce/download) (free GPL/Commercial license)
- Install the Projucer application (included with JUCE)

### Windows

1. **Additional Prerequisites**:
   - Install Visual Studio 2019 or later with C++ desktop development tools
   - Ensure Windows 10 SDK is installed

2. **Setup with Projucer**:
   - Open `alongsight.jucer` in Projucer
   - Click "Save Project and Open in IDE" (or press Ctrl+P)
   - This will generate and open the Visual Studio solution in `Builds\VisualStudio2026\`

3. **Build**:
   - In Visual Studio, select your build configuration (Debug/Release)
   - Build > Build Solution (or press F7)

4. **Run**:
   - Press F5 to run with debugging, or Ctrl+F5 without debugging
   - Executable location: `Builds\VisualStudio2026\x64\Debug\App\AlongSight_App.exe`

### Linux

1. **Additional Prerequisites**:
   ```bash
   sudo apt-get install build-essential git \
       libasound2-dev libjack-jackd2-dev \
       libfreetype6-dev libx11-dev libxrandr-dev \
       libxinerama-dev libxcursor-dev libwebkit2gtk-4.0-dev
   ```

2. **Setup with Projucer**:
   - Open `alongsight.jucer` in Projucer
   - Click "Save Project and Open in IDE" to export Makefile
   - This will generate build files in `Builds/LinuxMakefile/`

3. **Build**:
   ```bash
   cd Builds/LinuxMakefile
   make CONFIG=Release
   ```

4. **Run**:
   ```bash
   ./build/AlongSight_App
   ```

### macOS

1. **Additional Prerequisites**:
   - Install Xcode from the Mac App Store
   - Install Xcode Command Line Tools:
     ```bash
     xcode-select --install
     ```

2. **Setup with Projucer**:
   - Open `alongsight.jucer` in Projucer
   - Click "Save Project and Open in IDE" to export Xcode project
   - This will generate and open the project in `Builds/MacOSX/`

3. **Build**:
   - In Xcode, select your build scheme and target device
   - Product > Build (or press ⌘B)

4. **Run**:
   - Product > Run (or press ⌘R)
   - Application bundle: `Builds/MacOSX/build/Debug/AlongSight_App.app`

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
