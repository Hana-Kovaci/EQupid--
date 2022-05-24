/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace std;

//==============================================================================
EQupidAudioProcessorEditor::EQupidAudioProcessorEditor (EQupidAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 500);

    auto& params = processor.getParameters();    


    /*
    //yeah,,, so that band class didn't work out. Too many issues getting it to properly integrate itself in with the processor. 
    // A lot of stuff that was beyond the scope of my programming knowledge was also at play :/
    //I probably would have been able to fix it if I understood the nuances of c++ (esp newer versions) more but it was much too confusing
    //in order to create a semblance of what i first had, I've decided to swap out extra windows with comboboxes that determine whether Sliders are visible and in what order they should appear on the application
    //In addition, to limit the graphics of this project going berzerk, I've limited the user to viewing only 5 UNIQUE bands at once
        //earlier, it would have been 5 or 6 bnds of any kind but with the lack of a band class, it's just not in my reach
    */
    
       
    //ls slider
    AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
    ls.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    ls.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 18);
    ls.setRange(audioParam->range.start, audioParam->range.end);
    ls.setValue(*audioParam);
    ls.addListener(this);
    ls.setTextValueSuffix("dB");


    audioParam = (AudioParameterFloat*)params.getUnchecked(1);
    hs.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    hs.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 18);
    hs.setRange(audioParam->range.start, audioParam->range.end);
    hs.setValue(*audioParam);
    hs.addListener(this);
    hs.setTextValueSuffix("dB");


    audioParam = (AudioParameterFloat*)params.getUnchecked(2);
    notch.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    notch.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 18);
    notch.setRange(audioParam->range.start, audioParam->range.end);
    notch.setValue(*audioParam);
    notch.addListener(this);
    notch.setTextValueSuffix("dB");


    audioParam = (AudioParameterFloat*)params.getUnchecked(3);
    notchQ.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    notchQ.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 18);
    notchQ.setRange(audioParam->range.start, audioParam->range.end);
    notchQ.setValue(*audioParam);   
    notchQ.addListener(this);
    notchQ.setTextValueSuffix("dB");


    audioParam = (AudioParameterFloat*)params.getUnchecked(4);
    lpq.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    lpq.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 18);
    lpq.setRange(audioParam->range.start, audioParam->range.end);
    lpq.setValue(*audioParam);
    lpq.addListener(this);


    audioParam = (AudioParameterFloat*)params.getUnchecked(5);
    hpq.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    hpq.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 18);
    hpq.setRange(audioParam->range.start, audioParam->range.end);
    hpq.setValue(*audioParam);
    hpq.addListener(this);


//-------------------------------- frequency parameters
    audioParam = (AudioParameterFloat*)params.getUnchecked(6);
    lsfreq.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    lsfreq.setTextBoxStyle(Slider::TextBoxBelow, true, 120, 20);
    lsfreq.setRange(audioParam->range.start, audioParam->range.end);
    lsfreq.setValue(*audioParam);
    lsfreq.addListener(this);
    lsfreq.setSkewFactor(0.4); //easier honing in on lower freqs
    lsfreq.setTextValueSuffix("hz");

    audioParam = (AudioParameterFloat*)params.getUnchecked(7);
    hsfreq.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    hsfreq.setTextBoxStyle(Slider::TextBoxBelow, true, 120, 20);
    hsfreq.setRange(audioParam->range.start, audioParam->range.end);
    hsfreq.setValue(*audioParam);
    hsfreq.addListener(this);
    hsfreq.setSkewFactor(0.75); //working more with higher freqs so less of a skew
    hsfreq.setTextValueSuffix("hz");

    audioParam = (AudioParameterFloat*)params.getUnchecked(8);
    notchfreq.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    notchfreq.setTextBoxStyle(Slider::TextBoxBelow, true, 120, 20);
    notchfreq.setRange(audioParam->range.start, audioParam->range.end);
    notchfreq.setValue(*audioParam);
    notchfreq.addListener(this);
    notchfreq.setTextValueSuffix("hz");

    audioParam = (AudioParameterFloat*)params.getUnchecked(9);
    lpfreq.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    lpfreq.setTextBoxStyle(Slider::TextBoxBelow, true, 120, 20);
    lpfreq.setRange(audioParam->range.start, audioParam->range.end);
    lpfreq.setValue(*audioParam);
    lpfreq.addListener(this);
    lpfreq.setSkewFactor(0.4); //easier honing in on lower freqs
    lpfreq.setTextValueSuffix("hz");

    audioParam = (AudioParameterFloat*)params.getUnchecked(10);
    hpfreq.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    hpfreq.setTextBoxStyle(Slider::TextBoxBelow, true, 120, 20);
    hpfreq.setRange(audioParam->range.start, audioParam->range.end);
    hpfreq.setValue(*audioParam);
    hpfreq.addListener(this);
    hpfreq.setSkewFactor(0.75); //working more with higher freqs so less of a skew
    hpfreq.setTextValueSuffix("hz");

    //--------------------------------------------------------------------------------------------
    //adding options to our bandSelection
  

    addAndMakeVisible(bandSelection);
    bandSelection.setText("Add a band...", juce::dontSendNotification);
    bandSelection.addItem("LowPass Filter", lpass);
    bandSelection.addItem("HighPass Filter", hpass);
    bandSelection.addItem("LowShelf Filter", lshelf);
    bandSelection.addItem("HighShelf FIlter", hshelf);
    bandSelection.addItem("Notch", peak);
    bandSelection.setBounds(350, 425, 200, 50);
    bandSelection.onChange = [this] {bandSelectionMade(); };

   

    //--------------------------------------------------------------------------------------------------
    //going back to add some labels
    
    lsLabel.setText("LowShelf", juce::dontSendNotification);
    lsLabel.attachToComponent(&lsfreq, false);

    hsLabel.setText("HighShelf", juce::dontSendNotification);
    hsLabel.attachToComponent(&hsfreq, false);

    notchLabel.setText("Notch", juce::dontSendNotification);
    notchLabel.attachToComponent(&notchfreq, false);

    lpLabel.setText("LowPass", juce::dontSendNotification);
    lpLabel.attachToComponent(&lpfreq, false);

    hpLabel.setText("HighPass", juce::dontSendNotification);
    hpLabel.attachToComponent(&hpfreq, false);
}


