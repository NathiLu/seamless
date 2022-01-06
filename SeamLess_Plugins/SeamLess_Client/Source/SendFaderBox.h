/*
  ==============================================================================

    SendFaderBox.h
    Created: 31 Mar 2021 12:13:37am
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../Common/SeamLess.h"
#include "PluginProcessor.h"

#include "SendFader.h"

//==============================================================================
/*
*/
class SendFaderBox  : public juce::Component, juce::Slider::Listener
{
public:

    SendFaderBox(SeamLess_ClientAudioProcessor &p, juce::AudioProcessorValueTreeState &apvts);
    ~SendFaderBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /// \brief sliderValueChanged
    /// \param slider
    /// A slider listener for all send gain sliders.
    void sliderValueChanged (juce::Slider* slider) override;

private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    SendFader sendFaderHOA;
    SendFader sendFaderWFS;
    SendFader sendFaderREV;
//    SendFader sendFaderLFE;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SendFaderBox)
};
