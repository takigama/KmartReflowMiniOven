#pragma once

#define TOUCH_CS -1 // clear off touch pin
#define DISPLAY_ENABLED
#define DISPLAY_TYPE 1  // we ony have 1 - tft on t-display




// thermistor stuff
#define N_TEMP_SENSORS 1
#define THERMISTOR_PIN_1 32 // pin for reading voltage
#define REFERENCE_RESISTANCE_1   4700
#define NOMINAL_RESISTANCE_1     100000
#define NOMINAL_TEMPERATURE_1    25
#define B_VALUE_1                3950
#define TEMP_IDEAL_MAX_1           400
#define TEMP_IDEAL_MIN_1         10
#define ESP32_ADC_VREF_MV       3300


#define SSR_PIN 15 // PWM pin for controlling SSR
#define BUTTON_1_PIN 35
#define BUTTON_2_PIN 0

// #define ESP32
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <esp_adc_cal.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <LittleFS.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>

// Internal global things
extern int but1;
extern int but2;
extern int dcycle;
extern WiFiManager wm;

// SSR control parameters
#define LEDC_CHANNEL 0 // use channel 0
#define LEDC_FREQ 60 // 100hz frequence
#define LEDC_RESOLUTION 10 // Set
#define dcMAX 1024 // at 10 bits dcMAX is 1024
#define dcMIN 0