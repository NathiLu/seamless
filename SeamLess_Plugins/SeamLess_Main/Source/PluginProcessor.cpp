/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SeamLess_MainAudioProcessor::SeamLess_MainAudioProcessor()
    : AudioProcessor (BusesProperties())
{


    if (! connect (9002))
        std::cout << "Can not open port!" << '\n';
    else
        std::cout << "Connected to port 9002" << '\n';

    // Register OSC paths
    juce::OSCReceiver::addListener(this, "/source/pos/x");
    juce::OSCReceiver::addListener(this, "/source/pos/y");
    juce::OSCReceiver::addListener(this, "/source/pos/z");

    juce::OSCReceiver::addListener(this, "/send/gain");

    beginWaitingForSocket(incomingPort,"");


}

SeamLess_MainAudioProcessor::~SeamLess_MainAudioProcessor()
{
}

//==============================================================================
const juce::String SeamLess_MainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SeamLess_MainAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SeamLess_MainAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SeamLess_MainAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SeamLess_MainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SeamLess_MainAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SeamLess_MainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SeamLess_MainAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SeamLess_MainAudioProcessor::getProgramName (int index)
{
    return {};
}

void SeamLess_MainAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SeamLess_MainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void SeamLess_MainAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SeamLess_MainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SeamLess_MainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
//    juce::ScopedNoDenormals noDenormals;
//    auto totalNumInputChannels  = getTotalNumInputChannels();
//    auto totalNumOutputChannels = getTotalNumOutputChannels();

//    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
//        buffer.clear (i, 0, buffer.getNumSamples());


//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);

//    }
}

//==============================================================================
bool SeamLess_MainAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SeamLess_MainAudioProcessor::createEditor()
{
    return new SeamLess_MainAudioProcessorEditor (*this);
}

//==============================================================================
void SeamLess_MainAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml (new juce::XmlElement ("HoFo_MAIN"));
    xml->setAttribute ("incomingPort", (int) incomingPort);
    copyXmlToBinary (*xml, destData);
}

void SeamLess_MainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName ("HoFo_MAIN"))
            setIncomingPort((int) xmlState->getIntAttribute("incomingPort", 1.0));

}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SeamLess_MainAudioProcessor();
}


void SeamLess_MainAudioProcessor::oscMessageReceived (const juce::OSCMessage& message)
{


    int nArgs = message.size();
    std::cout << "Main plugin: received OSC message with " << nArgs << " arguments!" <<  std::endl;

    juce::String path(message.getAddressPattern().toString());
    juce::String msg = path;

    // All position messages:
    if(path=="/source/pos/x" || path=="/source/pos/y" || path=="/source/pos/z")
    {
        if(nArgs==2 && message[0].isInt32() && message[1].isFloat32())
        {
            msg+= " "+juce::String(message[0].getInt32());
            msg+= " "+juce::String(message[1].getFloat32());
        }
    }

    // All send gains:
    if(path=="/send/gain")
    {
        if(nArgs==3 && message[0].isInt32() && message[1].isInt32() && (message[2].isFloat32() || message[2].isInt32()))
        {
            msg+= " "+juce::String(message[0].getInt32());
            msg+= " "+juce::String(message[1].getInt32());
            msg+= " "+juce::String(message[2].getFloat32());
        }
    }

    juce::MemoryBlock mb(msg.toRawUTF8(), msg.length());

    // loop over all connections and send message
    for(auto const& c: connections)
        c->sendMessage(mb);

}


juce::InterprocessConnection* SeamLess_MainAudioProcessor::createConnectionObject()
{

    MainConnection* c =  new MainConnection(stop_signal,this);
    connections.push_back(c);

    // std::cout << "HuFo main plugin received new connection! N=" << connections.size() << '\n';
    return c;

}


void SeamLess_MainAudioProcessor::setIncomingPort(int p)
{
    incomingPort = p;

    if (! connect (p))
        std::cout << "Can not open port!" << '\n';
    else
        std::cout << "Connected to port " << p << '\n';
}


int SeamLess_MainAudioProcessor::getIncomingPort()
{
    return incomingPort;
}


void SeamLess_MainAudioProcessor::removeClosedConnections()
{

     for (auto it = begin(connections); it != end(connections);) {
       if ((*it)->isConnected()==false)
            it = connections.erase(it);
          else
            ++it;
        }

}
