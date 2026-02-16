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
