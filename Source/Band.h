/*
  ==============================================================================

    Band.h
    Created: 6 Dec 2021 12:38:03pm
    Author:  Hana Kovaci

  ==============================================================================
*/

#pragma once

#ifndef BAND_H
#define BAND_H

#include <JuceHeader.h>
#include "StkLite-4.6.1/Biquad.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"

using namespace juce;
using namespace std;


class Band {

public:
    //based on a listener, a new band will be created and subsequently, a new audioparameter as well.
    //The band can't just be a band tho, the user will have to also select the band type
    //so i'll probably just have the one button that's like create a band and then it'll prompt the user for a certain band

    Band();
    Band(int typeOfBand); //creates a band according to the band type-switch statement put in place to initialize the rest of the variables
    ~Band(); 


    int bandType; //number that corresponds with a band type, 1 low pass, 2 high pass, 2 lowshelf, 3 highshelf 4 peak 5 allpass
    string bandName; // the name of the band corresponds with what we'll be using in the new audioParameterFloat method
    float defaultVal; //this all rides on the bandType
    float minVal;
    float maxVal;
    float Q[3] = { 0.1, 3, 0.6 }; // only matters in the case of peak so we're just putting the Q values in here;
    //AudioParameterFloat * freq; //controls the transitive frequency of the band
    //AudioParameterFloat* gain; //controls the gain of the band


    //Slider freqSlider; //frequency will be a linear slider
    //Slider gainSlider; //gain slider will be a rotary slider
    //Label freqLabel; //freq label
    //Label gainLabel; //gain label


private:

};



#endif // !BAND_H

