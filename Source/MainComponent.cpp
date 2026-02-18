#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	// Configure title label
	titleLabel.setText("AlongSight - Music Sight-Reading Tutor", juce::dontSendNotification);
	titleLabel.setFont(juce::Font(juce::FontOptions(24.0f, juce::Font::bold)));
	titleLabel.setJustificationType(juce::Justification::centred);
	titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(titleLabel);
	
    // Configure status label
	statusLabel.setText("Ready - Click Start to begin", juce::dontSendNotification);
	statusLabel.setFont(juce::Font(juce::FontOptions(14.0f)));
	statusLabel.setJustificationType(juce::Justification::centred);
	statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
	addAndMakeVisible(statusLabel);

	// Configure start/stop button
	startStopButton.setButtonText("Start");
	startStopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
	startStopButton.onClick = [this] { startStopButtonClicked(); };
	addAndMakeVisible(startStopButton);

	// Configure key signature selector
	keySignatureLabel.setText("Key:", juce::dontSendNotification);
	keySignatureLabel.setFont(juce::Font(juce::FontOptions(14.0f)));
	keySignatureLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(keySignatureLabel);

	keySignatureSelector.addItem("C Major", 1);
	keySignatureSelector.addItem("G Major (1#)", 2);
	keySignatureSelector.addItem("D Major (2#)", 3);
	keySignatureSelector.addItem("A Major (3#)", 4);
	keySignatureSelector.addItem("E Major (4#)", 5);
	keySignatureSelector.addItem("B Major (5#)", 6);
	keySignatureSelector.addItem("F# Major (6#)", 7);
	keySignatureSelector.addItem("F Major (1b)", 8);
	keySignatureSelector.addItem("Bb Major (2b)", 9);
	keySignatureSelector.addItem("Eb Major (3b)", 10);
	keySignatureSelector.addItem("Ab Major (4b)", 11);
	keySignatureSelector.addItem("Db Major (5b)", 12);
	keySignatureSelector.addItem("Gb Major (6b)", 13);
	keySignatureSelector.setSelectedId(1); // Default to C Major
	keySignatureSelector.onChange = [this] { keySignatureChanged(); };
	addAndMakeVisible(keySignatureSelector);

	// Add staff display
	addAndMakeVisible(staffDisplay);


	// array of durations. 1, 2, 4, 8, 16
	const int durs[] = {1, 2, 4, 8, 16};

	float xOffset = 75.0f; // Starting offset for notes (after clef)
	float currentX = 0.0f;

	// Treble clef example notes
	const int trebleNotes[] = {64, 66, 67, 69, 71, 72, 73, 74, 76, 77}; // E4, F#4, G4, A4, B4, C5, C#5, D5, E5, F5

	for(int i = 0; i < 10; ++i)
	{
		// Cycle through durations for demonstration
		staffDisplay.drawTrebleNote(trebleNotes[i], durs[i % 5], currentX += xOffset);
	}

	// Bass clef example notes
	currentX = 0.0f; // Reset x position for bass clef
	const int bassNotes[] = {43, 45, 46, 48, 50, 52, 53, 55, 57, 59}; // G2, A2, A#2, C3, D3, E3, F3, G3, A3, B3

	for(int i = 0; i < 10; ++i)
	{
		// Cycle through durations for demonstration
		staffDisplay.drawBassNote(bassNotes[i], durs[i % 5], currentX += xOffset);
	}

	setSize(1600, 1200);

	// Start timer for updating UI
	startTimerHz(30);
}

