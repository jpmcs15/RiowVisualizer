#pragma once
#include <JuceHeader.h>
#include <array>

class RiowVisualizerAudioProcessor : public juce::AudioProcessor
{
public:
    RiowVisualizerAudioProcessor();
    ~RiowVisualizerAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Riow Visualizer"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    float getWaveSample(int index) const noexcept;
    float getSpectrumValue(int index) const noexcept;
    bool isNoteOn(int midiNote) const noexcept;
    float getPeak() const noexcept { return peak.load(); }
    void setStyle(int newStyle) noexcept { style.store(newStyle); }
    int getStyle() const noexcept { return style.load(); }

private:
    static constexpr int waveSize = 512;
    static constexpr int fftOrder = 10;
    static constexpr int fftSize = 1 << fftOrder;
    std::array<std::atomic<float>, waveSize> waveData {};
    std::array<std::atomic<float>, fftSize> fftInput {};
    std::array<std::atomic<float>, fftSize / 2> spectrumData {};
    std::array<std::atomic<bool>, 128> notes {};
    std::atomic<float> peak { 0.0f };
    std::atomic<int> style { 0 };

    juce::dsp::FFT fft { fftOrder };
    juce::dsp::WindowingFunction<float> window { fftSize, juce::dsp::WindowingFunction<float>::hann };
    std::array<float, fftSize * 2> fftWork {};
    int waveWrite = 0;
    int fftWrite = 0;
    int fftSamplesSinceUpdate = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RiowVisualizerAudioProcessor)
};
