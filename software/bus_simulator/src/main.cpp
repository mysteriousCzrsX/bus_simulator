#include <Arduino.h>
#include <userIO.h>
#include <cpu.h>
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

#include "config.h"

#define DEBUG

userIO io(DATA_PIN, CLOCK_PIN, LATCH_PIN, button_pins);
cpu CPU;
LiquidCrystal_PCF8574 lcd(0x27);

void setup() {

  Serial.begin(115200);
  Wire.setSCL(LCD_SCL);
  Wire.setSDA(LCD_SDA);
  Wire.begin();
  lcd.begin(20, 4);
  lcd.setBacklight(100);
  lcd.setCursor(0, 0);
  lcd.println("LCD_test!");
}

void loop() {
  lcd.println("LCD_test!");
  io.self_test();
  delay(3000);
}

