#include "Button.h"
#include "Display.h"

int but1 = 0;
int but2 = 0;


void pollButtons()
{
    // but 1
    if (digitalRead(0) == 0)
    {
        if (but1 != 0)
        {
            Serial.println("But1 released");
            Serial.printf("v1: %d, v2, %d", digitalRead(35), digitalRead(0));
            Serial.println();
            dcycle -= 64;
            if (dcycle < dcMIN)
                dcycle = dcMIN;
            ledcWrite(LEDC_CHANNEL, dcycle);
            Serial.printf("dcycle now %d", dcycle);
            Serial.println("");
            but1 = 0;
            updateDisplay(dcycle, 105);
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
            dcycle += 64;
            if (dcycle > dcMAX)
                dcycle = dcMAX;
            Serial.printf("dcycle now %d", dcycle);
            Serial.println("");
            ledcWrite(LEDC_CHANNEL, dcycle);
            updateDisplay(dcycle, 105);
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