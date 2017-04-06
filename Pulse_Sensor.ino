/*
    >>> Pulse Sensor purple wire goes to Analog Pin 0 <<<
  Pulse Sensor sample aquisition and processing happens in the background via Timer 2 interrupt. 2mS sample rate.
  PWM on pins 3 and 11 will not work when using this code, because we are using Timer 2!
  The following variables are automatically updated:
  Signal :    int that holds the analog signal data straight from the sensor. updated every 2mS.
  IBI  :      int that holds the time interval between beats. 2mS resolution.
  BPM  :      int that holds the heart rate value, derived every beat, from averaging previous 10 IBI values.
  QS  :       boolean that is made true whenever Pulse is found and BPM is updated. User must reset.
  Pulse :     boolean that is true when a heartbeat is sensed then false in time with pin13 LED going out.

  Pin 13 LED will blink with heartbeat.

  Basedon work by Joel Murphy & Yury Gitman  Fall 2012
  This update changes the HRV variable name to IBI, which stands for Inter-Beat Interval, for clarity.
  Switched the interrupt to Timer2.  500Hz sample rate, 2mS resolution IBI value.
  Fade LED pin moved to pin 5 (use of Timer2 disables PWM on pins 3 & 11).
  Tidied up inefficiencies since the last version.
*/
int fadeRate = 200;

void ledFadeToBeat() {
  fadeRate -= 15;                         //  set LED fade value
  fadeRate = constrain(fadeRate, 0, 255); //  keep LED fade value from going into negative numbers!
  analogWrite(fadePin, fadeRate);         //  fade LED
}

//
//void sendDataToProcessing(char symbol, int data ){
//    Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
//    Serial.println(data);                // the data to send culminating in a carriage return
//  }







