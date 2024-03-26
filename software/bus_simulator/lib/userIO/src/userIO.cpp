#include "userIO.h"

userIO::userIO(const pin_size_t data_out, const pin_size_t clock, const pin_size_t latch, const std::array<uint8_t, 12> button_pins)
:shift_register(data_out, clock, latch){
    buttons = button_pins;
    for (int i = 0; i < 12; i++){
        pinMode(button_pins[i], INPUT_PULLUP);
    }
    shift_register.setAllLow();
}

userIO::~userIO(){
    
}

uint16_t userIO::read_all_buttons(){
    uint16_t button_data = 0;
    for (int i = 0; i < 12; i++){
        button_data |= (digitalRead(buttons[i]) << i);
    }
    return button_data;
}

uint8_t userIO::read_user_input_buttons(){
    return (read_all_buttons() & 0x0F00) >> 8;
}

uint8_t userIO::read_data_input_buttons(){
    return read_all_buttons() & 0x00FF;
}

void userIO::render_led(const cpu_status status){
    uint8_t led_values[REG_NO] = {0, 0, 0};
    led_values[0] = status.RAM_value;
    led_values[1] = status.Rwy;

    led_values[2] = status.RAM_address;
    shift_register.setAll(led_values);
}

void userIO::set_displayed_register(const enum rgister_select reg){
    displayed_register = reg;
}

void userIO::self_test(){
    Serial.println("IO self test");
    shift_register.setAllHigh();
    delay(1000);
    shift_register.setAllLow();
    delay(2000);
    uint16_t button_data = read_all_buttons();    
    Serial.println(button_data, BIN);
}

