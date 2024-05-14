#include "PluginProcessor.h"
#include "AudioBackend/Brickwall.h"
#include "PluginEditor.h"
#include <limits>
#include <mutex>
#include <stdexcept>

AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    lastMaxGainReductionDB = 0;

    // setting that value requires that the corresponding AudioBackend default
    // value is the same.
    internalGainBoostDB = 0;

    addParameter(internalGainBoostParamDB = new juce::AudioParameterFloat("gain",                // parameterID
                                                                          "Internal Gain Boost", // parameter name
                                                                          0.0f,                  // minimum value
                                                                          MAX_DB_BOOST,          // maximum value
                                                                          0.0f));                // default value
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
    return false;
}

bool AudioPluginAudioProcessor::producesMidi() const
{
    return false;
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
    return false;
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double, int)
{
    channelsAudioProcessing[0].flushPastSignalResiduals();
    channelsAudioProcessing[1].flushPastSignalResiduals();
}

void AudioPluginAudioProcessor::releaseResources()
{
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    {
        return false;
    }
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    {
        return false;
    }

    return true;
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &)
{
    // a common thing to disabled for DSP, not sure if I should not remove it
    juce::ScopedNoDenormals noDenormals;

    std::scoped_lock<std::mutex> lock(gainChangeMutex);

    // if the gain changed, update it
    if (std::abs(*internalGainBoostParamDB - internalGainBoostDB) > std::numeric_limits<float>::epsilon())
    {
        internalGainBoostDB = *internalGainBoostParamDB;
        channelsAudioProcessing[0].setGainLevel(internalGainBoostDB);
        channelsAudioProcessing[1].setGainLevel(internalGainBoostDB);
    }

    // samples processing result with gain reduction
    CompressionResult processingResult;

    // max gain reduction
    float maxGainReduction = 0;

    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        auto *channelSamples = buffer.getWritePointer(channel);

        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            processingResult = channelsAudioProcessing[channel].process(channelSamples[i]);
            channelSamples[i] = processingResult.output;

            // we store the max gain reduction in the block to later display it
            if (processingResult.gainReductionDB < maxGainReduction)
            {
                maxGainReduction = processingResult.gainReductionDB;
            }
        }
    }

    lastMaxGainReductionDB = maxGainReduction;
}

bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this);
}

void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    destData.reset();
    float savedDbGain = getGainBoostDB();
    destData.append(&savedDbGain, sizeof(float));
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    if (sizeInBytes != sizeof(float))
    {
        throw std::runtime_error("Wrong state provided by DAW");
    }
    float newDbGain = *((float *)data);
    setGainBoostDB(newDbGain);
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

float AudioPluginAudioProcessor::getGainBoostDB() const
{
    return internalGainBoostDB;
}

float AudioPluginAudioProcessor::getGainReductionDB() const
{
    return lastMaxGainReductionDB;
}

void AudioPluginAudioProcessor::setGainBoostDB(float v)
{
    *internalGainBoostParamDB = v;
}