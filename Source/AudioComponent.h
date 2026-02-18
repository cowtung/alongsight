#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <atomic>

//==============================================================================
/**
	AudioComponent handles audio input for real-time analysis
	Provides audio level monitoring and will support pitch/rhythm detection
*/
class AudioComponent : public juce::AudioAppComponent,
					   public juce::MidiInputCallback
{
public:
	AudioComponent();
	~AudioComponent() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	// Audio level monitoring
	float getAudioLevel() const { return audioLevel.load(); }
	bool isRecording() const { return recording.load(); }
	
	void startRecording();
	void stopRecording();

	// MIDI input handling
	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
	void startMidiInput();
	void stopMidiInput();

	// Callback for MIDI note events
	std::function<void(int midiNote, bool isNoteOn)> onMidiNote;

private:
	std::atomic<float> audioLevel{0.0f};
	std::atomic<bool> recording{false};
	double currentSampleRate{0.0};
	int currentBlockSize{0};

	std::vector<std::unique_ptr<juce::MidiInput>> midiInputs;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioComponent)
};
