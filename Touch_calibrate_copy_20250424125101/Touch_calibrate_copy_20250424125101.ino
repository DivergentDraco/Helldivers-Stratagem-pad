#include "FS.h"
#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();
#define CALIBRATION_FILE "/TouchCalData3"
#define REPEAT_CAL false

struct Arrow {
    int x, y, size;
};

Arrow arrowUp = {240, 0, 130};
Arrow arrowDown = {240, 160, 130};
Arrow arrowRight = {400, 160, 130};
Arrow arrowLeft = {80 , 160, 130};
Arrow shiftButton = {400, 0, 130};

bool touchHandled = false;

void setup() {
  // Use serial port
  Serial.begin(9600);

  // Initialise the TFT screen
  tft.init();

  // Set the rotation to the orientation you wish to use in your project before calibration
  // (the touch coordinates returned then correspond to that rotation only)
  tft.setRotation(3);

  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

/*
  // Replace above line with the code sent to Serial Monitor
  // once calibration is complete, e.g.:
  uint16_t calData[5] = { 286, 3534, 283, 3600, 6 };
  tft.setTouch(calData);
*/

  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  //tft.drawCentreString("Touch screen to test!",tft.width()/2, tft.height()/2, 2);

      drawArrowUp(arrowUp.x, arrowUp.y, arrowUp.size, TFT_WHITE);
    drawArrowDown(arrowDown.x, arrowDown.y, arrowDown.size, TFT_WHITE);
    drawArrowRight(arrowRight.x, arrowRight.y, arrowRight.size, TFT_WHITE);
    drawArrowLeft(arrowLeft.x, arrowLeft.y, arrowLeft.size, TFT_WHITE);
    drawShift(shiftButton.x, shiftButton.y, shiftButton.size, TFT_YELLOW);
}

//------------------------------------------------------------------------------------------

void loop(void) {
  uint16_t h = 0, k = 0; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&h, &k);

  // Draw a white spot at the detected coordinates
  if (pressed) {
    //tft.fillCircle(h, k, 2, TFT_WHITE);         // <-- Uncomment this for touch debugging

    //Serial.print("x,y = ");
    //Serial.print(x);
    //Serial.print(",");
    //Serial.println(y);
  }

  uint16_t x, y;
    bool touched = tft.getTouch(&x, &y);

    if (touched && !touchHandled) {
        if (isInside(x, y, arrowUp)) {
            //drawArrowUp(arrowUp.x, arrowUp.y, arrowUp.size, TFT_YELLOW);
            //fadeArrowToWhite(drawArrowUp, arrowUp);
            Serial.println("UP_ARROW");
        } else if (isInside(x, y, arrowDown)) {
            //drawArrowDown(arrowDown.x, arrowDown.y, arrowDown.size, TFT_YELLOW);
            Serial.println("DOWN_ARROW");
            //fadeArrowToWhite(drawArrowDown, arrowDown);
        } else if (isInside(x, y, arrowLeft)) {
            //drawArrowLeft(arrowLeft.x, arrowLeft.y, arrowLeft.size, TFT_YELLOW);
            Serial.println("LEFT_ARROW");
            //fadeArrowToWhite(drawArrowLeft, arrowLeft);
        } else if (isInside(x, y, arrowRight)) {
            //drawArrowRight(arrowRight.x, arrowRight.y, arrowRight.size, TFT_YELLOW);
            Serial.println("RIGHT_ARROW");
            //fadeArrowToWhite(drawArrowRight, arrowRight);
        } else if (isInside(x, y, shiftButton)) {
            Serial.println("SHIFT");
        }
        touchHandled = true;
    } else if (!touched) {
        touchHandled = false;
    }
}

//------------------------------------------------------------------------------------------

// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}

void fadeArrowToWhite(void (*drawArrow)(int, int, int, uint16_t), Arrow arrow) {
    for (int b = 0; b <= 255; b += 15) {
        uint16_t color = tft.color565(255, 255, b);  // Fade blue in
        drawArrow(arrow.x, arrow.y, arrow.size, color); // tweak this for speed
    }
}

bool isInside(int x, int y, Arrow arrow) {
    int halfSize = arrow.size - 30;
    int yOffset = (arrow.y == 160) + 50;  // Only add if it's one of the arrows with vertical offset
    int centerY = arrow.y + yOffset;
    return (x > arrow.x - halfSize && x < arrow.x + halfSize && y > centerY - halfSize && y < centerY + halfSize);
}

void drawArrowUp(int x, int y, int size, uint16_t color) {
    int halfsize = size / 2;
    //draw red rectangle enshrouding the arrow to specify touch area for the button
    tft.drawRect(x - halfsize - 15, y, 130 + 30, 130 + 30, TFT_BLACK);
    //draw the arrow
    tft.fillRect(x, y + halfsize, halfsize / 2 - 7, halfsize, color);
    tft.fillRect(x - halfsize / 2 + 7, y + halfsize, halfsize / 2, halfsize, color);
    tft.fillTriangle(x - halfsize, y + halfsize, x, y, x + halfsize, y + halfsize, color);
}
void drawArrowDown(int x, int y, int size, uint16_t color) {
    int halfsize = size / 2;
    tft.drawRect(x - halfsize - 15, y, 130 + 30, 130 + 30, TFT_BLACK);
    y += 30;
    tft.fillRect(x, y, halfsize / 2 - 7, halfsize, color);
    tft.fillRect(x - halfsize / 2 + 7, y, halfsize / 2, halfsize, color);
    tft.fillTriangle(x - halfsize, y + halfsize, x, y + halfsize * 2, x + halfsize, y + halfsize, color);
}
void drawArrowRight(int x, int y, int size, uint16_t color) {
    int halfsize = size / 2;
    tft.drawRect(x - halfsize - 15, y, 130 + 30, 130 + 30, TFT_BLACK);
    y += 30;
    tft.fillRect(x - halfsize, y + halfsize / 2 + 7, halfsize, halfsize / 2 - 7, color);
    tft.fillRect(x - halfsize, y + halfsize - 7, halfsize, halfsize / 2, color);
    tft.fillTriangle(x, y, x, y + halfsize * 2, x + halfsize, y + halfsize, color);
}
void drawArrowLeft(int x, int y, int size, uint16_t color) {
    int halfsize = size / 2;
    tft.drawRect(x - halfsize - 15, y, 130 + 30, 130 + 30, TFT_BLACK);
    y += 30;
    tft.fillRect(x, y + halfsize / 2 + 7, halfsize, halfsize / 2 - 7, color);
    tft.fillRect(x, y + halfsize - 7, halfsize, halfsize / 2, color);
    tft.fillTriangle(x, y, x, y + halfsize * 2 - 7, x - halfsize, y + halfsize, color);
}
void drawShift(int x, int y, int size, uint16_t color) {
    int halfsize = size / 2;
    tft.drawRect(x - halfsize - 15, y, 130 + 30, 130 + 30, TFT_BLACK);
}