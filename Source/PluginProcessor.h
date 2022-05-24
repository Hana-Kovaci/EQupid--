/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once



#include <JuceHeader.h>
#include "StkLite-4.6.1/Biquad.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "Band.h"
#include <vector>

//==============================================================================
/**
*/
class EQupidAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    EQupidAudioProcessor();
    ~EQupidAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    //because we can't just set the gain on the highpass and lowpass to 0, we need to find out a way to disable them from running on the process block when they haven't been selected yet
    //SO what we're gonna do is create a bool that lets us know whether it was added as a parameter
    bool lpSelected = false;
    bool hpSelected = false;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQupidAudioProcessor)

        void calcAlgorithmParams();

    //user params

    // 1) Old Parameters:
    juce::AudioParameterFloat* lsParam;
    juce::AudioParameterFloat* hsParam;
    juce::AudioParameterFloat* lpGainParam;
    juce::AudioParameterFloat* hpGainParam;
    juce::AudioParameterFloat* notchParam;
    juce::AudioParameterFloat* notchQParam;

    // 1b) Old Parameter Frequencies:
    juce::AudioParameterFloat* lsFqParam;
    juce::AudioParameterFloat* hsFqParam;
    juce::AudioParameterFloat* notchFqParam;

    // 2) New Parameters:
    juce::AudioParameterFloat* hpParam;
    juce::AudioParameterFloat* lpParam;

    // 2b) New Parameter Frequencies:
    juce::AudioParameterFloat* hpFqParam;
    juce::AudioParameterFloat* lpFqParam;






    //mu45 objs
    stk::BiQuad mhsL, mhsR, mhpL, mhpR, mlpL, mlpR, mlsL, mlsR, mnotchL, mnotchR;
    float mFs; //sampling rate


};
