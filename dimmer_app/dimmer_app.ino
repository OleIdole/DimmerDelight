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

// Initial brightness for dimmer to use
int initialBrightness = 10;

// create and add a light to SinricPro
SinricProLight &myLight = SinricPro[sinric_switch_id];

bool onPowerStateCb(const String &deviceId, bool &state) {
  Serial.printf("device %s turned %s\r\n", deviceId.c_str(), state ? "on" : "off");
  if (state == true) {
    int brightness = dimmerModule.getBrightness();
    dimmerModule.setLightIntensity(brightness);  // Default on-state is 10% brightness
    myLight.sendBrightnessEvent(brightness); // Notify the SinricPro about this brightness value being used
  } else {
    dimmerModule.setLightIntensity(0);
  }
  return true;  // indicate that callback handled correctly
}

bool onBrightnessCb(const String &deviceId, int brightness) {
  //Serial.printf("Device %s brightness level set to %d\r\n", deviceId.c_str(), brightness);
  dimmerModule.setLightIntensity(brightness);
  return true;
}

void onShortButtonPressCb() {
  // Handle short press
  if (dimmerModule.isLightOn) {
    // Turn off light
    Serial.printf("Turning off light\r\n");
    dimmerModule.setLightIntensity(0);
    myLight.sendPowerStateEvent(false);
  }
  else {
    // Turn on light with previous brightness value
    int brightness = dimmerModule.getBrightness();
    Serial.printf("Turning on light\r\n");
    dimmerModule.setLightIntensity(brightness);
    myLight.sendPowerStateEvent(true);
    myLight.sendBrightnessEvent(brightness); // Notify the SinricPro about this brightness value being used
  }
}

void onLongButtonPressCb() {
  // Handle long press
  int brightness = dimmerModule.getBrightness();
  if (brightness == 100) {
    // Loop back to 10% brightness
    dimmerModule.setLightIntensity(10);
    myLight.sendBrightnessEvent(10);
  }
  else if (brightness > 90) {
    // Set brightness to 100%
    dimmerModule.setLightIntensity(100);
    myLight.sendBrightnessEvent(100);
  }
  else {
    // Increment brightness by 10%
    dimmerModule.setLightIntensity(brightness + 10);
    myLight.sendBrightnessEvent(brightness + 10);
  }
}

void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.printf("Booting device..\r\n");

  wifiModule.init();
  dimmerModule.init(initialBrightness);
  buttonModule.init();
  buttonModule.onShortPress(onShortButtonPressCb);
  buttonModule.onLongPress(onLongButtonPressCb);

  // set callback function
  myLight.onPowerState(onPowerStateCb);
  myLight.onBrightness(onBrightnessCb);

  // setup SinricPro
  SinricPro.onConnected([]() {
    Serial.printf("Connected to SinricPro\r\n");
    myLight.sendBrightnessEvent(initialBrightness); // Notify the SinricPro about this brightness value being used
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
