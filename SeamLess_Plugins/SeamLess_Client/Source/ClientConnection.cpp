/*
  ==============================================================================

    ClientConnection.cpp
    Created: 19 Mar 2021 4:14:08pm
    Author:  anwaldt

  ==============================================================================
*/

#include "ClientConnection.h"

ClientConnection::ClientConnection(SeamLess_ClientAudioProcessor& p)
    : InterprocessConnection(false, 15)
{
    processor = &p;
}

void ClientConnection::connectionMade()
{
    printf("Connection established from client plugin!\n");
}

void ClientConnection::connectionLost()
{
    printf("Connection lost\n");
}


void ClientConnection::messageReceived(const juce::MemoryBlock& msg)
{

    juce::String str        = msg.toString();
    juce::StringArray array = juce::StringArray::fromTokens(str," ");


    if(array[1].getIntValue() == processor->getSourceIndex())
    {
        std::cout << "Client " << processor->getSourceIndex() << " received message: " << str << '\n';

        if(array[0]=="/source/pos/x")
            processor->setXPos(array[2].getFloatValue());

        else if(array[0]=="/source/pos/y")
            processor->setYPos(array[2].getFloatValue());

        else if(array[0]=="/source/pos/z")
            processor->setZPos(array[2].getFloatValue());

        else if(array[0]=="/send/gain")
            processor->setSendGain(array[2].getIntValue(), array[3].getFloatValue());
    }
}


