#include "ButtonModule.h"

#include "esp32-hal-gpio.h"
#include <Arduino.h>
#include <stdio.h>

#define BUTTON_INPUT_IO (6)  // This is GPIO D4 on the ESP32C3

boolean buttonWasLow = false;         // variable flag for when the pushbutton goes low

void ButtonModule::init() {
  pinMode(BUTTON_INPUT_IO, INPUT_PULLDOWN);
}

void ButtonModule::monitorState() {
  // read the state of the pushbutton and set a flag if it is low:
  if (digitalRead(BUTTON_INPUT_IO) == LOW)  {
      buttonWasLow = true;
  }

  // This if statement will only fire on the rising edge of the button input
  if (digitalRead(BUTTON_INPUT_IO) == HIGH && buttonWasLow)  {
      // reset the button low flag
      buttonWasLow = false;

      // Button event here
      Serial.println("Button pressed..");
  }
}
