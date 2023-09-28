#include <Arduino.h>
#include "WifiModule.h"
#include "config.h"

WifiModule wifiModule;

void setup() {
    wifiModule.init();
}

void loop() {
    // Your main application code here
    if (wifiModule.isConnected()) {
        // WiFi is connected, do something
    } else {
        // WiFi is not connected, handle accordingly
    }
}
