#include "PluginEditor.h"

namespace
{
    const juce::Colour bg  { 0xff080a0b };
    const juce::Colour panel { 0xff111719 };
    const juce::Colour line { 0xffaab7b8 };
    const juce::Colour accent { 0xffe6eeee };
}

RiowVisualizerAudioProcessorEditor::RiowVisualizerAudioProcessorEditor(
    RiowVisualizerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(760, 470);
    titleLabel.setText("RIOW  /  VISUALIZER", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::FontOptions(22.0f, juce::Font::bold)));
    titleLabel.setColour(juce::Label::textColourId, accent);
    addAndMakeVisible(titleLabel);

    statusLabel.setText("AUDIO MONITOR  •  MIDI NOTE DISPLAY  •  VST3", juce::dontSendNotification);
    statusLabel.setFont(juce::Font(juce::FontOptions(11.0f)));
    statusLabel.setColour(juce::Label::textColourId, line);
    addAndMakeVisible(statusLabel);

    styleBox.addItem("01  RETRO MONO", 1);
    styleBox.addItem("02  Y2K GRID", 2);
    styleBox.addItem("03  BLUEPRINT", 3);
    styleBox.setSelectedId(1);
    styleBox.onChange = [this] { audioProcessor.setStyle(styleBox.getSelectedId() - 1); repaint(); };
    addAndMakeVisible(styleBox);
    startTimerHz(30);
}

void RiowVisualizerAudioProcessorEditor::timerCallback() { repaint(); }

void RiowVisualizerAudioProcessorEditor::resized()
{
    titleLabel.setBounds(24, 16, 430, 32);
    statusLabel.setBounds(26, 49, 470, 20);
    styleBox.setBounds(getWidth() - 190, 22, 164, 28);
}

void RiowVisualizerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(bg);
    const int style = audioProcessor.getStyle();
    g.setColour(panel);
    g.fillRoundedRectangle(20.0f, 82.0f, (float)getWidth() - 40.0f, 245.0f, 4.0f);
    g.setColour(line.withAlpha(0.65f));
    g.drawRoundedRectangle(20.0f, 82.0f, (float)getWidth() - 40.0f, 245.0f, 4.0f, 1.0f);

    g.setColour(line);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText(style == 0 ? "OSCILLOSCOPE / LIVE INPUT" :
               style == 1 ? "Y2K / LIVE INPUT" : "BLUEPRINT / LIVE INPUT",
               34, 92, 300, 18, juce::Justification::left);

    auto area = juce::Rectangle<float>(35.0f, 120.0f, (float)getWidth() - 70.0f, 170.0f);
    g.setColour(line.withAlpha(0.12f));
    for (int i = 0; i <= 10; ++i)
    {
        float x = area.getX() + area.getWidth() * i / 10.0f;
        g.drawVerticalLine((int)x, area.getY(), area.getBottom());
    }
    for (int i = 0; i <= 6; ++i)
    {
        float y = area.getY() + area.getHeight() * i / 6.0f;
        g.drawHorizontalLine((int)y, area.getX(), area.getRight());
    }

    if (style == 0)
    {
        juce::Path wave;
        for (int i = 0; i < 512; ++i)
        {
            const float x = area.getX() + area.getWidth() * (float)i / 511.0f;
            const float y = area.getCentreY() - audioProcessor.getWaveSample(i) * area.getHeight() * 0.43f;
            if (i == 0) wave.startNewSubPath(x, y); else wave.lineTo(x, y);
        }
        g.setColour(accent);
        g.strokePath(wave, juce::PathStrokeType(1.4f));
    }
    else
    {
        const int bars = 96;
        for (int i = 0; i < bars; ++i)
        {
            float v = audioProcessor.getSpectrumValue(i * 5);
            float h = juce::jmax(2.0f, v * area.getHeight());
            float x = area.getX() + area.getWidth() * (float)i / bars;
            float w = area.getWidth() / bars * 0.62f;
            g.setColour(style == 1 ? accent.withAlpha(0.85f) : line);
            g.fillRect(x, area.getBottom() - h, w, h);
        }
    }

    g.setColour(panel);
    g.fillRoundedRectangle(20.0f, 340.0f, (float)getWidth() - 40.0f, 82.0f, 4.0f);
    g.setColour(line.withAlpha(0.65f));
    g.drawRoundedRectangle(20.0f, 340.0f, (float)getWidth() - 40.0f, 82.0f, 4.0f, 1.0f);
    g.setColour(line);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("MIDI KEY MONITOR  /  NOTES 36–83", 32, 346, 260, 14, juce::Justification::left);

    const int firstNote = 36, numKeys = 48;
    const float keyW = ((float)getWidth() - 72.0f) / numKeys;
    const float keyY = 366.0f, keyH = 40.0f;
    for (int i = 0; i < numKeys; ++i)
    {
        const int note = firstNote + i;
        const int pitch = note % 12;
        const bool black = pitch == 1 || pitch == 3 || pitch == 6 || pitch == 8 || pitch == 10;
        const float x = 32.0f + i * keyW;
        g.setColour(audioProcessor.isNoteOn(note) ? accent : (black ? juce::Colour(0xff394244) : juce::Colour(0xffb9c2c2)));
        g.fillRect(x, keyY, keyW - 1.0f, keyH);
        g.setColour(bg);
        g.drawRect(x, keyY, keyW - 1.0f, keyH, 0.6f);
    }

    g.setColour(line);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("RIOW  •  PROTOTYPE 0.1  •  AUDIO PASSTHROUGH", 24, 438, getWidth() - 48, 18,
               juce::Justification::centredLeft);
}
