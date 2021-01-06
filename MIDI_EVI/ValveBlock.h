#ifndef MY_VALVEBLOCK_H
#define MY_VALVEBLOCK_H
#include <Arduino.h>
#include "Button.h"

class ValveBlock {
  private:
    //Change pins here if needed, also change constructor
    Button valves[8] = {
      Button(2), //v0 - v5
      Button(3),
      Button(4),
      Button(5),
      Button(6),
      Button(7),
      Button(8), //octave keys
      Button(9),
    };
  public:
    ValveBlock();

    int getMIDINote();
};

#endif
