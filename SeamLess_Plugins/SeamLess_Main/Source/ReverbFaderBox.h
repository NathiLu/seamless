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

#include "ReverbFader.h"


//==============================================================================
/*
*/
class ReverbFaderBox  : public juce::Component, juce::Slider::Listener
{
public:

    ReverbFaderBox(SeamLess_MainAudioProcessor &p, juce::AudioProcessorValueTreeState &apvts);
    ~ReverbFaderBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /// \brief sliderValueChanged
    /// \param slider
    /// A slider listener for all send gain sliders.
    void sliderValueChanged (juce::Slider* slider) override;

private:

    SeamLess_MainAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    ReverbFader revGainFader;
    ReverbFader revFreq1Fader;
    ReverbFader revFreq2Fader;
    ReverbFader revRdelFader;
    ReverbFader revRgxyzFader;
    ReverbFader revT60dcFader;
    ReverbFader revT60mFader;
    ReverbFader revLpFreqFader;
    ReverbFader revLpRsFader;
    ReverbFader revLpDbFader;



    //    SendFader sendFaderLFE;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbFaderBox)
};
