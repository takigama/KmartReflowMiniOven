#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <esp_adc_cal.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>

#define LEDC_CHANNEL 0
#define LEDC_PIN 15
#define LEDC_FREQ 2
#define LEDC_RESOLUTION 10 // Set
#define dcMAX 1024
#define dcMIN 0

int dcycle = 0;

// void setup()
// {

// }

// void loop()
// {

// }


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

// TODO: borrowed from elegantota - fix
WebServer server(80);
unsigned long ota_progress_millis = 0;
const char* ssid = "........";
const char* password = "........";

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}
// ENDTODO

// #define TFT_GREY 0x5AEB // New colour

void setup(void) {
  Serial.begin(115200);

  // TODO: borrowed from elegantota - fix
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.on("/", []() {
    server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  server.begin();
  // ENDTODO

  tft.init();
  tft.setRotation(1);

  ledcSetup(LEDC_CHANNEL, LEDC_FREQ, LEDC_RESOLUTION);
  ledcAttachPin(LEDC_PIN, LEDC_CHANNEL);


  pinMode(15, OUTPUT); // SSR PWM
  pinMode(35, INPUT); // button 0
  pinMode(0, INPUT_PULLUP); // button 1
}

void loop() {
  
  // Fill screen with grey so we can see the effect of printing with and without 
  // a background colour defined
  tft.fillScreen(TFT_BLACK);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.println("Hello World!");
  
  // Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_YELLOW); tft.setTextFont(2);
  tft.println(1234.56);
  
  // Set the font colour to be red with black background, set to font 4
  tft.setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
  tft.println((long)3735928559, HEX); // Should print DEADBEEF

  // Set the font colour to be green with black background, set to font 2
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextFont(2);
  tft.println("Groop");

  // Test some print formatting functions
  float fnumber = 123.45;
   // Set the font colour to be blue with no background, set to font 2
  tft.setTextColor(TFT_BLUE);    tft.setTextFont(2);
  tft.print("Float = "); tft.println(fnumber);           // Print floating point number
  tft.print("Binary = "); tft.println((int)fnumber, BIN); // Print as integer value in binary
  tft.print("Hexadecimal = "); tft.println((int)fnumber, HEX); // Print as integer number in Hexadecimal

  int v=3;
  int but1 = 0;
  int but2 = 0;
  while(1) {
    yield(); // We must yield() to stop a watchdog timeout.
    int secs = millis()/1000;
    // Serial.println(secs%2);
    if((secs%2)==0) {
      if(v!=0) {
        v=0;
        Serial.println("secs 1");
        digitalWrite(15, LOW);
      }
    } else {
      if(v!=1) {
        Serial.println("secs 2");
        v = 1;
        digitalWrite(15, HIGH);
      }
    }

    // but 1
    if(digitalRead(0)==0) {
      if(but1 != 0) {
        Serial.println("But1 released");
        Serial.printf("v1: %d, v2, %d", digitalRead(35),digitalRead(0));
        Serial.println();
        dcycle -= 50;
        if(dcycle > dcMAX) dcycle=dcMAX;
        ledcWrite(LEDC_CHANNEL, dcycle);
        Serial.printf("dcycle now %d",dcycle);
        Serial.println("");
        but1 = 0;
      }
    } else {
      if(but1 != 1) {
        Serial.println("But1 pressed");
        Serial.printf("v1: %d, v2, %d", digitalRead(35),digitalRead(0));
        Serial.println();
        but1 = 1;
      }
    }

    // but 2
    if(digitalRead(35)==0) {
      if(but2 != 0) {
        Serial.println("But2 released");
        Serial.printf("v1: %d, v2, %d", digitalRead(35),digitalRead(0));
        Serial.println();
        but2 = 0;
        dcycle += 50;
        if(dcycle < dcMIN) dcycle=dcMIN;
        Serial.printf("dcycle now %d",dcycle);
        Serial.println("");
        ledcWrite(LEDC_CHANNEL, dcycle);
      }
    } else {
      if(but2 != 1) {
        Serial.println("But2 pressed");
        Serial.printf("v1: %d, v2, %d", digitalRead(35),digitalRead(0));
        Serial.println();
        but2 = 1;
      }
    }
  }
  ElegantOTA.loop();
}