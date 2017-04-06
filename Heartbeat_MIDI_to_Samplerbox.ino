/*
    MIDI to Samplerbox
    Triggers sequences for heartbeat sensor at midi note 60
    Further sound samples are triggered at random,
    with probability increasing with heartbeat rate.

*/

/*
   TODO - Answer these questions
   What is the range of heartbeat BPMs?
     My guess is anywhere from 60 to 80?
   What are all of the notes and what range from 60+ do we want to have ready?
     Let's set around 10 extra samples, so 60 = heartbeat sound, 61 - 71 = samples.
   What is the behaviour that changes with a higher BPM?
     Perhaps the rate of new samples triggered?

*/

#include "MIDIUSB.h"

//  VARIABLES
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeTimer = 0;                 // used to fade LED on with PWM on fadePin


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

void setup() {
  Serial.begin(115200);
  pinMode(blinkPin, OUTPUT);        // blinks to heartbeat
  pinMode(fadePin, OUTPUT);         // fades to heartbeat
  interruptSetup();                 // Read Pulse Sensor signal every 2mS
  midiSetup();

  delay(5000);
  noteOn(0, 42, 60);
  MidiUSB.flush();
}

const int NOTE_NUM = 120;
unsigned long notesArray[NOTE_NUM];
boolean pulseOn = false;
const long interval = 8000;           // interval at which to blink (milliseconds)

void loop() {
  if (QS && BPM > 65 && BPM < 100) {
    if (Pulse && !pulseOn) {
      noteOn(0, 60, 64);   // Channel 0, middle C, normal velocity
      pulseOn = true;
    } else if (!Pulse && pulseOn) {
      noteOff(0, 60, 64);  // Channel 0, middle C, normal velocity
      pulseOn = false;

    }
    MidiUSB.flush();
  }

  ledFadeToBeat();
  randomSample(BPM);

  delay(20);                             //  take a break
}


void midiSetup() {
  for (int i = 0; i < NOTE_NUM; i++) {
    notesArray[i] = (0); // notesArray holds the last millis when it was triggered or 0
  }
}

void randomSample(int bpm) {
  unsigned long currentMillis = millis();
  
  if (QS && bpm > 50 && bpm < 150) {
    // trigger a random sample
    int probability = random(0, bpm);
    probability = constrain(probability, 0, 120);
    int r = random(0, NOTE_NUM);
    
    if (notesArray[r] == 0 && probability > 60) { // if the note isnt being played and chance roll
//      Serial.print("playing " + r);
      notesArray[r] == currentMillis;
      noteOn(0, 61+r, 127);
      MidiUSB.flush();
    }
  }

  // time interval check
  for (int i = 0; i < NOTE_NUM; i++ ) {
    // if the note has a number in it, check how long it has been since it started playing and if it's past a certain interval then send note off.
    if (currentMillis - notesArray[i] >= interval) {
//      Serial.print("stopping " + i);
      noteOff(0, (61+i), 127);
      MidiUSB.flush();
      notesArray[i] = 0;
    }
  }
}


