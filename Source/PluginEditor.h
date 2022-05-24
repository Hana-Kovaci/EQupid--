/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;


//==============================================================================
/**
*/
class EQupidAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Slider::Listener

{
public:
    EQupidAudioProcessorEditor (EQupidAudioProcessor&);
    ~EQupidAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider *slider);
    




private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EQupidAudioProcessor& audioProcessor;

    //these are our bands
    Slider ls;
    Slider hs;
    Slider lpq;
    Slider hpq;
    Slider notch; // go together hehe
    Slider notchQ;

    //these are our frequencies
    Slider lsfreq;
    Slider hsfreq;
    Slider lpfreq;
    Slider hpfreq;
    Slider notchfreq;

    //ghost sliders that only exist for helping with positioning
    Slider lpGain;
    Slider hpGain;

    //lovely labels that go with frequencies
    Label lsLabel;
    Label hsLabel;
    Label notchLabel;
    Label notchQLabel;
    Label lpLabel;
    Label hpLabel;

    //int that keeps track of when bands were added (this is needed because order is relevant to position)
    //not that complicated but this is small scale so i'm keeping it as simple as possible
    int bandOrder = 0;
    //


    //----------variables dependent on when band selection was made. USE THESE TO SET BOUNDS

    int gainX = 0;

    int freqX = 200;

    int xSize = 70;
    int ySize = 70;

    int xSizeFreq = 400;
    int ySizeFreq = 40;

    
        
    ComboBox bandSelection;

    void bandSelectionMade() { //once a selection is made in the combobox, we can create our band!
        switch (bandSelection.getSelectedId()) {
        case lpass: {
            lpGain.setBounds(gainX, 80+65*bandOrder,xSize, ySize);
            lpfreq.setBounds(freqX, lpGain.getBounds().getY()+15, xSizeFreq, ySizeFreq);
            addAndMakeVisible(lpfreq);
            lpfreq.setValue(250);
            lpq.setBounds(gainX + 75,  lpGain.getBounds().getY()-10, xSize - 5, ySize - 5);
            addAndMakeVisible(lpq);
            break;
        }
        case hpass: {
            hpGain.setBounds(gainX, 80+65*bandOrder, xSize, ySize);
            hpfreq.setBounds(freqX, hpGain.getBounds().getY() + 15, xSizeFreq, ySizeFreq);
            addAndMakeVisible(hpfreq);
            hpfreq.setValue(2000);
            hpq.setBounds(gainX + 75, hpGain.getBounds().getY()-10, xSize - 5, ySize - 5);
            addAndMakeVisible(hpq);
            break; 
        }
        case lshelf: {
            ls.setBounds(gainX, 80+65*bandOrder, xSize, ySize);
            addAndMakeVisible(ls);
            lsfreq.setBounds(freqX, ls.getBounds().getY() + 15, xSizeFreq, ySizeFreq);
            addAndMakeVisible(lsfreq);
            break; 
        }
        case hshelf: {
            hs.setBounds(gainX, 80+65*bandOrder, xSize, ySize);
            addAndMakeVisible(hs);
            hsfreq.setBounds(freqX, hs.getBounds().getY() + 15, xSizeFreq, ySizeFreq);
            addAndMakeVisible(hsfreq);
            break; 
        }
        case peak: {
            notch.setBounds(gainX, 80+65*bandOrder, xSize, ySize);
            addAndMakeVisible(notch);
            notchfreq.setBounds(freqX, notch.getBounds().getY() + 15, xSizeFreq, ySizeFreq);
            addAndMakeVisible(notchfreq);
            notchQ.setBounds(gainX + 75, notch.getBounds().getY()-10, xSize - 5, ySize - 5);
            addAndMakeVisible(notchQ);
            break; 
        }
        default: {
            break; }

        }
        bandOrder++;
        bandSelection.setItemEnabled(bandSelection.getSelectedId(), false); //only 5 unique bandcs can be created so we disable the option to add it again
    }
   
    enum BandTypes {
        lpass = 1,
        hpass,
        lshelf,
        hshelf,
        peak
    };
    //make some buttons to push

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQupidAudioProcessorEditor)
};
