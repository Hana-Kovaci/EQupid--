/*
  ==============================================================================

    Band.cpp
    Created: 6 Dec 2021 12:38:03pm
    Author:  Hana Kovaci

  ==============================================================================
*/

#include "Band.h"

Band::Band()
{
    bandType = 0;
    bandName = "NULL";
    defaultVal = 0;
    minVal = 0;
    maxVal = 0;
    //freq = new AudioParameterFloat(bandName+"freq", bandName+"gain", minVal, maxVal, defaultVal);
    //gain = new AudioParameterFloat(bandName + "gain", bandName + "gain", -18, 18, 0);
   
}

Band::Band(int typeOfBand)
{
    switch (typeOfBand) {
    case 1 : //lp filter
    {
        bandType = typeOfBand;
        bandName = "LowPass";
        defaultVal = 160;
        minVal = 20;
        maxVal = 4000;
        //not sure if we wanna instantiate the audioparameter floats here or not;
        break;
    }
    case 2 : //hp filter
    {
        bandType = typeOfBand;
        bandName = "HighPass";
        defaultVal = 160;
        minVal = 20;
        maxVal = 4000;
        //not sure if we wanna instantiate the audioparameter floats here or not;
        break;
    }
    case 3 : //lshelf filter
    {
        bandType = typeOfBand;
        bandName = "LowShelf";
        defaultVal = 160;
        minVal = 20;
        maxVal = 4000;
        //not sure if we wanna instantiate the audioparameter floats here or not;
        break;
    }
    case 4 : //hshelf filter
    {
        bandType = typeOfBand;
        bandName = "HighShelf";
        defaultVal = 160;
        minVal = 20;
        maxVal = 4000;
        //not sure if we wanna instantiate the audioparameter floats here or not;
        break;
    }
    case 5: //peak
    {
        bandType = typeOfBand;
        bandName = "Peak";
        minVal = Q[0];
        maxVal = Q[1];
        defaultVal = Q[2];
        //not sure if we wanna instantiate the audioparameter floats here or not;
        break;
    }

    }

    //freq = new AudioParameterFloat(bandName + "freq", bandName + "freq", minVal, maxVal, defaultVal);
    //gain = new AudioParameterFloat(bandName + "gain", bandName + "gain", -18, 18, 0);
    
}

Band::~Band()
{
    //delete freq;
    //delete gain;
}
