# Contributing to AlongSight

Thank you for your interest in contributing to AlongSight! This document will help you get set up and understand our development workflow.

## Project Overview

AlongSight is a music education app that teaches sight-reading by listening to audio input, detecting rhythm and chord progressions, and generating notes in real-time for users to play.

## Development Setup

### Prerequisites

1. **JUCE Framework**
   - Download from [juce.com/get-juce](https://juce.com/get-juce)
   - Install to any location on your system
   - Version: 7.0.x or later recommended

2. **Projucer**
   - Comes with the JUCE download
   - Used to manage the project structure and generate IDE project files

3. **Visual Studio 2026** (Windows) or **Xcode** (macOS)
   - Install the C++ development workload
   - Visual Studio 2022 also works

### Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/cowtung/alongsight.git
   cd alongsight
   ```

2. **Configure JUCE paths in Projucer**
   - Open Projucer
   - Go to **Settings** (gear icon) → **Global Paths**
   - Set **Path to JUCE** to your JUCE installation directory
   - Set **JUCE Modules** to `<your_juce_path>/modules`

3. **Open the project**
   - Open `AlongSight.jucer` in Projucer
   - Click **"Save and Open in IDE"**
   - This generates platform-specific project files and opens your IDE

4. **Build and run**
   - In Visual Studio: Press F5 or click Build → Build Solution
   - In Xcode: Press Cmd+R or click Product → Run

## Development Workflow

### Important: Projucer is the Source of Truth

AlongSight uses **Projucer** to manage the project structure. The IDE project files (`.sln`, `.vcxproj`, `.xcodeproj`) are **generated** and should never be edited directly.

```
AlongSight.jucer  →  (generates)  →  IDE project files
```

### Adding New Source Files

**✅ DO THIS:**
1. Open `AlongSight.jucer` in Projucer
2. Right-click the **Source** folder in the left sidebar
3. Select **"Add New C++ File"** or **"Add Existing Files"**
4. Name your file (Projucer creates both `.cpp` and `.h`)
5. Click **Save Project** (Ctrl+S / Cmd+S)
6. Return to your IDE - it will prompt to reload the project
7. Click **Reload All**

**❌ DON'T DO THIS:**
- Don't add files directly in Visual Studio or Xcode
- Don't manually edit `.vcxproj` or `.xcodeproj` files
- Any changes made in the IDE will be overwritten when Projucer regenerates

### Editing Code

**Use your IDE (Visual Studio/Xcode) for:**
- Writing and editing `.cpp` and `.h` files
- Debugging
- Building and running
- Code completion and IntelliSense
- Git operations

**Use Projucer for:**
- Adding/removing source files
- Adding/removing JUCE modules
- Changing project settings (version, bundle ID, etc.)
- Configuring build configurations
- Managing exporters (Visual Studio/Xcode)

### Typical Development Flow

1. **Make structural changes in Projucer** (if needed)
   - Add new files, modules, or change settings
   - Save the project

2. **Write code in your IDE**
   - Edit existing files
   - Build and test

3. **Commit your changes**
   ```bash
   git add Source/
   git add AlongSight.jucer
   git commit -m "Add chord detection algorithm"
   ```

## Code Style

### Formatting

This project uses **tabs for indentation** (displayed as 4 spaces wide).

- A `.clang-format` file is provided in the repository
- Format your code before committing:
  - Visual Studio: Ctrl+K, Ctrl+D (Format Document)
  - Xcode: Use ClangFormat extension or command line

### Naming Conventions

Follow JUCE coding conventions:
- **Classes**: `PascalCase` (e.g., `ChordDetector`, `AudioAnalyzer`)
- **Methods**: `camelCase` (e.g., `processAudioBlock`, `detectChord`)
- **Member variables**: `camelCase` (e.g., `sampleRate`, `bufferSize`)
- **Constants**: `ALL_CAPS` or `kPascalCase` (e.g., `MAX_CHANNELS`, `kDefaultBufferSize`)

## Project Structure

```
alongsight/
├── .clang-format           # Code formatting rules
├── .editorconfig           # Editor configuration
├── .gitignore              # Git ignore rules
├── AlongSight.jucer        # Projucer project file (COMMIT THIS)
├── README.md               # Project overview
├── CONTRIBUTING.md         # This file
├── Source/                 # Your source code (COMMIT THIS)
│   ├── Main.cpp
│   ├── MainComponent.cpp
│   └── MainComponent.h
├── Builds/                 # Generated IDE files (DON'T COMMIT)
│   └── VisualStudio2026/
│       └── AlongSight.sln
└── JuceLibraryCode/        # Generated JUCE glue code (DON'T COMMIT)
```

## What to Commit

**✅ Always commit:**
- `AlongSight.jucer` (project file)
- `Source/` (your code)
- `.clang-format`, `.editorconfig`, `.gitignore`
- Documentation files (`.md`)

**❌ Never commit:**
- `Builds/` (generated IDE project files)
- `JuceLibraryCode/` (auto-generated)
- Build artifacts (`Debug/`, `Release/`, `x64/`, etc.)
- IDE user settings (`.vs/`, `.user`, `.suo`)

## Pull Request Guidelines

1. **Create a feature branch**
   ```bash
   git checkout -b feature/chord-detection
   ```

2. **Make your changes**
   - Follow the code style guidelines
   - Add comments for complex logic
   - Test your changes thoroughly

3. **Format your code**
   - Use Ctrl+K, Ctrl+D in Visual Studio
   - Ensure tabs are used for indentation

4. **Commit with clear messages**
   ```bash
   git commit -m "Add FFT-based chord detection algorithm"
   ```

5. **Push and create a pull request**
   ```bash
   git push origin feature/chord-detection
   ```
   - Go to https://github.com/cowtung/alongsight
   - Click "New Pull Request"
   - Describe your changes clearly

6. **Respond to review feedback**
   - Make requested changes
   - Push updates to the same branch

## Testing

- Build and run the application before submitting a PR
- Test on your target platform (Windows/macOS)
- Verify that audio input and chord detection work as expected
- Check for memory leaks and performance issues

## Getting Help

- **Issues**: Check [existing issues](https://github.com/cowtung/alongsight/issues) or open a new one
- **Discussions**: Use GitHub Discussions for questions and ideas
- **JUCE Documentation**: https://juce.com/learn/documentation
- **JUCE Forum**: https://forum.juce.com/

## License

By contributing to AlongSight, you agree that your contributions will be licensed under the Apache 2.0 License (or whatever license is specified in the LICENSE file).

## Questions?

If you have any questions about contributing, please open an issue or start a discussion on GitHub!

---

Thank you for contributing to AlongSight! 🎵
