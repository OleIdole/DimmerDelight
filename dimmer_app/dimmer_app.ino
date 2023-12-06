#include <Arduino.h>
#include "WifiModule.h"
#include "DimmerModule.h"
#include "ButtonModule.h"
#include "config.h"

#include <SinricPro.h>
#include <SinricProLight.h>

WifiModule wifiModule;
DimmerModule dimmerModule;
ButtonModule buttonModule;

// create and add a light to SinricPro
SinricProLight &myLight = SinricPro[sinric_switch_id];

// TODO: make button press use currentBrightness and send brightness/power event to myLight as well when changing intensity/on/off
// TODO: make button longpress cycle between 10, 50 and 100 percent and update currentBrightness

bool onPowerStateCb(const String &deviceId, bool &state) {
  Serial.printf("device %s turned %s\r\n", deviceId.c_str(), state ? "on" : "off");
  if (state == true) {
    int currentBrightness = dimmerModule.getCurrentBrightness();
    dimmerModule.setLightIntensity(currentBrightness);  // Default on-state is 10% brightness
    myLight.sendBrightnessEvent(currentBrightness); // Notify the SinricPro about this brightness value being used
  } else {
    dimmerModule.setLightIntensity(0);
  }
  return true;  // indicate that callback handled correctly
}

bool onBrightnessCb(const String &deviceId, int brightness) {
  Serial.printf("Device %s brightness level set to %d\r\n", deviceId.c_str(), brightness);
  dimmerModule.setLightIntensity(brightness);
  return true;
}

void onShortButtonPressCb() {
  // Handle short press
  Serial.println("Short press");
}

void onLongButtonPressCb() {
  // Handle long press
  Serial.println("Long press");
}

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Booting device..");

  wifiModule.init();
  dimmerModule.init(10); // Set initial brightness percentage
  buttonModule.init();
  buttonModule.onShortPress(onShortButtonPressCb);
  buttonModule.onLongPress(onLongButtonPressCb);

  // set callback function
  myLight.onPowerState(onPowerStateCb);
  myLight.onBrightness(onBrightnessCb);

  // setup SinricPro
  SinricPro.onConnected([]() {
    Serial.printf("Connected to SinricPro\r\n");
  });
  SinricPro.onDisconnected([]() {
    Serial.printf("Disconnected from SinricPro\r\n");
  });
  SinricPro.begin(sinric_app_key, sinric_app_secret);
}

void loop() {
  if (wifiModule.isConnected()) {
    // WiFi is connected, do something
  } else {
    // WiFi is not connected, handle accordingly
  }

  SinricPro.handle();
  buttonModule.monitorState();
}
