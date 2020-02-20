#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMem1;       //xy=124,119
AudioPlayMemory          playMem5;       //xy=124,403
AudioPlayMemory          playMem4;       //xy=129,320
AudioPlayMemory          playMem3;       //xy=141,245
AudioPlayMemory          playMem2;       //xy=151,178
AudioPlayMemory          playMem6;       //xy=173,449
AudioMixer4              mixer1;         //xy=414,226
AudioMixer4              mixer2;         //xy=422,298
AudioOutputI2S           i2s1;           //xy=576,224
AudioConnection          patchCord1(playMem1, 0, mixer1, 0);
AudioConnection          patchCord2(playMem5, 0, mixer2, 0);
AudioConnection          patchCord3(playMem4, 0, mixer1, 3);
AudioConnection          patchCord4(playMem3, 0, mixer1, 2);
AudioConnection          patchCord5(playMem2, 0, mixer1, 1);
AudioConnection          patchCord6(playMem6, 0, mixer2, 1);
AudioConnection          patchCord7(mixer1, 0, i2s1, 0);
AudioConnection          patchCord8(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=543,127
// GUItool: end automatically generated code

#include "AudioSampleKick.h"
#include "AudioSampleBass.h"
#include "AudioSampleSnare.h"
#include "AudioSampleHihat.h"
#include "AudioSampleDoum.h"
#include "AudioSampleTak.h"

#define triggerThreshold 600 // If this is set too low, hits on other pads will trigger a "hit" on this pad
#define initialHitReadDuration 500 // In microseconds. Shorter times will mean less latency, but less accuracy. 500 microseconds is nothing, anyway

#define inputPin A20
#define inputPin2 A12
#define inputPin3 A18
#define inputPin4 A17
#define inputPin5 A16
#define inputPin6 A14




#define F_CPU 96000000
#define LAYOUT_US_ENGLISH

 
// Getting the ideal balance of these two constants will ensure that fast subsequent hits are perceived accurately, but false hits are not generated
#define subsequentHitThreshold 1.7
#define subsequentHitThresholdDecaySpeed 14
 
uint16_t highestYet;
uint16_t highestYet2;
uint16_t highestYet3;
uint16_t highestYet4;
uint16_t highestYet5;
uint16_t highestYet6;

uint32_t startReadingTime;
uint32_t highestValueTime;
boolean hitOccurredRecently = false;
 
void setup() 
{
  Serial.begin(9600);
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(10);
}



  
// Compares times without being prone to problems when the micros() counter overflows, every ~70 mins
boolean timeGreaterOrEqual(uint32_t lhs, uint32_t rhs) {
  return (((lhs - rhs) & 2147483648) == 0);
}



 
void loop() {
 
  // Assume the normal hit-threshold
  uint16_t thresholdNow = triggerThreshold;



 
  // But, if a hit occurred very recently, we need to set a higher threshold for triggering another hit, otherwise the dissipating vibrations
  // of the previous hit would trigger another one now
  if (hitOccurredRecently) {
  
      // Work out how high a reading we'd need to see right now in order to conclude that another hit has occurred
      uint16_t currentDynamicThreshold = (highestYet >> ((micros() - highestValueTime) >> subsequentHitThresholdDecaySpeed)) * subsequentHitThreshold;
 
      // If that calculated threshold is now as low as the regular threshold, we can go back to just waiting for a regular, isolated hit
      if (currentDynamicThreshold <= triggerThreshold) hitOccurredRecently = false;
 
      // Otherwise, do use this higher threshold
      else thresholdNow = currentDynamicThreshold;
  }



 
  // Read the piezo
  uint16_t value = analogRead(inputPin);
  uint16_t value2 = analogRead(inputPin2);
  uint16_t value3 = analogRead(inputPin3);
  uint16_t value4 = analogRead(inputPin4);
  uint16_t value5 = analogRead(inputPin5);
  uint16_t value6 = analogRead(inputPin6);
 
  // If we've breached the threshold, it means we've got a hit!
  if (value >= thresholdNow) {
    startReadingTime = micros();
    highestYet = 0;
 
    // For the next few milliseconds, look out for the highest "spike" in the reading from the piezo. Its height is representative of the hit's velocity
    do {
      if (value > highestYet) {
        highestYet = value;
        highestValueTime = micros();
      }
      value = analogRead(inputPin);
    } while (timeGreaterOrEqual(startReadingTime + initialHitReadDuration, micros()));

  // Send the MIDI note
      playMem1.play(AudioSampleKick);
      Serial.print("1:");Serial.println(value);
      hitOccurredRecently = true;
  }



// If we've breached the threshold, it means we've got a hit!
  if (value2 >= thresholdNow) {
    startReadingTime = micros();
    highestYet = 0;
 
    // For the next few milliseconds, look out for the highest "spike" in the reading from the piezo. Its height is representative of the hit's velocity
    do {
      if (value2 > highestYet) {
        highestYet = value2;
        highestValueTime = micros();
      }
      value2 = analogRead(inputPin2);
    } while (timeGreaterOrEqual(startReadingTime + initialHitReadDuration, micros()));

  // Send the MIDI note
      playMem2.play(AudioSampleBass);
      Serial.print("2:");Serial.println(value2);
      hitOccurredRecently = true;
  }



// If we've breached the threshold, it means we've got a hit!
  if (value3 >= thresholdNow) {
    startReadingTime = micros();
    highestYet = 0;
 
    // For the next few milliseconds, look out for the highest "spike" in the reading from the piezo. Its height is representative of the hit's velocity
    do {
      if (value3 > highestYet) {
        highestYet = value3;
        highestValueTime = micros();
      }
      value3 = analogRead(inputPin3);
    } while (timeGreaterOrEqual(startReadingTime + initialHitReadDuration, micros()));

  // Send the MIDI note
      playMem3.play(AudioSampleSnare);
      Serial.print("3:");Serial.println(value3);
      hitOccurredRecently = true;
  }  



  // If we've breached the threshold, it means we've got a hit!
  if (value4 >= thresholdNow) {
    startReadingTime = micros();
    highestYet = 0;
 
    // For the next few milliseconds, look out for the highest "spike" in the reading from the piezo. Its height is representative of the hit's velocity
    do {
      if (value4 > highestYet) {
        highestYet = value4;
        highestValueTime = micros();
      }
      value4 = analogRead(inputPin4);
    } while (timeGreaterOrEqual(startReadingTime + initialHitReadDuration, micros()));

  // Send the MIDI note
      playMem4.play(AudioSampleHihat);
      Serial.print("4:");Serial.println(value4);
      hitOccurredRecently = true;
  }  



  // If we've breached the threshold, it means we've got a hit!
  if (value5 >= thresholdNow) {
    startReadingTime = micros();
    highestYet = 0;
 
    // For the next few milliseconds, look out for the highest "spike" in the reading from the piezo. Its height is representative of the hit's velocity
    do {
      if (value5 > highestYet) {
        highestYet = value5;
        highestValueTime = micros();
      }
      value5 = analogRead(inputPin5);
    } while (timeGreaterOrEqual(startReadingTime + initialHitReadDuration, micros()));

  // Send the MIDI note
      playMem5.play(AudioSampleDoum);
      Serial.print("5:");Serial.println(value5);
      hitOccurredRecently = true;
  }  



  // If we've breached the threshold, it means we've got a hit!
  if (value6 >= thresholdNow) {
    startReadingTime = micros();
    highestYet = 0;
 
    // For the next few milliseconds, look out for the highest "spike" in the reading from the piezo. Its height is representative of the hit's velocity
    do {
      if (value6 > highestYet) {
        highestYet = value6;
        highestValueTime = micros();
      }
      value6 = analogRead(inputPin6);
    } while (timeGreaterOrEqual(startReadingTime + initialHitReadDuration, micros()));

  // Send the MIDI note
      playMem6.play(AudioSampleTak);
      Serial.print("6:");Serial.println(value6);
      hitOccurredRecently = true;
  } 
}
