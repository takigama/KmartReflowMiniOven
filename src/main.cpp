#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <esp_adc_cal.h>

#include <WiFiManager.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#define LEDC_CHANNEL 0
#define LEDC_PIN 2
#define LEDC_FREQ 100
#define LEDC_RESOLUTION 10 // Set
#define dcMAX 1024
#define dcMIN 0

// TODO: REMOVE THESE LATER
#include <ElegantOTA.h>
// ENDTODO

WiFiManager wm;

int dcycle = 0;

TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h

AsyncWebServer server(80);
bool restartBeforeBind = false;

void saveConfigCallback () {
  Serial.println("Should save config - lets hope the config was saved before we... restart");
  restartBeforeBind = true;
}

void updateDisplay() {
    // Fill screen with grey so we can see the effect of printing with and without
  // a background colour defined
  tft.fillScreen(TFT_BLACK);

  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  if(dcycle ==0) tft.println("Off");
  else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Running!");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  tft.printf("Duty Cycle: %d",dcycle);
  tft.println();
  tft.printf("Temperture: %d", 105);
}

void setup(void)
{
  Serial.begin(115200);

  pinMode(15, LEDC_PIN);    // SSR PWM
  pinMode(35, INPUT);       // button 0
  pinMode(0, INPUT_PULLUP); // button 1

  int timeoutWifi = 0;

  LittleFS.begin(true);

  // TODO WIFI MANAGER

  bool res;
  wm.setSaveConfigCallback(saveConfigCallback);
  res = wm.autoConnect("ReflowOven"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
    ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  // ENDTODO

  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  //   if ((timeoutWifi++) > 20)
  //   {
  //     Serial.println("Wifi timout");
  //     break;
  //   }
  // }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->redirect("/index.html"); });

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("restarting");
              request->send(200, "text/plain", "Restarting...");
              ESP.restart();
            });

  server.on("/forgetwifi", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Forgetting Wifi");
              request->send(200, "text/plain", "Restarting...");
              wm.resetSettings();
              ESP.restart();
            });

  server.serveStatic("/index.html", LittleFS, "/index.html");
  server.serveStatic("/css", LittleFS, "/css");
  server.serveStatic("/js", LittleFS, "/js");

  server.on("/setduty", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // display params
    size_t count = request->params();
    for (size_t i = 0; i < count; i++) {
      const AsyncWebParameter *p = request->getParam(i);
      Serial.printf("PARAM[%u]: \"%s\" = \"%s\"\n", i, p->name().c_str(), p->value().c_str());
      Serial.println("");
    }

    // get duty param
    String duty;
    int dutyc;
    if (request->hasParam("duty")) {
      duty = request->getParam("duty")->value();
      dutyc = duty.toInt();
      Serial.printf("Have duty of \"%s\", which is \"%d\" and \"%d\" in cycles",duty, dutyc, ((dutyc*dcMAX)/100));
      Serial.println("");
      if(dutyc < 0 || dutyc > 100) {
        Serial.println("Dutyc out of range");
      }
      else 
      {
        Serial.printf("dutyc from web: %d", dutyc);
        Serial.println("");
        dcycle = ((dutyc*dcMAX)/100);
        Serial.printf("Setting dcycle to \"%d\"", dcycle);
        Serial.println("");
        ledcWrite(LEDC_CHANNEL, dcycle);
      }
    } else {
      Serial.println("No duty in params");
    }
    Serial.println("giving up on duty");
    request->send(200, "text/plain", "Hello!"); });

  Serial.println("About to begin()");
  if(restartBeforeBind) {
    Serial.println("Have to restart as work around for wmanager");
    ESP.restart();
  }
  server.begin();
  Serial.println("HTTP server started");

  // TODO: REMOVE LATER
  ElegantOTA.begin(&server);
  // ENDTODO

  tft.init();
  tft.setRotation(1);

  ledcSetup(LEDC_CHANNEL, LEDC_FREQ, LEDC_RESOLUTION);
  ledcAttachPin(LEDC_PIN, LEDC_CHANNEL);
}

void loop()
{


  int v = 3;
  int but1 = 0;
  int but2 = 0;
  while (1)
  {
    yield(); // We must yield() to stop a watchdog timeout.

    // TODO: REMOVE LATER
    ElegantOTA.loop();
    // ENDTODO

    int secs = millis() / 1000;
    // Serial.println(secs%2);
    if ((secs % 2) == 0)
    {
      if (v != 0)
      {
        v = 0;
        // Serial.println("secs 1");
        // digitalWrite(15, LOW);
      }
    }
    else
    {
      if (v != 1)
      {
        // Serial.println("secs 2");
        v = 1;
        // digitalWrite(15, HIGH);
      }
    }

    // but 1
    if (digitalRead(0) == 0)
    {
      if (but1 != 0)
      {
        Serial.println("But1 released");
        Serial.printf("v1: %d, v2, %d", digitalRead(35), digitalRead(0));
        Serial.println();
        dcycle -= 50;
        if (dcycle < dcMIN)
          dcycle = dcMIN;
        ledcWrite(LEDC_CHANNEL, dcycle);
        Serial.printf("dcycle now %d", dcycle);
        Serial.println("");
        but1 = 0;
        updateDisplay();
      }
    }
    else
    {
      if (but1 != 1)
      {
        Serial.println("But1 pressed");
        Serial.printf("v1: %d, v2, %d", digitalRead(35), digitalRead(0));
        Serial.println();
        but1 = 1;
      }
    }

    // but 2
    if (digitalRead(35) == 0)
    {
      if (but2 != 0)
      {
        Serial.println("But2 released");
        Serial.printf("v1: %d, v2, %d", digitalRead(35), digitalRead(0));
        Serial.println();
        but2 = 0;
        dcycle += 50;
        if (dcycle > dcMAX)
          dcycle = dcMAX;
        Serial.printf("dcycle now %d", dcycle);
        Serial.println("");
        ledcWrite(LEDC_CHANNEL, dcycle);
        updateDisplay();
      }
    }
    else
    {
      if (but2 != 1)
      {
        Serial.println("But2 pressed");
        Serial.printf("v1: %d, v2, %d", digitalRead(35), digitalRead(0));
        Serial.println();
        but2 = 1;
      }
    }
  }
}