#include "userIO.h"

userIO::userIO(const pin_size_t data_out, const pin_size_t clock, const pin_size_t latch, const std::array<uint8_t, 12> button_pins)
:shift_register(data_out, clock, latch){
    buttons = button_pins;
    for (int i = 0; i < 12; i++){
        pinMode(button_pins[i], INPUT_PULLUP);
    }
    shift_register.setAllLow();
    displayed_register = Ra;
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

void userIO::render_led(const bus_cpu_status status){
    uint8_t led_values[REG_NO] = {0, 0, 0};
    led_values[0] = status.RAM_value;
    led_values[1] = status.Rwy;
    switch (displayed_register){
        case Ra:
            led_values[1] |= (status.Ra << 4);
            break;
        case Rb:
            led_values[1] |= (status.Rb << 4);
            break;
        case Rc:
            led_values[1] |= (status.Rc << 4);
            break;
        default:
            led_values[1] |= (status.Ra << 4);
            break;
    }
    led_values[2] = status.RAM_address;
    shift_register.setAll(led_values);
}

void userIO::set_displayed_register(const enum registers reg){
    displayed_register = reg;
}

void userIO::self_test(){
    Serial.println("IO self test");
    shift_register.setAllHigh();   
    delay(500);
    shift_register.setAllLow();
    delay(500);

    bus_cpu_status test_status = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    Serial.println("Displaying RAM value");
    test_status.RAM_value = 0x0F;
    render_led(test_status);
    delay(2000);
    test_status.RAM_value = 0;


    Serial.println("Displaying RAM address");
    test_status.RAM_address = 0x0F;
    render_led(test_status);
    delay(2000);
    test_status.RAM_address = 0;

    Serial.println("Displaying register");
    test_status.Ra = 0x0F;
    render_led(test_status);
    delay(2000);
    test_status.Ra = 0;

    Serial.println("Displaying Rwy");
    test_status.Rwy = 0x0F;
    render_led(test_status);
    delay(2000);
    test_status.Rwy = 0;

    uint8_t user_button = read_user_input_buttons();  
    Serial.println("User button");
    Serial.println(user_button, BIN);
    uint8_t data_button = read_data_input_buttons();
    Serial.println("Data button");
    Serial.println(data_button, BIN);

}

