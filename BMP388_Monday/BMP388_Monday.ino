/** 
 *  TGH1
 *  BMP388 Temperature and Atmospheric Pressure Sensor 
 *  with results on a small LCD display for ESE 218 lab 
 *  
 *  author - tghedaoo
 *  author_name - Tushar G. Hedaoo
 */

#include <Wire.h>
#include <SPI.h>
// The below is for BMP388 sensor library
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
// The below is for LCD screen library 
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define SEALEVELPRESSURE_HPA (1013.25)

/*
 * BMP sensor declaration
 * pins on MKR1000 SCK -> 12 & SDI -> 11 (no other pins except power pins will be used 
 */
Adafruit_BMP3XX bmp; // I2C

/*
 * LCD display declaration (Only the below pins will be used)
 * Power the board with 3.3 V and not 5 V
 */
// PINS on MKR1000
#define TFT_DC 7
#define TFT_CS 6
#define TFT_MISO 10
#define TFT_MOSI 8
#define TFT_CLK 9
#define TFT_RST 13
 
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Temperature and Pressure Readings - ESE 218");
  
  /*
   * BMP TEMP PRESS SENSOR
   */
    
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  //bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  /*
   * DISPLAY INITIALIZATION
   */
  
  tft.begin();
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);
 
  delay(10);
  // Setting Display view to desired rotation
  tft.setRotation(3);
  Serial.print("Starting.....");
  Serial.println(startText());
  delay(2000);
  Serial.println(introText());
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  float temp = bmp.temperature;
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  float pres = (bmp.pressure / 100.0);
  Serial.print(pres);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  float alt = (bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.print(alt);
  Serial.println(" m");

  Serial.println();

  display_data(temp,pres,alt);
  
  delay(1000);
}

// This is for testing purposes in the beginning
unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.fillScreen(ILI9341_RED);
  yield();
  tft.fillScreen(ILI9341_GREEN);
  yield();
  tft.fillScreen(ILI9341_BLUE);
  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

// Data on display
unsigned long display_data(float temp,float pres,float alt) {
  //tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0,50);
  //tft.setTextColor(ILI9341_WHITE);  
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.print("Temperature : ");
  tft.print(temp);
  tft.println(" *C");
  
  tft.print("Pressure    : ");
  tft.print(pres);
  tft.println(" hPa");
  tft.print("Altitude    : ");
  tft.print(alt);
  tft.println(" m");
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("TGH1");
  return micros() - start;
}

// Introductions
unsigned long startText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0,100);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println("UNIVERSITY OF ");
  tft.println("PENNSYLVANIA ");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println("ESE 218");

  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("TGH1");
  return micros() - start;
}

unsigned long introText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0,50);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println("Temperature");
  tft.println("Pressure");
  tft.println("Altitude");
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("Reader");

  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("TGH1");
  return micros() - start;
}
