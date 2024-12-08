#include <Arduino.h>
#include "config.h"

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <menu.h>//menu macros and objects
#include <menuIO/PCF8574Out.h>//arduino I2C LCD
#include <menuIO/encoderIn.h>//quadrature encoder driver and fake stream
#include <menuIO/keyIn.h>//keyboard driver and fake stream (for the encoder button)
#include <menuIO/chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)

using namespace Menu;

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Encoder /////////////////////////////////////

encoderIn<ENC_S1,ENC_S2> encoder;//simple quad encoder driver
#define ENC_SENSIVITY 4
encoderInStream<ENC_S1,ENC_S2> encStream(encoder,ENC_SENSIVITY);// simple quad encoder fake Stream

//a keyboard with only one key as the encoder button
keyMap encBtn_map[]={{-ENC_SW,defaultNavCodes[enterCmd].ch}};//negative pin numbers use internal pull-up, this is on when low
keyIn<1> encButton(encBtn_map);//1 is the number of keys

//input from the encoder + encoder button + serial
menuIn* inputsList[]={&encStream,&encButton};
chainStream<2> in(inputsList);//3 is the number of inputs

#define LEDPIN LED_BUILTIN

result doAlert(eventMask e, prompt &item);

result showEvent(eventMask e,navNode& nav,prompt& item) {
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}

result myLedOff() {
  return proceed;
}

uint8_t ram_address_edit = 0;

MENU(ram_edit,"Zapis RAM",showEvent,anyEvent,noStyle
  ,FIELD(ram_address_edit,"Adres","",0,15,1,1,doNothing,noEvent,wrapStyle)
  ,OP("Zapisz",showEvent,anyEvent)
  ,EXIT("<Wroc")
);

uint8_t program_address_edit = 0;

MENU(program_edit,"Zapis programu",showEvent,anyEvent,noStyle
  ,FIELD(program_address_edit,"Adres","",0,15,1,1,doNothing,noEvent,wrapStyle)
  ,OP("Zapisz",showEvent,anyEvent)
  ,EXIT("<Wroc")
);

MENU(program_mode,"Program",showEvent,anyEvent,noStyle
  ,OP("Pauza",showEvent,anyEvent)
  ,OP("Start",showEvent,anyEvent)
);

MENU(cycle_mode,"Cykl",showEvent,anyEvent,noStyle
  ,OP("Kontynuuj",showEvent,anyEvent)
  ,OP("Zakoncz",showEvent,anyEvent)
);

MENU(ucycle_mode,"Mikrocykl",showEvent,anyEvent,noStyle
  ,OP("Kontynuuj",showEvent,anyEvent)
  ,OP("Zakoncz",showEvent,anyEvent)
);

MENU(start_execution,"Sub-Menu",showEvent,anyEvent,noStyle
  ,SUBMENU(program_mode)
  ,SUBMENU(cycle_mode)
  ,SUBMENU(ucycle_mode)
  ,EXIT("<Wroc")
);

uint8_t execution_speed = 50;

MENU(busMainMenu,"Szyna danych",doNothing,noEvent,wrapStyle
  ,SUBMENU(ram_edit)
  ,SUBMENU(program_edit)
  ,SUBMENU(start_execution)
  ,FIELD(execution_speed,"Czestotliwosc pracy","%",0,100,10,1,doNothing,noEvent,wrapStyle)
  ,OP("Reset",myLedOff,enterEvent)
);

#define MAX_DEPTH 3

// idx_t tops[MAX_DEPTH]={0,0};
// const panel panels[] MEMMODE={{0,0,16,2}};
// navNode* nodes[MAX_DEPTH];
// panelsList pList(panels,nodes,1);
// lcdOut outLCD(&lcd,tops,pList);//output device for LCD
// menuOut* constMEM outputs[] MEMMODE={&outLCD};//list of output devices
// outputsList out(outputs,1);//outputs list with 1 outputs

MENU_OUTPUTS(out, MAX_DEPTH, LCD_OUT(lcd,{0,0,20,4}), NONE);
NAVROOT(nav,busMainMenu,MAX_DEPTH,in,out);//the navigation root object

result alert(menuOut& o,idleEvent e) {
  if (e==idling) {
    o.setCursor(0,0);
    o.print("alert test");
    o.setCursor(0,1);
    o.print("[select] to continue...");
  }
  return proceed;
}

result doAlert(eventMask e, prompt &item) {
  nav.idleOn(alert);
  return proceed;
}

result idle(menuOut& o,idleEvent e) {
  switch(e) {
    case idleStart:o.print("suspending menu!");break;
    case idling:o.print("suspended...");break;
    case idleEnd:o.print("resuming menu.");break;
  }
  return proceed;
}

void setup() {
  pinMode(ENC_SW,INPUT_PULLUP);
  pinMode(LEDPIN,OUTPUT);
  Serial.begin(115200);
  Serial.println("Arduino Menu Library");
  encoder.begin();
  Wire.setSCL(LCD_SCL);
  Wire.setSDA(LCD_SDA);
  Wire.begin();
  lcd.begin(20, 4);
  nav.idleTask=idle;//point a function to be used when menu is suspended
  mainMenu[1].enabled=disabledStatus;
  nav.showTitle=false;
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("LCD_bruh");
  lcd.setCursor(0, 1);
  lcd.print("aaaa");
  delay(2000);
}

void loop() {
  nav.poll();
  digitalWrite(LEDPIN, ledCtrl);
  test=(millis()/1000)%101;
  Serial.println("test: ");
  delay(100);
}