EQupidAudioProcessorEditor::~EQupidAudioProcessorEditor()
{
}

//==============================================================================
void EQupidAudioProcessorEditor::paint (juce::Graphics& g)
{
    //look and feel of our GUI window------------------------------------------
        // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::lightpink);
    g.setColour (juce::Colours::white);
    getLookAndFeel().setColour(Slider::ColourIds::backgroundColourId, Colours::whitesmoke);
    getLookAndFeel().setColour(Slider::ColourIds::trackColourId, Colours::whitesmoke);
    getLookAndFeel().setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::whitesmoke);
    getLookAndFeel().setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::whitesmoke);
    getLookAndFeel().setColour(Slider::ColourIds::thumbColourId, Colours::powderblue);
    getLookAndFeel().setColour(ComboBox::ColourIds::backgroundColourId, Colours::powderblue);
    getLookAndFeel().setColour(ComboBox::ColourIds::outlineColourId, Colours::whitesmoke);
    g.setFont (15.0f);
    //text to write!---------------------------------------------------------------
    g.drawFittedText("Equpid 1.1 <3", getLocalBounds(), Justification::centredBottom, 1);
    g.setFont(25.0f);

    //Text
    g.drawFittedText("Gain | Notch", 20, 25, 150, 25, Justification::verticallyCentred, 1);
    g.drawFittedText("Transition Frequency", 300, 25, 300, 25, Justification::verticallyCentred, 1);
    g.drawRoundedRectangle(10, 10, 580, 50, 5, 3);
    
    
    
   /* g.drawFittedText("LowShelf fc", getLocalBounds(), Justification::verticallyCentred, 1);
    g.drawFittedText("HighShelf fc", hs.getBounds(), Justification::horizontallyCentred, 1);
    g.drawFittedText("peak fc", notch.getBounds(), Justification::horizontallyCentred, 1);
    g.drawFittedText("Q", notchQ.getBounds(), Justification::horizontallyCentred, 1);*/


    //adding in our svg!!!!-----------------------------------------------------
    unique_ptr<XmlElement> svg_xml_1(XmlDocument::parse(BinaryData::EqupidLogo_svg)); // GET THE SVG AS A XML https://christiangaertner.github.io/uhh-digitalaudioprocessing/ui/2018/02/15/svgs.html
    unique_ptr<Drawable> cupid;
    AffineTransform affine; //not sure what the affine transform does but since I'm not altering my images it shouldn't matter 

    cupid = Drawable::createFromSVG(*svg_xml_1); 
    cupid->setTransformToFit(Rectangle<float>(50, 400, 200, 100), RectanglePlacement::stretchToFit);
    cupid->draw(g, 1.f, affine = AffineTransform()); //drawing the svg!

   
   
    

}

void EQupidAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void EQupidAudioProcessorEditor::sliderValueChanged(Slider* slider) //getting rid of the listener/broadcaster format
{
    auto& params = processor.getParameters();

    if (slider == &ls) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = ls.getValue();
        DBG("ls slider changed");
    }
    else if (slider == &hs) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = hs.getValue();
        DBG("hs slider changed");
    }
    else if (slider == &notch) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = notch.getValue();
        DBG("notch slider changed");
    }
    else if (slider == &notchQ) {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = notchQ.getValue();
        DBG("Q slider changed");
    }
    else if (slider == &lpq) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(4);
        *audioParam = lpq.getValue();
        DBG("lp slider changed");
    }
    else if (slider == &hpq) {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(5);
        *audioParam = hpq.getValue();
        DBG("hp slider changed");
    }
      ///-- now we're onto frequency sliders
    else if (slider == &lsfreq) {
            juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(6);
            *audioParam = lsfreq.getValue();
            DBG("lsfreq slider changed");
        }
        else if (slider == &hsfreq) {
            juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(7);
            *audioParam = hsfreq.getValue();
            DBG("hsfreq slider changed");
        }
        else if (slider == &notchfreq) {
            juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(8);
            *audioParam = notchfreq.getValue();
            DBG("notchfreq slider changed");
        }
        else if (slider == &lpfreq) {
            juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(9);
            *audioParam = lpfreq.getValue();
            DBG("lpfreq slider changed");
        }
        else if (slider == &hpfreq) {
            juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(10);
            *audioParam = hpfreq.getValue();
            DBG("hp freqslider changed");
        }
}
