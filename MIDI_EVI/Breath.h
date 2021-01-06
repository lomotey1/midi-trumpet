class Breath {
  private:
    const int breathPin = A0;
    int intensity = 0;
    int last = 0;
    int reading;
    int sensorIni;
    bool changed;
    bool 


    public:
    Breath();


    void update() {
      reading = constrain(analogRead(breathPin) - sensorIni, 0, 800);
  intensity = map(reading, 0, 800, 0, 127);

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
    }
    
}
