/*
  ==============================================================================

    sineWave.h
    Created: 31 Jan 2014 2:56:47pm
    Author:  yvan

  ==============================================================================
*/

#ifndef SINEWAVE_H_INCLUDED
#define SINEWAVE_H_INCLUDED

#include "../dspObject.hpp"
#include "../oscillators.hpp"
#include "../drawableBuffer.hpp"

namespace YSE {
  namespace DSP {

    class API sineWave : public dspSourceObject {
    public:
      // object is thread safe
      sineWave();

      // change the frequency instantly, or wait for the next note on status
      void      frequency(Flt value);
      Flt       frequency();

      virtual void process(SOUND_STATUS & intent, Int & latency); // use only during DSP

    private:
      sine sineGen;
      YSE::DSP::drawableBuffer volumeCurve;

      YSE::DSP::drawableBuffer frequencyCurve;
      aFlt parmFrequency;
      Flt currentFrequency;
    };

  }
}




#endif  // SINEWAVE_H_INCLUDED
