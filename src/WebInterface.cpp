#include "WebInterface.h"
#include "Temperature.h"

AsyncWebServer server(80); // our webserver

void initWebServer()
{
  // server = new AsyncWebServer(80);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->redirect("/index.html"); });

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("restarting");
              request->send(200, "text/plain", "Restarting...");
              ESP.restart(); });

  server.on("/gettemp1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Get Temp 1 Called");
              char tempString[64];
              snprintf(tempString, 64, "%f\n", getTemp());
              Serial.println(tempString);
              request->send(200, "text/plain", tempString); });

  server.on("/forgetwifi", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Forgetting Wifi");
              request->send(200, "text/plain", "Restarting...");
              wm.resetSettings();
              ESP.restart(); });

  server.serveStatic("/index.html", LittleFS, "/index.html");
  server.serveStatic("/css", LittleFS, "/css");
  server.serveStatic("/js", LittleFS, "/js");

  // thi sshould probably be a put or a post
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
    request->send(200, "text/plain", "DutySetSuccess"); });
}

void WebServerBegin()
{
  server.begin();
}