/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EQupidAudioProcessor::EQupidAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{


    addParameter(lsParam = new juce::AudioParameterFloat("LowShelf Cutoff",
        "low Shelf", //parameterName,
        -10.0f, //minimum
        10.0f, //maximum
        0.0f //default
    ));

    addParameter(hsParam = new juce::AudioParameterFloat("highShelf Cutoff",
        "high Shelf", //parameterName
        -10.0f, //minimum
        10.0f, //maximum
        0.0f //default
    ));
    addParameter(notchParam = new juce::AudioParameterFloat("peak filter",
        "notch",
        -10.0f,
        10.0f,
        0.0f));
    addParameter(notchQParam = new juce::AudioParameterFloat("peak Q",
        "Q",
        0.1f,
        3.0f,
        0.6f));


    addParameter(lpParam = new juce::AudioParameterFloat("lowpass Cutoff",
        "low pass", //parameterName
        0.1f, //minimum
        3.0f, //maximum
        0.6f //default
    ));


    addParameter(hpParam = new juce::AudioParameterFloat("highpass Cutoff",
        "high pass", //parameterName
        0.1f, //minimum
        3.0f, //maximum
        0.6f //default
    ));


    addParameter(lsFqParam = new juce::AudioParameterFloat("Lowshelf transition Frequency", "ls fc", 20, 4196, 250));//bass frequencies
    addParameter(hsFqParam = new juce::AudioParameterFloat("Highshelf transition Frequency", " hs fc", 20, 8000, 2000));//high midrange

    addParameter(notchFqParam = new juce::AudioParameterFloat("Notch transition frequency", " notch fc", 20, 8000, 160));


    addParameter(lpFqParam = new juce::AudioParameterFloat("Lowpass transition frequency", "lp fc", 20, 4196, 249)); //setting the defaults to one above/below. When they're selected, they're set to 249/2000
    addParameter(hpFqParam = new juce::AudioParameterFloat("HighPass transition Frequency", " hp fc", 20, 8000, 2001));



    //vector<Band> bands;
    //bands.push_back(Band(1));
    
    
    

    

}

EQupidAudioProcessor::~EQupidAudioProcessor()
{
}

//==============================================================================
const juce::String EQupidAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EQupidAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EQupidAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EQupidAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EQupidAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EQupidAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EQupidAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EQupidAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EQupidAudioProcessor::getProgramName (int index)
{
    return {};
}

void EQupidAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EQupidAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
}

void EQupidAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EQupidAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EQupidAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    calcAlgorithmParams();
    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    

    for (int samp = 0; samp < buffer.getNumSamples(); samp++) {
        channelDataLeft[samp] = mhsL.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mhsR.tick(channelDataRight[samp]);

        channelDataLeft[samp] = mlsL.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mlsR.tick(channelDataRight[samp]);

        channelDataLeft[samp] = mnotchL.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mnotchR.tick(channelDataRight[samp]);

        if (lpSelected == true) {
            channelDataLeft[samp] = mlpL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mlpR.tick(channelDataRight[samp]);
        }
        
        if (hpSelected == true) {
            channelDataLeft[samp] = mhpL.tick(channelDataLeft[samp]);
            channelDataRight[samp] = mhpR.tick(channelDataLeft[samp]);
        }
        
    }
}



//==============================================================================
bool EQupidAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EQupidAudioProcessor::createEditor()
{
    return new EQupidAudioProcessorEditor (*this);
}

//==============================================================================
void EQupidAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EQupidAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void EQupidAudioProcessor::calcAlgorithmParams()
{
    //lowshelf and highshelf
    float ls = lsParam->get(); //gain then frequency
    float lsfq = lsFqParam->get();
    float hs = hsParam->get();
    float hsfq = hsFqParam->get();


    //lowpass and highpass
    float lpq = lpParam->get(); //q then frequency
    float lpfq = lpFqParam->get();
    float hpq = hpParam->get();
    float hpfq = hpFqParam->get();
    

    //checking to see if lp and hp were selected
    if (lpfq == 250) { //bass freq. 
        lpSelected = true;
    }

    if (hpfq == 2000) { //higher midrange
        hpSelected = true;
    }

    //notch
    float notchgain = notchParam->get();
    float notchfq = notchFqParam->get();
    float notchq = notchQParam->get();
    

    //coefficients for each parameter
    float coeffsLs[5];
    float coeffsHs[5];

    float coeffsLp[5];
    float coeffsHp[5];

    float coeffsNotch[5];

    //-----------------------------------------------calculating all of the coefficients

    Mu45FilterCalc::calcCoeffsLowShelf(coeffsLs, lsfq, ls, mFs);
    mlsL.setCoefficients(coeffsLs[0], coeffsLs[1], coeffsLs[2], coeffsLs[3], coeffsLs[4]);
    mlsR.setCoefficients(coeffsLs[0], coeffsLs[1], coeffsLs[2], coeffsLs[3], coeffsLs[4]);
    
    Mu45FilterCalc::calcCoeffsHighShelf(coeffsHs, hsfq, hs, mFs);
    mhsL.setCoefficients(coeffsHs[0], coeffsHs[1], coeffsHs[2], coeffsHs[3], coeffsHs[4]);
    mhsR.setCoefficients(coeffsHs[0], coeffsHs[1], coeffsHs[2], coeffsHs[3], coeffsHs[4]);

    Mu45FilterCalc::calcCoeffsLPF(coeffsLp, lpfq, lpq, mFs);
    mlpL.setCoefficients(coeffsLp[0], coeffsLp[1], coeffsLp[2], coeffsLp[3], coeffsLp[4]);
    mlpR.setCoefficients(coeffsLp[0], coeffsLp[1], coeffsLp[2], coeffsLp[3], coeffsLp[4]);
    

    Mu45FilterCalc::calcCoeffsHPF(coeffsHp, hpfq, hpq, mFs);
    mhpL.setCoefficients(coeffsHp[0], coeffsHp[1], coeffsHp[2], coeffsHp[3], coeffsHp[4]);
    mhpR.setCoefficients(coeffsHp[0], coeffsHp[1], coeffsHp[2], coeffsHp[3], coeffsHp[4]);


    Mu45FilterCalc::calcCoeffsPeak(coeffsNotch, notchfq, notchgain, notchq, mFs);
    mnotchL.setCoefficients(coeffsNotch[0], coeffsNotch[1], coeffsNotch[2], coeffsNotch[3], coeffsNotch[4]);
    mnotchR.setCoefficients(coeffsNotch[0], coeffsNotch[1], coeffsNotch[2], coeffsNotch[3], coeffsNotch[4]);

    /**
     DBG("ls = " << ls);
    DBG("hs = " << hs);
    DBG("notchgain = " << notchgain);
    DBG("notchq = " << notchq);
    */
   
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EQupidAudioProcessor();
}
