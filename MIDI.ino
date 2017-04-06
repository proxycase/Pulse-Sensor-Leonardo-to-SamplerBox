//int[] notesArray;
//// could have a velocity array here if I wanted...
//
//void midiSetup() {
//  notesArray = new int[2];
//  for (int i = 0; i < notesArray.length; i++) {
//    notesArray[i] = (60 + i); // Starts MIDI channel assignment from note 60
//  }
//}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

//void controlChange(byte channel, byte control, byte value) {
//  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
//  MidiUSB.sendMIDI(event);
//}

void midiUpdate() {
  Serial.println("Sending note on");
  noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(500);
  Serial.println("Sending note off");
  noteOff(0, 48, 64);  // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(1500);

  // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65
}

// These will be heartbeats regardless
void sendBeatOn() {
  midiEventPacket_t noteOn = {0x09, 0x90 | (byte) 0, (byte) 60, (byte) 64};
  MidiUSB.sendMIDI(noteOn);
}

void sendBeatOff() {
  midiEventPacket_t noteOff = {0x08, 0x80 | (byte) 0, (byte) 60, (byte) 64};
  MidiUSB.sendMIDI(noteOff);
}
