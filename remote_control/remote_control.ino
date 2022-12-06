//This code is developed by Ishan_Sachintha

#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS 0x3C
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;

bool select_anime = false;
bool set_anime = false;
bool Home_axis = false;
int anime = 0;


#define Emg_stop  8
#define home_axis 7
#define up_button 5
#define down_button 6
#define set_button 4

uint8_t col0 = 0;
uint8_t col1 = 0;
uint8_t rows;

RH_ASK driver;  

void setup()
{
  Serial.begin(9600);	  // Debugging only
  if (!driver.init())
    Serial.println("init failed");

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(Adafruit5x7);

    oled.clear();
    oled.set2X();
    oled.setCursor(10, 10);
    oled.println(" SMART");
    delay(1000);
    oled.println("    WET");
    delay(1000);
    oled.println("     SIGN");
    delay(1000);
    oled.clear();


  oled.setCursor(25, 0);
  oled.set1X();
  oled.println("SMART WET SIGN");
  oled.println("_____________________");
  oled.println("");
  oled.set1X();
  oled.print("Animation-no:");
  oled.print("\n");
  oled.print("\n");
  oled.print("\nSet and Press for Run");
  uint8_t w = oled.strWidth("Animation-no:");
  col0 = col0 < w ? w : col0;
  col0 += 3;
  col1 = col0 + oled.strWidth("99") + 10;
  rows = oled.fontRows();

  pinMode(Emg_stop, INPUT_PULLUP);
  pinMode(home_axis, INPUT_PULLUP);
  pinMode(up_button, INPUT_PULLUP);
  pinMode(down_button, INPUT_PULLUP);
  pinMode(set_button, INPUT_PULLUP);

}

void clearValue(uint8_t row) {
  oled.clear(col0, col1, row, 1);
}
void loop()
{
  clearValue(2 * rows);
  oled.set2X();
  oled.print(anime);
  delay(100);

  if (digitalRead(Emg_stop) == 0) {

    const char *msg = "s";

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    Serial.println("stoped");
    oled.clear(0, 124, 4, 8);
    oled.set1X();
    oled.print("\n");
    oled.set2X();
    oled.print("Stoped");
    delay(1000);
    //oled.clear(0, 124, 4, 8);
  }

  if (digitalRead(home_axis) == 0) {

    const char *msg = "h";

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    Serial.println("Homing");
    oled.clear(0, 124, 4, 8);
    oled.set1X();
    oled.print("\n");
    oled.set2X();
    oled.print("Homing");
    delay(1000);
    oled.clear(0, 124, 4, 8);
    oled.set1X();
    oled.print("\n");
    oled.set2X();
    oled.print("Homed");
  }

  if (digitalRead(set_button) == 0) {

    char cstr[16];
    itoa(anime, cstr, 10);
    const char *msg = cstr;

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    Serial.print("set animation  to -");
    Serial.println(cstr);
    oled.clear(0, 124, 4, 8);
    oled.set1X();
    oled.print("\n");
    oled.set2X();
    oled.print("SEND-ANIME");
    delay(500);
    oled.clear(0, 124, 4, 8);
    oled.set1X();
    oled.print("\n");
    oled.set2X();
    oled.print("Running");
  }

  if (digitalRead(up_button) == 0) {
    if (anime < 5) {
      anime++;
      Serial.println(anime);
      delay(500);
    }
  }
  if (digitalRead(down_button) == 0) {
    if (anime >= 1) {
      anime--;
      Serial.println(anime);
      delay(500);
    }
  }
}
