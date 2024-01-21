#pragma once

#include "PluginProcessor.h"

/**
 * @brief Class that describes the GUI of the plugin.
 */
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
  public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;
    void paint(juce::Graphics &) override;
    void resized() override;
    void timerCallback() override;
    void mouseDrag(const juce::MouseEvent &ev) override;
    void mouseDown(const juce::MouseEvent &ev) override;
    void mouseUp(const juce::MouseEvent &ev) override;

  private:
    /**
     * @brief Updates the pixel position rectangle for gain
     * cursor and gain reduction vumeter needle.
     */
    void updateProcessorVariables();

    AudioPluginAudioProcessor &processorRef; /**< Provided as an easy way to
                                                access the audio/midi processor. */

    juce::Image backgroundImage;                         /**< Image for the plugin background */
    std::unique_ptr<juce::Drawable> gainCursor;          /**< Cursor to change db gain */
    std::unique_ptr<juce::Drawable> gainReductionNeedle; /**< Needle to indicate gain reduction */

    juce::Rectangle<int> lastCursorPosition; /**< rectangle with pixel position of cursor */
    juce::Rectangle<int> lastNeedlePosition; /**< rectangle with pixel position of needle */

    bool draggingGainCursor; /**< Boolean to know if we are currently dragging the gain cursor button */

    float lastGainReductionDB,
        lastGainBoostDB; /**< last recorded values for gain reduction vumeter and gain boost setting */

    // destroy copy constructors
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
