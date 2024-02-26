#include <Arduino.h>

const uint8_t BUTTON_PIN = 0;   // Button connected to GPIO 0
const uint8_t RED_PIN = 16;     // Red LED pin
const uint8_t GREEN_PIN = 17;   // Green LED pin
const uint8_t BLUE_PIN = 18;    // Blue LED pin

volatile bool buttonPressed = false;
volatile bool isFirstPress = true;
volatile bool isBlinking = false;

void IRAM_ATTR buttonISR() {
  buttonPressed = true;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RED_PIN, HIGH);   // Turn off LEDs initially
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void loop() {
  if (buttonPressed) {
    if (isFirstPress) {
      digitalWrite(RED_PIN, LOW);   // Turn on red LED
      isFirstPress = false;
    } else {
      isBlinking = !isBlinking;     // Toggle blinking state
      if (isBlinking) {
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, HIGH); // Turn off blue LED initially
        delay(100);                   // Delay for stability
      } else {
        // Button pressed again after blink, restart
        isFirstPress = true;
        digitalWrite(RED_PIN, HIGH);   // Turn off all LEDs
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, HIGH);
      }
    }
    buttonPressed = false;           // Reset button press flag
  }

  if (isBlinking) {
    // Blink blue LED
    digitalWrite(BLUE_PIN, !digitalRead(BLUE_PIN));
    delay(57);                      // Blink interval
  }
}
