#include <WiFi.h>
#include "WifiModule.h"
#include "config.h"

void WifiModule::init() {
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

bool WifiModule::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}
