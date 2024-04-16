#ifndef BUZZ_H
#define BUZZ_H

#include <Arduino.h>

#define BUZZ_PIN 25 // Define the pin for the buzzer

// Function to play a dangerous sounding tone on the buzzer
void playDangerousTone() {
  // Define the frequency for the dangerous tone
  int toneFrequency = 2000; // Adjust this value to change the pitch
  
    // Turn on the buzzer
    analogWrite(BUZZ_PIN, 255);
    // Play the tone
    tone(BUZZ_PIN, toneFrequency);
    // Wait for the beep duration
    delay(100);
    // Turn off the buzzer
    analogWrite(BUZZ_PIN, 0);
    // Pause between beeps
    delay(100);

}

// Function to play a dangerous sounding tone on the buzzer
void playDangerousTone2() {
  // Define the frequency for the dangerous tone
  int toneFrequency = 1000; // Adjust this value to change the pitch
  
    // Turn on the buzzer
    analogWrite(BUZZ_PIN, 255);
    // Play the tone
    tone(BUZZ_PIN, toneFrequency);
    // Wait for the beep duration
    delay(2000);
    // Turn off the buzzer
    analogWrite(BUZZ_PIN, 0);
    // Pause between beeps
    delay(100);

}
// Function to buzz the buzzer with a single short beep
void buzzMe() {
  int toneFrequency = 1500; // Adjust this value to change the pitch
  analogWrite(BUZZ_PIN, 255);
  tone(BUZZ_PIN, 1500);
  delay(300);
  analogWrite(BUZZ_PIN, 100);
   tone(BUZZ_PIN, 1000);
  delay(150);
  analogWrite(BUZZ_PIN, 0);
   noTone(BUZZ_PIN);
}

// Function to buzz the buzzer with a single short beep
void buzzMe2() {
  int toneFrequency = 1500; // Adjust this value to change the pitch
  analogWrite(BUZZ_PIN, 255);
  tone(BUZZ_PIN, 1500);
  delay(300);
  analogWrite(BUZZ_PIN, 0);
   noTone(BUZZ_PIN);
}
// Function to buzz the buzzer with a longer beep pattern
void buzzMe1() {
  int toneFrequency = 3000;
  analogWrite(BUZZ_PIN, 255);
  tone(BUZZ_PIN, 1500);
  delay(100);
  analogWrite(BUZZ_PIN, 100);
  tone(BUZZ_PIN, 1000);
  delay(100);
  analogWrite(BUZZ_PIN, 0);
  noTone(BUZZ_PIN);
  analogWrite(BUZZ_PIN, 255);
  tone(BUZZ_PIN, 1500);
  delay(100);
  analogWrite(BUZZ_PIN, 100);
  tone(BUZZ_PIN, 1000);
  delay(100);
  analogWrite(BUZZ_PIN, 0);
  noTone(BUZZ_PIN);
}

// Function to stop the buzzer tone
void stopBuzzer() {
  // Turn off the buzzer
  analogWrite(BUZZ_PIN, 0);
  // If you were using tone() function to generate the sound, you need to stop it as well
  noTone(BUZZ_PIN);
}
#endif // BUZZ_H
