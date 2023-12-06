#include "ButtonModule.h"

#include "esp32-hal-gpio.h"
#include <Arduino.h>
#include <stdio.h>

#define BUTTON_INPUT_IO (6)             // This is GPIO D4 on the ESP32C3

boolean buttonWasHigh = false;          // variable flag for when the pushbutton goes high
int prevButtonState = LOW;
unsigned long buttonPressedTime = 0;    // the last time the button was pressed (ms)
unsigned long debounceDelay = 50;       // the debounce time; increase if the output flickers (ms)
unsigned long longPressDuration = 1000; // duration required to qualify as long press (ms)

void ButtonModule::init() {
  pinMode(BUTTON_INPUT_IO, INPUT_PULLDOWN);
}

void ButtonModule::onShortPress(ButtonPressCallback callback) {
  shortPressCallback = callback;
}

void ButtonModule::onLongPress(ButtonPressCallback callback) {
  longPressCallback = callback;
}

void ButtonModule::monitorState() {
  // read the state of the pushbutton
  int buttonState = digitalRead(BUTTON_INPUT_IO);

  // Check if button is just pressed
  if (prevButtonState == LOW && buttonState == HIGH)  {
      prevButtonState = HIGH;
      buttonPressedTime = millis();
  }

  // Check if button is just released
  if (prevButtonState == HIGH && buttonState == LOW) {
    prevButtonState = LOW;
    unsigned long buttonPressDuration = millis() - buttonPressedTime;
  
    // Dont do anything if shorter press than debounce delay
    if (buttonPressDuration < debounceDelay) {
      return;
    }

    // Long press
    if (buttonPressDuration > longPressDuration) {
      if (longPressCallback) {
        longPressCallback();
      }
    }
    // Short press
    else {
      if (shortPressCallback) {
        shortPressCallback();
      }
    }
  }
}
