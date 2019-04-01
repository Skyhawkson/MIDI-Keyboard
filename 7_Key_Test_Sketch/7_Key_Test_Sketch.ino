/*
   This examples shows how to make a simple seven keys MIDI keyboard with volume control

   Created: 4/10/2015
   Author: Arturo Guadalupi <a.guadalupi@arduino.cc>
   
   http://www.arduino.cc/en/Tutorial/MidiDevice
*/

#include "MIDIUSB.h"
//#include "PitchToNote.h"
#define C8  108

#define B7  107
#define B7b 106
#define A7  105
#define A7b 104
#define G7  103
#define G7b 102
#define F7  101
#define E7  100
#define E7b 99
#define D7  98
#define D7b 97
#define C7  96

#define B6  95
#define B6b 94
#define A6  93
#define A6b 92
#define G6  91
#define G6b 90
#define F6  89
#define E6  88
#define E6b 87
#define D6  86
#define D6b 85
#define C6  84

#define B5  83
#define B5b 82
#define A5  81
#define A5b 80
#define G5  79
#define G5b 78
#define F5  77
#define E5  76
#define E5b 75
#define D5  74
#define D5b 73
#define C5  72

#define B4  71
#define B4b 70
#define A4  69
#define A4b 68
#define G4  67
#define G4b 66
#define F4  65
#define E4  64
#define E4b 63
#define D4  62
#define D4b 61
#define C4  60

#define B3  59
#define B3b 58
#define A3  57
#define A3b 56
#define G3  55
#define G3b 54
#define F3  53
#define E3  52
#define E3b 51
#define D3  50
#define D3b 49
#define C3  48

#define B2  47
#define B2b 46
#define A2  45
#define A2b 44
#define G2  43
#define G2b 42
#define F2  41
#define E2  40
#define E2b 39
#define D2  38
#define D2b 37
#define C2  36

#define B1  35
#define B1b 34
#define A1  33
#define A1b 32
#define G1  31
#define G1b 30
#define F1  29
#define E1  28
#define E1b 27
#define D1  26
#define D1b 25
#define C1  24

#define B0  23
#define B0b 22
#define A0  21
#define NUM_BUTTONS  7

const uint8_t button1 = 2;
const uint8_t button2 = 3;
const uint8_t button3 = 4;
const uint8_t button4 = 5;
const uint8_t button5 = 6;
const uint8_t button6 = 7;
const uint8_t button7 = 8;

const int intensityPot = 0;  //A0 input

const uint8_t buttons[NUM_BUTTONS] = {button1, button2, button3, button4, button5, button6, button7};
const byte notePitches[NUM_BUTTONS] = {C3, D3, E3, F3, G3, A3, B3}; //not originally quoted, should this be in PitchToNote?

uint8_t notesTime[NUM_BUTTONS];
uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;
uint8_t intensity;

void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);
}


void loop() {
  readButtons();
  readIntensity();
  playNotes();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      bitWrite(pressedButtons, i, 1);
      delay(50);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void readIntensity()
{
  //int val = analogRead(intensityPot);
  //intensity = (uint8_t) (map(val, 0, 1023, 0, 127));
  intensity = 70;
}

void playNotes()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i , 1);
        noteOn(0, notePitches[i], intensity);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousButtons, i , 0);
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