//==============================================================================
MainComponent::~MainComponent()
{
	stopTimer();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
	// Draw gradient background
	g.fillAll(juce::Colours::darkblue.darker());

	auto bounds = getLocalBounds();
	juce::ColourGradient gradient(juce::Colours::darkblue.darker(), 0.0f, 0.0f,
								  juce::Colours::darkslateblue, 0.0f, (float)bounds.getHeight(),
								  false);
	g.setGradientFill(gradient);
	g.fillAll();


	//// (Our component is opaque, so we must completely fill the background with a solid colour)
	//g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	//g.setFont(juce::FontOptions(16.0f));
	//g.setColour(juce::Colours::white);
	//g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

//==============================================================================
void MainComponent::resized()
{
	auto bounds = getLocalBounds().reduced(10);

	// Key signature selector in top left corner
	auto topLeftArea = bounds.removeFromTop(30);
	auto keyArea = topLeftArea.removeFromLeft(250);
	keySignatureLabel.setBounds(keyArea.removeFromLeft(40));
	keySignatureSelector.setBounds(keyArea);

	bounds.removeFromTop(10);

	// Title at top
	titleLabel.setBounds(bounds.removeFromTop(40));
	bounds.removeFromTop(10);

	    // Status label
	statusLabel.setBounds(bounds.removeFromTop(25));
	bounds.removeFromTop(10);

	// Control area at bottom
	auto controlArea = bounds.removeFromBottom(60);
	startStopButton.setBounds(controlArea.withSizeKeepingCentre(120, 40));

	bounds.removeFromBottom(10);

	// Staff display takes the remaining space
	staffDisplay.setBounds(bounds);
}

//==============================================================================
void MainComponent::timerCallback()
{
	// Audio meter removed - no updates needed
}

//==============================================================================
void MainComponent::startStopButtonClicked()
{
	isRunning = !isRunning;

	if(isRunning)
	{
		audioComponent.startRecording();

		// Start MIDI input
		audioComponent.startMidiInput();

		// Connect MIDI callback to staff display
		audioComponent.onMidiNote = [this](int midiNote, bool isNoteOn)
		{
			if (isNoteOn)
			{
				// Determine which staff based on note range
				// Middle C (MIDI 60) and above go to treble, below to bass
				bool isTreble = (midiNote >= 60);
				staffDisplay.setLiveNote(midiNote, isTreble);
			}
			else
			{
				staffDisplay.clearLiveNote(midiNote);
			}
		};
		audioComponent.startMidiInput();

		audioComponent.onMidiNote = [this](int midiNote, bool isNoteOn)
		{
			if (isNoteOn)
			{
				bool isTreble = (midiNote >= 60);
				staffDisplay.setLiveNote(midiNote, isTreble);
			}
			else
			{
				staffDisplay.clearLiveNote(midiNote);
			}
		};

		startStopButton.setButtonText("Stop");
		startStopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
		statusLabel.setText("Listening... Play notes on your MIDI keyboard",
							juce::dontSendNotification);
	}
	else
	{
		audioComponent.stopRecording();
			audioComponent.stopMidiInput();
			staffDisplay.clearAllLiveNotes();
		startStopButton.setButtonText("Start");
		startStopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
		statusLabel.setText("Stopped - Click Start to begin", juce::dontSendNotification);
	}
}

void MainComponent::keySignatureChanged()
{
	int selectedId = keySignatureSelector.getSelectedId();

	// Map item ID to number of sharps (positive) or flats (negative)
	int accidentals = 0;
	switch (selectedId)
	{
		case 1: accidentals = 0; break;   // C Major
		case 2: accidentals = 1; break;   // G Major (1#)
		case 3: accidentals = 2; break;   // D Major (2#)
		case 4: accidentals = 3; break;   // A Major (3#)
		case 5: accidentals = 4; break;   // E Major (4#)
		case 6: accidentals = 5; break;   // B Major (5#)
		case 7: accidentals = 6; break;   // F# Major (6#)
		case 8: accidentals = -1; break;  // F Major (1b)
		case 9: accidentals = -2; break;  // Bb Major (2b)
		case 10: accidentals = -3; break; // Eb Major (3b)
		case 11: accidentals = -4; break; // Ab Major (4b)
		case 12: accidentals = -5; break; // Db Major (5b)
		case 13: accidentals = -6; break; // Gb Major (6b)
		default: accidentals = 0; break;
	}

	staffDisplay.setKeySignature(accidentals);
}
