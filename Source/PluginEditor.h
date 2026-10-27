#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class RiowVisualizerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                           private juce::Timer
{
public:
    explicit RiowVisualizerAudioProcessorEditor(RiowVisualizerAudioProcessor&);
    ~RiowVisualizerAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    RiowVisualizerAudioProcessor& audioProcessor;
    juce::ComboBox styleBox;
    juce::Label titleLabel, statusLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RiowVisualizerAudioProcessorEditor)
};
