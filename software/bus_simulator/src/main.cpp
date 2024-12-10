#include <Arduino.h>
#include "config.h"

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <menu.h>//menu macros and objects
#include <menuIO/PCF8574Out.h>//arduino I2C LCD
#include <menuIO/encoderIn.h>//quadrature encoder driver and fake stream
#include <menuIO/keyIn.h>//keyboard driver and fake stream (for the encoder button)
#include <menuIO/chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)

#include <bus_cpu.h>
#include <userIO.h>

using namespace Menu;

userIO io(DATA_PIN, CLOCK_PIN, LATCH_PIN, button_pins);
bus_cpu cpu;
bus_cpu_status cpu_status;

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Encoder /////////////////////////////////////

encoderIn<ENC_S2,ENC_S1> encoder;//simple quad encoder driver

encoderInStream<ENC_S2,ENC_S1> encStream(encoder,ENC_SENSIVITY);// simple quad encoder fake Stream

//a keyboard with only one key as the encoder button
keyMap encBtn_map[]={{-ENC_SW,defaultNavCodes[enterCmd].ch}};//negative pin numbers use internal pull-up, this is on when low
keyIn<1> encButton(encBtn_map);//1 is the number of keys

//input from the encoder + encoder button + serial
menuIn* inputsList[]={&encStream,&encButton};
chainStream<2> in(inputsList);//3 is the number of inputs

uint8_t ram_address_edit = 0;
uint8_t ram_value = 0;

result edit_ram_commit(eventMask e,navNode& nav, prompt &item) {
  cpu.set_RAM(ram_address_edit, io.read_data_input_buttons());
  ram_address_edit++;
  ram_value = cpu.get_RAM(ram_address_edit);
  return proceed;
}

result display_ram(eventMask e,navNode& nav, prompt &item) {
  ram_value = cpu.get_RAM(ram_address_edit);
  return proceed;
}

MENU(ram_edit,"Zapis RAM",doNothing,noEvent,noStyle
  ,FIELD(ram_address_edit,"Adres:","",0,63,1,0,display_ram,anyEvent,wrapStyle)
  ,OP("Zapisz",edit_ram_commit,enterEvent)
  ,FIELD(ram_value,"Wartosc:","",0,255,0,0,doNothing,noEvent,noStyle)
  ,EXIT("<Wroc")
);

uint8_t program_address_edit = 0;
uint8_t program_value = 0;

result edit_programm_commit(eventMask e,navNode& nav, prompt &item) {
  cpu.set_Rp(program_address_edit, io.read_data_input_buttons());
  program_address_edit++;
  program_value = cpu.get_Rp(program_address_edit);
  return proceed;
}

result display_program(eventMask e,navNode& nav, prompt &item) {
  program_value = cpu.get_Rp(program_address_edit);
  return proceed;
}

MENU(program_edit,"Zapis programu",doNothing,noEvent,noStyle
  ,FIELD(program_address_edit,"Adres","",0,15,1,0,display_program,anyEvent,wrapStyle)
  ,OP("Zapisz",edit_programm_commit,enterEvent)
  ,FIELD(program_value,"Wartosc","",0,255,0,0,doNothing,noEvent,noStyle)
  ,EXIT("<Wroc")
);

result end_execution_commit(eventMask e,navNode& nav, prompt &item) {
  cpu.reset();
  return quit;
}

registers displayed_register = Ra;

TOGGLE(displayed_register,dispReg,"Rejestr: ",doNothing,noEvent,wrapStyle
  ,VALUE("Ra",registers::Ra,doNothing,noEvent)
  ,VALUE("Rb",registers::Rb,doNothing,noEvent)
  ,VALUE("Rc",registers::Rc,doNothing,noEvent)
);

result start_program(eventMask e,navNode& nav, prompt &item) {
  cpu.set_execution_type(execution_type::PROGRAM);
  cpu.start_execution();
  return proceed;
}

