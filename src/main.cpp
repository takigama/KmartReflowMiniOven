
#include "config.h"
#include "Display.h"
#include "Button.h"
#include "WebInterface.h"

WiFiManager wm; // Wifimanager is such an awesome project

int dcycle = 0; // our duty cucle

bool restartBeforeBind = false; // hack to get around incompatability between wifimanager and ESPAsyncWebserver

// TODO: REMOVE THESE LATER
#include <ElegantOTA.h>
// ENDTODO

void saveConfigCallback () {
  Serial.println("Should save config - lets hope the config was saved before we... restart");
  restartBeforeBind = true;
}

void setup(void)
{
  Serial.begin(115200);

  pinMode(SSR_PIN, LEDC_PIN);    // SSR PWM
  pinMode(BUTTON_1_PIN, INPUT);       // button 0
  pinMode(BUTTON_2_PIN, INPUT_PULLUP); // button 1

  int timeoutWifi = 0;

  initDisplay();
  updateDisplay(0, 0);

  LittleFS.begin(true);

  bool res;
  wm.setSaveConfigCallback(saveConfigCallback); // to work around wmmanager not releasing port 80
  res = wm.autoConnect("ReflowOven"); // Unprotected ap for config

  if (!res)
  {
    Serial.println("Failed to to WiFi connect");
    ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("WiFi Connected");
  }

  initWebServer();

  Serial.println("About to begin()");
  if(restartBeforeBind) {
    Serial.println("Have to restart as work around for wmanager");
    ESP.restart();
  }
  WebServerBegin();
  Serial.println("HTTP server started");

  // TODO: REMOVE LATER
  ElegantOTA.begin(&server);
  // ENDTODO

  ledcSetup(LEDC_CHANNEL, LEDC_FREQ, LEDC_RESOLUTION);
  ledcAttachPin(LEDC_PIN, LEDC_CHANNEL);
}

void loop()
{

  int v = 3;
  while (1)
  {
    yield(); // We must yield() to stop a watchdog timeout but i've forgotten where this yield comes from

    // TODO: REMOVE LATER
    ElegantOTA.loop();
    // ENDTODO

    // TODO move buttons to ISR's?
    pollButtons(); 
  }
}