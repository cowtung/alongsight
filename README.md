# AlongSight

**AlongSight** is a music sight-reading tutor application built with JUCE and modern C++17. It provides real-time audio input analysis, visual feedback, and interactive sight-reading exercises.

## Features

- **Real-time Audio Input**: Captures audio from microphone or instrument input
- **Audio Level Meter**: Visual feedback of input audio levels with color-coded display
- **Music Staff Display**: Interactive musical notation display for sight-reading exercises
- **Start/Stop Controls**: Easy-to-use interface for controlling audio capture

## Technology Stack

- **Language**: C++17
- **Framework**: JUCE 7.0.9
- **Build System**: CMake 3.15+
- **Platform**: Windows (with cross-platform potential)

## Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (MSVC 2019+, GCC 7+, or Clang 5+)
- Git (for fetching JUCE)
- Audio input device (microphone or instrument interface)

## Building the Project

### Windows (Visual Studio)

```bash
# Clone the repository
git clone https://github.com/cowtung/alongsight.git
cd alongsight

# Create build directory
mkdir build
cd build

# Generate Visual Studio project
cmake ..

# Build the project
cmake --build . --config Release

# Run the application
Release\AlongSight.exe
```

### Windows (MinGW)

```bash
# Create build directory
mkdir build
cd build

# Generate Makefiles
cmake -G "MinGW Makefiles" ..

# Build
cmake --build .

# Run
.\AlongSight.exe
```

### Linux

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y build-essential cmake git \
    libasound2-dev libjack-jackd2-dev \
    libfreetype6-dev libx11-dev libxrandr-dev \
    libxinerama-dev libxcursor-dev

# Build
mkdir build && cd build
cmake ..
cmake --build .

# Run
./AlongSight
```

## Project Structure

```
alongsight/
├── Source/
│   ├── Main.cpp              # Application entry point
│   ├── MainComponent.cpp/h   # Main UI component
│   ├── AudioComponent.cpp/h  # Audio input handling
│   ├── AudioMeter.cpp/h      # Visual audio level meter
│   └── StaffDisplay.cpp/h    # Music notation display
├── Resources/
│   ├── Images/               # Image resources
│   └── Fonts/                # Font resources
├── CMakeLists.txt            # CMake build configuration
└── README.md                 # This file
```

## Usage

1. Launch the AlongSight application
2. Click the **Start** button to begin audio capture
3. Play your instrument or sing - the audio meter will show input levels
4. The staff display shows the musical notation area (exercise generation coming soon)
5. Click **Stop** to pause audio capture

## Future Development

- **Pitch Detection**: Real-time pitch analysis for note recognition
- **Rhythm Detection**: Temporal analysis for rhythm pattern recognition
- **Chord Recognition**: Multi-note chord identification
- **Exercise Generation**: Dynamic sight-reading exercise creation
- **Progress Tracking**: User progress monitoring and statistics
- **MIDI Support**: MIDI input device support
- **Custom Exercises**: User-defined practice patterns

## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
