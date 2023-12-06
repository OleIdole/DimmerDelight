#include "DimmerModule.h"

#include <Arduino.h>
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO (5)  // This is GPIO D3 on the ESP32C3
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT  // Set duty resolution to 13 bits
#define LEDC_DUTY_MAX (8191)             // Max duty cycle = ((2 ** 13) - 1) * 100% = 8191
#define LEDC_FREQUENCY (4000)            // Frequency in Hertz. Set frequency at 4 kHz

void DimmerModule::init(int initialBrightness) {
  // Prepare and then apply the LEDC PWM timer configuration
  ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_MODE,
    .duty_resolution = LEDC_DUTY_RES,
    .timer_num = LEDC_TIMER,
    .freq_hz = LEDC_FREQUENCY,  // Set output frequency
    .clk_cfg = LEDC_AUTO_CLK
  };
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  // Prepare and then apply the LEDC PWM channel configuration
  ledc_channel_config_t ledc_channel = {
    .gpio_num = LEDC_OUTPUT_IO,
    .speed_mode = LEDC_MODE,
    .channel = LEDC_CHANNEL,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER,
    .duty = 0,  // Set duty to 0%
    .hpoint = 0
  };
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

  setLightIntensity(initialBrightness);
}

void DimmerModule::setLightIntensity(int percentage) {
  if (percentage >= 0 && percentage <= 100) {
    // Convert percentage to duty cycle, hardcoded for min and max percentage to prevent rounding errors
    int duty_cycle;
    if (percentage == 0){
      duty_cycle = 0;
      // Not setting brightness for 0 percentage, want to keep last non-zero value for when turning on again
    }
    else if (percentage == 100){
      duty_cycle = LEDC_DUTY_MAX;
      setCurrentBrightness(percentage);
    }
    else {
      duty_cycle = (int)(LEDC_DUTY_MAX * percentage / 100);
      setCurrentBrightness(percentage);
    }

    // Set duty on-time
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty_cycle));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
  } else {
    Serial.println("ERROR: light intensity out of range 0 to 100");
  }
}

int DimmerModule::getCurrentBrightness() {
  return currentBrightness;
}

// Private method to set the brightness
void DimmerModule::setCurrentBrightness(int percentage) {
  if (percentage >= 0 && percentage <= 100) {
    currentBrightness = percentage;
    char buffer[50]; // Buffer to hold the formatted string
    snprintf(buffer, sizeof(buffer), "Light set to %d %%", currentBrightness);
    Serial.println(buffer);
  }
}

