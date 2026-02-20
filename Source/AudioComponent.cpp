#include "AudioComponent.h"

//==============================================================================
AudioComponent::AudioComponent()
{
	// Configure audio device with 2 input channels and 2 output channels
	setAudioChannels(2, 2);
}

AudioComponent::~AudioComponent()
{
	shutdownAudio();
}

void AudioComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	currentSampleRate = sampleRate;
	currentBlockSize = samplesPerBlockExpected;
}

void AudioComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	// Clear output buffer (we're not outputting audio, just analyzing input)
	bufferToFill.clearActiveBufferRegion();

	if (!recording.load())
	{
		audioLevel.store(0.0f);
		return;
	}

	// Calculate RMS level from input
	auto* buffer = bufferToFill.buffer;
	const int numSamples = bufferToFill.numSamples;
	float level = 0.0f;

	for (int channel = 0; channel < buffer->getNumChannels(); ++channel)
	{
		auto* channelData = buffer->getReadPointer(channel, bufferToFill.startSample);
		
		for (int i = 0; i < numSamples; ++i)
		{
			const float sample = channelData[i];
			level += sample * sample;
		}
	}

	// Calculate RMS and average across channels
	if (buffer->getNumChannels() > 0 && numSamples > 0)
	{
		level = std::sqrt(level / (buffer->getNumChannels() * numSamples));
		audioLevel.store(level);
	}
}

void AudioComponent::releaseResources()
{
	// Clean up any resources
}

void AudioComponent::startRecording()
{
	recording.store(true);
}

void AudioComponent::stopRecording()
{
	recording.store(false);
	audioLevel.store(0.0f);
}

void AudioComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
	if (message.isNoteOn())
	{
		int noteNumber = message.getNoteNumber();
		int velocity = message.getVelocity();
		DBG("[MIDI] Note ON  - Note: " << noteNumber << " (" << juce::MidiMessage::getMidiNoteName(noteNumber, true, true, 4) 
			<< "), Velocity: " << velocity << ", Channel: " << message.getChannel());

		// MIDI callbacks happen on the MIDI thread, so we need to call the callback on the message thread
		if (onMidiNote)
		{
			juce::MessageManager::callAsync([callback = onMidiNote, noteNumber]()
			{
				callback(noteNumber, true);
			});
		}
	}
	else if (message.isNoteOff())
	{
		int noteNumber = message.getNoteNumber();
		DBG("[MIDI] Note OFF - Note: " << noteNumber << " (" << juce::MidiMessage::getMidiNoteName(noteNumber, true, true, 4) 
			<< "), Channel: " << message.getChannel());

		// MIDI callbacks happen on the MIDI thread, so we need to call the callback on the message thread
		if (onMidiNote)
		{
			juce::MessageManager::callAsync([callback = onMidiNote, noteNumber]()
			{
				callback(noteNumber, false);
			});
		}
	}
}

void AudioComponent::startMidiInput()
{
	// Find and open ALL available MIDI input devices
	auto availableDevices = juce::MidiInput::getAvailableDevices();

	if (availableDevices.isEmpty())
	{
		DBG("No MIDI input devices found");
		return;
	}

	DBG("====================================");
	DBG("Found " << availableDevices.size() << " MIDI input device(s)");
	DBG("====================================");

	// Open each available MIDI device
	for (const auto& device : availableDevices)
	{
		DBG("Attempting to open: " << device.name);
		DBG("  Device ID: " << device.identifier);

		auto input = juce::MidiInput::openDevice(device.identifier, this);

		if (input)
		{
			input->start();
			midiInputs.push_back(std::move(input));
			DBG("  ✓ SUCCESS - Device is now listening");
		}
		else
		{
			DBG("  ✗ FAILED - Device may be in use by another application");
			DBG("    Close any DAWs, MIDI software, or other apps using this device");
		}
	}

	DBG("====================================");
	DBG("Total MIDI devices opened: " << midiInputs.size() << " of " << availableDevices.size());
	DBG("====================================");

	if (midiInputs.empty())
	{
		DBG("WARNING: No MIDI devices could be opened!");
		DBG("Please check:");
		DBG("  1. Close any other applications using MIDI devices");
		DBG("  2. Restart this application");
		DBG("  3. Check device drivers are installed correctly");
	}
}

void AudioComponent::stopMidiInput()
{
	for (auto& input : midiInputs)
	{
		if (input)
		{
			input->stop();
		}
	}
	midiInputs.clear();
	DBG("All MIDI devices stopped");
}
