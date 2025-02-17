#include "Display.h"

TFT_eSPI *tft;

void initDisplay()
{
    tft = new TFT_eSPI();
    tft->init();
    tft->setRotation(1);
}

void updateDisplay(int duty, int temp)
{
    // TODO: until i have a real temp, lets set it randomally betwen 15 and 250
    temp = (int)random(15, 250);
    // Fill screen with grey so we can see the effect of printing with and without
    // a background colour defined
    tft->fillScreen(TFT_BLACK);

    // Set "cursor" at top left corner of display (0,0) and select font 2
    // (cursor will move to next line automatically during printing with 'tft.println'
    //  or stay on the line is there is room for the text with tft.print)
    tft->setCursor(0, 0, 2);
    // Set the font colour to be white with a black background, set text size multiplier to 1
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextSize(2);
    // We can now plot text on screen using the "print" class
    if (duty == 0)
    {
        tft->setTextColor(TFT_GREEN, TFT_BLACK);
        tft->println("Off");
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
    }
    else
    {
        tft->setTextColor(TFT_RED, TFT_BLACK);
        tft->println("Running!");
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft->printf("Power: %d%%", ((duty*100)/dcMAX));
    tft->println();

    if (temp < 30)
    {
        tft->setTextColor(TFT_BLUE, TFT_BLACK);
    }
    else if (temp > 60)
    {
        tft->setTextColor(TFT_RED, TFT_BLACK);
    }
    else
    {
        tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    }
    tft->printf("Temperture: %d", temp);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
}