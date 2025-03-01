#include "WebInterface.h"
#include "AggregateTemperature.h"
#include "TemperatureProfile.h"

// bit of an ugly way to do this.. but close enough
extern AggregateTemperature tempController;
AsyncWebServer server(80); // our webserver
char endarg[32];

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
              snprintf(tempString, 64, "%f\n", tempController.getCelcius());
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

  server.on("/engageprofile", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)

            {
              // we expect to get a JSON object that contains a temperature profile
              Serial.println("engage profile");
              size_t count = request->params();
              for (size_t i = 0; i < count; i++) {
                const AsyncWebParameter *p = request->getParam(i);
                Serial.printf("PARAM[%u]: \"%s\" = \"%s\"\n", i, p->name().c_str(), p->value().c_str());
                Serial.println("");
              }
              tempProfileProcessJson();
              if(!index){
    Serial.printf("BodyStart: %u B\n", total);
  }
  for(size_t i=0; i<len; i++){
    Serial.write(data[i]);
  }
  if(index + len == total){
    Serial.printf("BodyEnd: %u B\n", total);
  }
              request->send(200, "text/plain", "engagein5seconds"); });

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
        strcpy(endarg,"DutyRangeFail");
      }
      else 
      {
        Serial.printf("dutyc from web: %d", dutyc);
        Serial.println("");
        dcycle = ((dutyc*dcMAX)/100);
        Serial.printf("Setting dcycle to \"%d\"", dcycle);
        Serial.println("");
        ledcWrite(LEDC_CHANNEL, dcycle);
        strcpy(endarg,"DusySuccessOK");
      }
    } else {
      Serial.println("No duty in params");
      strcpy(endarg,"NoDutySetFail");
    }
    Serial.println("giving up on duty");
    request->send(200, "text/plain", endarg); });
}

void WebServerBegin()
{
  server.begin();
}