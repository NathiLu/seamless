/*
  ==============================================================================

    SourceSettingsComponent.h
    Created: 10 Apr 2021 10:58:19pm
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Common/SeamLess.h"

#include "PluginProcessor.h"

//==============================================================================
/*
*/
class SourceSettingsComponent  : public juce::Component, juce::Timer
{
public:
    SourceSettingsComponent(SeamLess_ClientAudioProcessor *a, juce::AudioProcessorValueTreeState &apvts);
    ~SourceSettingsComponent() override;

    void setConnectionFeedback(bool state);

    void paint (juce::Graphics&) override;
    void resized() override;



private:

    SeamLess_ClientAudioProcessor *processor;

    juce::Label inputLabel;
    juce::Label sourceIndText;
    juce::AudioProcessorParameter& sourceIndex;

    juce::TextButton mainConnectionButton;

    void timerCallback() override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceSettingsComponent)
};
