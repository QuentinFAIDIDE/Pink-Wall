#include "PluginEditor.h"
#include "BackgroundImage.h"
#include "PluginProcessor.h"
#include <limits>

#define CURSOR_WIDTH 11
#define CURSOR_HEIGHT 10

#define UI_RULE_0DB_X 474
#define UI_RULE_30DB_X 618
#define UI_RULE_BOTTOM_Y 87

#define NEEDLE_WIDTH 12
#define NEEDLE_HEIGHT 16

#define NEEDLE_RELEASE_ITERATION_DB 0.5f

#define UI_FPS 19

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    setSize(650, 143);

    draggingGainCursor = false;

    // load the background image
    backgroundImage =
        juce::ImageFileFormat::loadFrom(BackgroundImage::background_png, BackgroundImage::background_pngSize);
    // load the svg cursors, and vu meter needle
    gainCursor = juce::Drawable::createFromImageData(BackgroundImage::cursor_svg, BackgroundImage::cursor_svgSize);
    gainReductionNeedle =
        juce::Drawable::createFromImageData(BackgroundImage::gain_needle_svg, BackgroundImage::gain_needle_svgSize);

    lastGainBoostDB = 0;
    lastGainReductionDB = 0;

    lastCursorPosition.setWidth(CURSOR_WIDTH);
    lastCursorPosition.setHeight(CURSOR_HEIGHT);
    lastCursorPosition.setY(UI_RULE_BOTTOM_Y);
    lastCursorPosition.setX(UI_RULE_0DB_X - (CURSOR_WIDTH >> 1));

    lastNeedlePosition.setWidth(NEEDLE_WIDTH);
    lastNeedlePosition.setHeight(NEEDLE_HEIGHT);
    lastNeedlePosition.setY(UI_RULE_BOTTOM_Y - NEEDLE_HEIGHT);
    lastNeedlePosition.setX(UI_RULE_0DB_X - (NEEDLE_WIDTH >> 1));

    updateProcessorVariables();

    startTimer(int(1000 / UI_FPS));
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    updateProcessorVariables();
}

void AudioPluginAudioProcessorEditor::updateProcessorVariables()
{
    // let's not eat precious DSP CPU when the UI is not visible
    if (!isShowing())
    {
        return;
    }

    float gainBoostDB = processorRef.getGainBoostDB();
    float gainReductionDB = processorRef.getGainReductionDB();

    lastGainBoostDB = gainBoostDB;

    if (lastGainReductionDB < gainReductionDB)
    {
        lastGainReductionDB = juce::jlimit(-MAX_DB_BOOST, 0.0f, lastGainReductionDB + NEEDLE_RELEASE_ITERATION_DB);
    }
    else if (lastGainReductionDB > gainReductionDB)
    {
        lastGainReductionDB = gainReductionDB;
    }

    float centerGainBoostPositionX = UI_RULE_0DB_X + (juce::jlimit(0.0f, 1.0f, (lastGainBoostDB / MAX_DB_BOOST)) *
                                                      (float)(UI_RULE_30DB_X - UI_RULE_0DB_X));
    float centerGainReducPositionX =
        UI_RULE_0DB_X + (juce::jlimit(0.0f, 1.0f, (std::abs(lastGainReductionDB) / MAX_DB_BOOST)) *
                         (float)(UI_RULE_30DB_X - UI_RULE_0DB_X));

    lastCursorPosition.setX(centerGainBoostPositionX - (CURSOR_WIDTH >> 1));
    lastNeedlePosition.setX(centerGainReducPositionX - (NEEDLE_WIDTH >> 1));

    repaint();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.drawImage(backgroundImage, g.getClipBounds().toFloat());

    gainCursor->drawWithin(g, lastCursorPosition.toFloat(), juce::RectanglePlacement::centred, 1.0f);
    gainReductionNeedle->drawWithin(g, lastNeedlePosition.toFloat(), juce::RectanglePlacement::centred, 1.0f);
}

void AudioPluginAudioProcessorEditor::mouseDrag(const juce::MouseEvent &ev)
{
    if (draggingGainCursor)
    {
        float newGain =
            MAX_DB_BOOST * (((float)(ev.position.x - UI_RULE_0DB_X)) / (float)(UI_RULE_30DB_X - UI_RULE_0DB_X));
        newGain = juce::jlimit(0.0f, 30.0f, newGain);
        processorRef.setGainBoostDB(newGain);
        repaint();
    }
}

void AudioPluginAudioProcessorEditor::mouseDown(const juce::MouseEvent &ev)
{
    if (lastCursorPosition.contains(ev.getMouseDownPosition()))
    {
        if (ev.mods.isLeftButtonDown() && !ev.mods.isShiftDown() && !ev.mods.isAltDown() && !ev.mods.isCtrlDown())
        {
            draggingGainCursor = true;
        }
    }
}

void AudioPluginAudioProcessorEditor::mouseUp(const juce::MouseEvent &ev)
{
    if (ev.mods.isLeftButtonDown())
    {
        draggingGainCursor = false;
    }
}

void AudioPluginAudioProcessorEditor::resized()
{
    // this plugin can't really be resized and we don't have much layout to do
}
