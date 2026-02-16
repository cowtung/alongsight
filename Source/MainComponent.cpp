#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Configure title label
    titleLabel.setText("AlongSight - Music Sight-Reading Tutor", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);

    // Configure status label
    statusLabel.setText("Ready - Click Start to begin", juce::dontSendNotification);
    statusLabel.setFont(juce::Font(14.0f));
    statusLabel.setJustificationType(juce::Justification::centred);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(statusLabel);

    // Configure start/stop button
    startStopButton.setButtonText("Start");
    startStopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    startStopButton.onClick = [this] { startStopButtonClicked(); };
    addAndMakeVisible(startStopButton);

    // Add audio meter
    addAndMakeVisible(audioMeter);

    // Add staff display
    addAndMakeVisible(staffDisplay);

    // Set initial size
    setSize(800, 600);

    // Start timer for updating UI
    startTimerHz(30);
}

MainComponent::~MainComponent()
{
    stopTimer();
}

void MainComponent::paint(juce::Graphics& g)
{
    // Draw gradient background
    g.fillAll(juce::Colours::darkblue.darker());
    
    auto bounds = getLocalBounds();
    juce::ColourGradient gradient(
        juce::Colours::darkblue.darker(), 0.0f, 0.0f,
        juce::Colours::darkslateblue, 0.0f, (float)bounds.getHeight(),
        false);
    g.setGradientFill(gradient);
    g.fillAll();
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
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
    
    // Staff display takes most of the space
    auto displayArea = bounds.removeFromTop((int)(bounds.getHeight() * 0.7f));
    staffDisplay.setBounds(displayArea);
    
    bounds.removeFromTop(10);
    
    // Audio meter on the right side
    auto meterArea = bounds.removeFromRight(60);
    audioMeter.setBounds(meterArea);
}

void MainComponent::timerCallback()
{
    // Update audio meter with current level
    if (isRunning)
    {
        static constexpr float audioLevelScale = 5.0f;
        float level = audioComponent.getAudioLevel();
        audioMeter.setLevel(level * audioLevelScale); // Scale up for better visibility
    }
}

void MainComponent::startStopButtonClicked()
{
    isRunning = !isRunning;
    
    if (isRunning)
    {
        audioComponent.startRecording();
        startStopButton.setButtonText("Stop");
        startStopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        statusLabel.setText("Listening... Play or sing to see audio levels", juce::dontSendNotification);
    }
    else
    {
        audioComponent.stopRecording();
        startStopButton.setButtonText("Start");
        startStopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        statusLabel.setText("Stopped - Click Start to begin", juce::dontSendNotification);
        audioMeter.setLevel(0.0f);
    }
}
