#include "ValveBlock.h"

ValveBlock::ValveBlock() {
  for (int i = 0; i < 8; i++) {
    pinMode(i + 2, INPUT_PULLUP); //makes it register as high
  }
}
int ValveBlock::getMIDINote() {
  int drop = 0;
  int midinote = 48 - 2; //Low C is 48, change transposition here

  //Choose the partial with valves 4 and 5 here. This base MIDI note should be a variable to allow transposition/octaves
  //partial select
  if (valves[4].isPressed()) {
    if (valves[5].isPressed()) {
      midinote += 5;
    }
    midinote += 7;
  }
  else {
    if (valves[5].isPressed()) {
      midinote += 19;
    }
  }

  //Valve 0 just adds, valves 1-3 drop pitches similarly to real brass instruments but use a switch case to add to the midi number

  if (valves[0].isPressed()) {
    midinote += 2;
  }

  //Pitch altering

  if (valves[1].isPressed()) {
    drop -= 2;
  }

  if (valves[2].isPressed()) {
    drop -= 1;
  }

  if (valves[3].isPressed()) {
    drop -= 3;
  }

  //lower half of the octave

  if ( (valves[4].isPressed() && valves[5].isPressed()) || (!valves[4].isPressed() && !valves[5].isPressed()) ) {
    switch (drop) {
      case -6:
        midinote += 1;
        break;
      case -5:
        midinote += 2;
        break;
      case -4:
        midinote += 3;
        break;
      case -3:
        midinote += 4;
        break;
      case -2:
        midinote += 5;
        break;
      case -1:
        midinote += 6;
        break;
      default:
        break;
    }

  }
  else {
    //higher half of the octave
    switch (drop) {
      case -4:
        midinote += 1;
        break;
      case -3:
        midinote += 2;
        break;
      case -2:
        midinote += 3;
        break;
      case -1:
        midinote += 4;
        break;
      default:
        break;
    }
  }
  return midinote;
}
};
