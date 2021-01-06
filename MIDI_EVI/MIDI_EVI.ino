
#include <MIDI.h>
#define MIDI_PITCHBEND_MAX   8191
#define MIDI_PITCHBEND_MIN   -8192

/*TODO
   Stop retriggering of notes
   Include CC breath message (every update when not 0, unlike noteon/off)
   Pitch bend/mod wheel
*/



//Valves (turn this into an array dummy)

int currentNote;
int lastNoteOn;

//Breath sensor
const int breath = A0;
int intensity = 0;
int last = 0;
int reading;
int sensorIni;
const int breathPin = A0;

//Joystick

const int X = A1;
const int Y = A2;
int Xval;
int Xtemp;
int Yval;
int Ytemp;
bool pitchGate = false;





ValveBlock valves;








// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  //sets up valve buttons  pinMode(noteOn, INPUT_PULLUP);
  lastNoteOn = 0;
  delay(200);
  //calibrator for breath sensor
  sensorIni = analogRead(breathPin);
  MIDI.begin(MIDI_CHANNEL_OFF);
}


/*Functions:

   getMIDINote() - returns whatever note is fingered
*/
void loop() {
  // put your main code here, to run repeatedly:

  reading = constrain(analogRead(breathPin) - sensorIni, 0, 800);
  intensity = map(reading, 0, 800, 0, 127);
  currentNote = valves.getMIDINote();

    //No breath, no note
    if (intensity == 0 && intensity != last){
      MIDI.sendNoteOff(lastNoteOn, 0, 1);
      //Serial.println("Note off");
      lastNoteOn = -1;
    }

  //Breath? Note
  //Only sends a value if it has changed and is different from 0)
  if ((intensity > 0 && intensity != last) ) {
    if (currentNote != lastNoteOn) {
      MIDI.sendNoteOn(currentNote, intensity, 1);
      MIDI.sendNoteOff(lastNoteOn, 0, 1);
      //  Serial.println("Note on: velocity " + intensity);
      lastNoteOn = currentNote;
    }
    //Send Breath message
    MIDI.sendControlChange(2, intensity, 1);
  }
  last = intensity;


  // Pitch bend
  Ytemp = analogRead(Y);
/*Find center value (=- deadzone), init? (515-522)max top = 701 (top off) min = 240
 * If it's in deadzone, Y = 0. Don't send repeated 0s, deadzone boolean?
 * If value is above center, then Y = map(where deadzone ends to max, 1 - 8191
 * If it's below, Y = map (min to where deadzone begins, -8192 - -1
 * Use gate to not send excessive 0 messages
 */

if (Ytemp >= 506 && Ytemp <= 522) {
  Yval = 0;
  //Only send pitch bend if the last value was not 0
  if (pitchGate = false) {
    MIDI.sendPitchBend (0, 1);
    pitchGate = true;
  }
}

else if (Ytemp > 522) {
  Yval = map(Ytemp, 523, 701, 1, MIDI_PITCHBEND_MAX);
  //Send pitch bend
  MIDI.sendPitchBend (Yval, 1);
  pitchGate = false;
}

else if (Ytemp < 506) {
  Yval = map(Ytemp, 240, 505, MIDI_PITCHBEND_MIN, -1);
  //Send pitch bend
  MIDI.sendPitchBend (Yval, 1);
  pitchGate = false;
}



  
  delay(30);


}
