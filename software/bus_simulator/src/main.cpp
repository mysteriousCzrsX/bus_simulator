#include <Arduino.h>
#include <userIO.h>
#include <bus_cpu.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <menu.h>

#include "config.h"

#define DEBUG

userIO io(DATA_PIN, CLOCK_PIN, LATCH_PIN, button_pins);
bus_cpu CPU;
LiquidCrystal_PCF8574 lcd(0x27);

int error;

void setup() {

  Serial.begin(115200);
  delay(2000);
  Wire.setSCL(LCD_SCL);
  Wire.setSDA(LCD_SDA);
  Wire.begin();
  lcd.begin(20, 4);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.println("LCD_test!");
}

void loop() {
  Serial.println(error);
  lcd.println("LCD_test!");
  //io.self_test();
  delay(3000);
}

