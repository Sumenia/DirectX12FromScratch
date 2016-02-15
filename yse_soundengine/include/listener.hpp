/*
  ==============================================================================

    listener.h
    Created: 30 Jan 2014 4:22:19pm
    Author:  yvan

  ==============================================================================
*/


#ifndef LISTENER_H_INCLUDED
#define LISTENER_H_INCLUDED

#include "headers/defines.hpp"
#include "utils/vector.hpp"

namespace YSE {
    
    /**
     The Listener is a singleton object used to control your position
     in the virtual space. With sound positions and listener position 
     (and rotation) you control how the sound output is spread out over
     the available speakers.
     
     Also the listener keeps track of its own velocity, ie. the speed at 
     which the object is moving. This is used in doppler calculations.
    */
  class API listener {
  public:

    Vec getPosition(); //< Get the current position of the listener.
    Vec getVelocity(); //< Get the current velocity of the listener. This is a calculated value. Velocity cannot be set manually.
    Vec getForward(); //< Get the 'forward' orientation of the listener.
    Vec getUpward(); //< Get the 'upward' orientation of the listener.
      
    /**
     Set the current position of the listener. If you want to use doppler
     and velocity, you should update the position of the listener at a frequent
     interval. Usually you will update the position just as often as you use 
     System().update().
    */
    listener& setPosition(const Vec &pos);
      
    /**
     Set the orientation of the listener. The upward vector is optional, and assumes 
     rotation on a horizontal plane by default.
    */
    listener& setOrientation(const Vec &forward, const Vec &up = Vec(0, 1, 0));

  };

  /**
    Functor to retrieve the singleton listener object.
  */
  API listener & Listener();
}



#endif  // LISTENER_H_INCLUDED
