#pragma once

#define THERMISTOR_1_TYPE 1 // we'll pull the types from marlin later
#define LEDC_PIN 2 // pin we control SSR from
#define TOUCH_CS -1 // clear off touch pin
#define DISPLAY_ENABLED true
#define DISPLAY_TYPE 1  // we ony have 1 - tft on t-display

// #define ESP32
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <esp_adc_cal.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <LittleFS.h>
#include <WiFiManager.h>

// Internal global things
extern int but1;
extern int but2;
extern int dcycle;
extern WiFiManager wm;

// SSR control parameters
#define LEDC_CHANNEL 0 // use channel 0
#define LEDC_FREQ 100 // 100hz frequence
#define LEDC_RESOLUTION 10 // Set
#define dcMAX 1024 // at 10 bits dcMAX is 1024
#define dcMIN 0