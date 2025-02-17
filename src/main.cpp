
#include "config.h"
#include "Display.h"
#include "Button.h"
#include "WebServer.h"

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

  pinMode(15, LEDC_PIN);    // SSR PWM
  pinMode(35, INPUT);       // button 0
  pinMode(0, INPUT_PULLUP); // button 1

  int timeoutWifi = 0;

  initDisplay();
  updateDisplay(0, 0);

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
    yield(); // We must yield() to stop a watchdog timeout.

    // TODO: REMOVE LATER
    ElegantOTA.loop();
    // ENDTODO

    pollButtons();

    
  }
}