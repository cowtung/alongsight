# Contributing to AlongSight

Thank you for your interest in contributing to AlongSight! This document provides guidelines for contributing to the project.

## Development Setup

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler
- Git
- Audio input device for testing

### Getting Started

1. Fork the repository
2. Clone your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/alongsight.git
   cd alongsight
   ```

3. Build the project:
   ```bash
   # Linux/macOS
   ./build.sh
   
   # Windows
   build.bat
   ```

## Code Structure

```
Source/
├── Main.cpp              # Application entry point
├── MainComponent.*       # Main UI component
├── AudioComponent.*      # Audio input handling
├── AudioMeter.*          # Audio level visualization
└── StaffDisplay.*        # Music notation display
```

## Coding Standards

- Use C++17 features appropriately
- Follow JUCE coding conventions
- Keep components modular and testable
- Add comments for complex algorithms
- Use meaningful variable names

## Component Guidelines

### AudioComponent
- Handles real-time audio input
- Provides audio level monitoring
- Will support pitch/rhythm detection

### MainComponent
- Integrates all UI components
- Manages application state
- Handles user interactions

### AudioMeter
- Visual feedback for audio levels
- Smooth decay animation
- Color-coded level indication

### StaffDisplay
- Renders musical staff
- Will display notes and exercises
- Supports standard music notation

## Testing

Currently, the project focuses on building and running the application manually. Future additions:

- Unit tests for audio processing
- UI component tests
- Integration tests

## Pull Request Process

1. Create a feature branch from `main`
2. Make your changes
3. Test your changes thoroughly
4. Update documentation as needed
5. Submit a pull request with a clear description

## Future Development Areas

- **Audio Analysis**: Implement FFT-based pitch detection
- **Rhythm Detection**: Temporal pattern recognition
- **Chord Recognition**: Multi-note identification
- **Exercise Generation**: Dynamic sight-reading exercises
- **Progress Tracking**: User statistics and history
- **MIDI Support**: MIDI device integration

## Questions?

Feel free to open an issue for any questions or concerns!
