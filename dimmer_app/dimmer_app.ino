#include <Arduino.h>
#include "WifiModule.h"
#include "DimmerModule.h"
#include "config.h"

WifiModule wifiModule;
DimmerModule dimmerModule;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Booting device..");

  wifiModule.init();
  dimmerModule.init();
}

void loop() {
  if (wifiModule.isConnected()) {
    // WiFi is connected, do something
  } else {
    // WiFi is not connected, handle accordingly
  }

  // When get a command to dim light, use line below:
  // dimmerModule.setLightIntensity(50);
}
