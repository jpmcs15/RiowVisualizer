#include "PluginProcessor.h"
#include "PluginEditor.h"

RiowVisualizerAudioProcessor::RiowVisualizerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    for (auto& n : notes) n.store(false);
    for (auto& v : waveData) v.store(0.0f);
    for (auto& v : fftInput) v.store(0.0f);
    for (auto& v : spectrumData) v.store(0.0f);
}

void RiowVisualizerAudioProcessor::prepareToPlay(double, int)
{
    waveWrite = 0;
    fftWrite = 0;
    fftSamplesSinceUpdate = 0;
    peak.store(0.0f);
}

bool RiowVisualizerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto in = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();
    return (in == juce::AudioChannelSet::mono() || in == juce::AudioChannelSet::stereo())
        && in == out;
}

void RiowVisualizerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                 juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn()) notes[(size_t) msg.getNoteNumber()].store(true);
        else if (msg.isNoteOff()) notes[(size_t) msg.getNoteNumber()].store(false);
        else if (msg.isAllNotesOff() || msg.isAllSoundOff())
            for (auto& n : notes) n.store(false);
    }

    float blockPeak = 0.0f;
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    for (int s = 0; s < numSamples; ++s)
    {
        float mono = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            mono += buffer.getReadPointer(ch)[s];
        mono /= (float) juce::jmax(1, numChannels);
        blockPeak = juce::jmax(blockPeak, std::abs(mono));

        waveData[(size_t) waveWrite].store(mono);
        waveWrite = (waveWrite + 1) % waveSize;

        fftInput[(size_t) fftWrite].store(mono);
        fftWrite = (fftWrite + 1) % fftSize;
        if (++fftSamplesSinceUpdate >= fftSize / 4)
        {
            fftSamplesSinceUpdate = 0;
            for (int i = 0; i < fftSize; ++i)
                fftWork[(size_t)i] = fftInput[(size_t)((fftWrite + i) % fftSize)].load();
            window.multiplyWithWindowingTable(fftWork.data(), fftSize);
            fft.performFrequencyOnlyForwardTransform(fftWork.data());
            for (int i = 0; i < fftSize / 2; ++i)
            {
                const float normalized = juce::jlimit(0.0f, 1.0f,
                    std::log10(1.0f + fftWork[(size_t)i]) / 3.0f);
                spectrumData[(size_t)i].store(normalized);
            }
        }
    }

    peak.store(juce::jmax(blockPeak, peak.load() * 0.88f));
    // This plugin is a visual monitor: audio and incoming MIDI are passed through unchanged.
}

float RiowVisualizerAudioProcessor::getWaveSample(int index) const noexcept
{
    index = juce::jlimit(0, waveSize - 1, index);
    return waveData[(size_t)((waveWrite + index) % waveSize)].load();
}

float RiowVisualizerAudioProcessor::getSpectrumValue(int index) const noexcept
{
    index = juce::jlimit(0, fftSize / 2 - 1, index);
    return spectrumData[(size_t) index].load();
}

bool RiowVisualizerAudioProcessor::isNoteOn(int midiNote) const noexcept
{
    return midiNote >= 0 && midiNote < 128 && notes[(size_t)midiNote].load();
}

juce::AudioProcessorEditor* RiowVisualizerAudioProcessor::createEditor()
{
    return new RiowVisualizerAudioProcessorEditor(*this);
}

void RiowVisualizerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    stream.writeInt(style.load());
}

void RiowVisualizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (data != nullptr && sizeInBytes >= (int) sizeof(int))
        style.store(juce::jlimit(0, 2, *static_cast<const int*>(data)));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RiowVisualizerAudioProcessor();
}
