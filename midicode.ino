#include <MIDIUSB.h>
//No rotary encoders because i ran out of time

const int numButtons = 14;
const int buttonPins[numButtons] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 22, 23};
int buttonState[numButtons];
int lastButtonState[numButtons];

const int numPots = 14;
const int potPins[numPots] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13};
int potValue[numPots];
int lastPotValue[numPots];

void setup() {
  //For loop to initialise all the buttons so i don't have to write them out one by one
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttonState[i] = digitalRead(buttonPins[i]);
    lastButtonState[i] = buttonState[i];
  }
//same for the pots
  for (int i = 0; i < numPots; i++) {
    potValue[i] = analogRead(potPins[i]);
    lastPotValue[i] = potValue[i];
  }
}

void loop() {
  for (int i = 0; i < numButtons; i++) {
    buttonState[i] = digitalRead(buttonPins[i]);
    if (buttonState[i] != lastButtonState[i]) {
      if (buttonState[i] == LOW) {
        noteOn(0, 60 + i, 127);  // Note On
      } else {
        noteOff(0, 60 + i, 0);   // Note Off
      }
      lastButtonState[i] = buttonState[i];
    }
  }

  for (int i = 0; i < numPots; i++) {
    potValue[i] = analogRead(potPins[i]);
    if (abs(potValue[i] - lastPotValue[i]) > 4) {
      int mappedValue = map(potValue[i], 0, 1023, 0, 127);
      controlChange(0, i + 1, mappedValue); 
      lastPotValue[i] = potValue[i];
    }
  }

  MidiUSB.flush();
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  //Function called by draw loop incase any button/pot is detected to have moved
  MidiUSB.sendMIDI(noteOn);
}