MENU(program_mode,"Program",start_program,enterEvent,noStyle
  ,SUBMENU(dispReg)
  ,OP("Zakoncz",end_execution_commit,enterEvent)
);

result continue_execution_commit(eventMask e,navNode& nav, prompt &item) {
  cpu.continue_execution();
  return proceed;
}

result start_cycle(eventMask e,navNode& nav, prompt &item) {
  cpu.set_execution_type(execution_type::CYCLE);
  cpu.start_execution();
  return proceed;
}

MENU(cycle_mode,"Cykl",start_cycle,enterEvent,noStyle
  ,OP("Kontynuuj",continue_execution_commit,enterEvent)
  ,SUBMENU(dispReg)
  ,OP("Zakoncz",end_execution_commit,enterEvent)
);

result start_micro_cycle(eventMask e,navNode& nav, prompt &item) {
  cpu.set_execution_type(execution_type::MICRO_CYCLE);
  cpu.start_execution();
  return proceed;
}

MENU(ucycle_mode,"Mikrocykl",start_micro_cycle,enterEvent,noStyle
  ,OP("Kontynuuj",continue_execution_commit,enterEvent)
  ,SUBMENU(dispReg)
  ,OP("Zakoncz",end_execution_commit,enterEvent)
);

MENU(start_execution,"Praca",doNothing,noEvent,noStyle
  ,SUBMENU(program_mode)
  ,SUBMENU(cycle_mode)
  ,SUBMENU(ucycle_mode)
  ,EXIT("<Wroc")
);

uint8_t execution_clock = 10;

result reset_system_commit(eventMask e,navNode& nav, prompt &item) {
  cpu.clear();
  return proceed;
}

result set_exec_speed_commit(eventMask e,navNode& nav, prompt &item) {
  cpu.set_execution_speed(1000/execution_clock);
  return proceed;
}


MENU(busMainMenu,"Szyna danych",doNothing,noEvent,wrapStyle
  ,SUBMENU(start_execution)
  ,SUBMENU(ram_edit)
  ,SUBMENU(program_edit)
  ,FIELD(execution_clock,"Zegar","Hz",1,20,1,0,set_exec_speed_commit,anyEvent,wrapStyle)
  ,OP("Reset",reset_system_commit,enterEvent)
);

MENU_OUTPUTS(out, 3, LCD_OUT(lcd,{0,0,20,4}), NONE);
NAVROOT(nav, busMainMenu, 3, in, out);//the navigation root object, 3 is the nesting level

result user_input(menuOut& o,idleEvent e) {
  switch(e) {
    case idleStart:
      o.print("Wprowadz wartosc");
      break;
    case idleEnd:
      cpu.take_user_input(io.read_user_input_buttons());
    break;
  }
  return proceed;
}

void setup() {
  pinMode(ENC_SW,INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Starting up...");
  encoder.begin();
  Wire.setSCL(LCD_SCL);
  Wire.setSDA(LCD_SDA);
  Wire.begin();
  lcd.begin(20, 4);
  nav.showTitle=false;
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("Szyna danych");
  lcd.setCursor(0, 1);
  lcd.print("symulator V1.0");
  delay(2000);
}

void loop() {
  static bus_cpu_state cpu_state;
  nav.poll();
  cpu_state = cpu.schedule_execution();
  switch (cpu_state) {
    case bus_cpu_state::READY:
      break;
    case bus_cpu_state::EXECUTION:
      break;
    case bus_cpu_state::CYCLE_DONE:
      break;
    case bus_cpu_state::DONE:
      break;
    case bus_cpu_state::USER_INPUT:
      nav.idleOn(user_input);
      break;
    case bus_cpu_state::EXCEPTION:
      Serial.println("Exception occured");
      break;
  }
  io.set_displayed_register(displayed_register);
  cpu.get_register_values(cpu_status);
  io.render_led(cpu_status);
  delay(50);
}