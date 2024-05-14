#ifndef USERIO_H
#define USERIO_H

#include <ShiftRegister74HC595.h>
#include <array>
#include <cpu.h>

#define REG_NO 3

enum rgisters{
    Ra = 0,
    Rb = 1,
    Rc = 2
};

class userIO{
private:
    ShiftRegister74HC595<REG_NO> shift_register;
    std::array<uint8_t, 12> buttons;
    enum rgisters displayed_register;

    uint16_t read_all_buttons();
public:
    userIO(const pin_size_t shift_data, const pin_size_t shift_clock, const pin_size_t shift_latch, const std::array<uint8_t, 12> button_pins);
    ~userIO();
    
    void self_test();
    uint8_t read_user_input_buttons();
    uint8_t read_data_input_buttons();
    void render_led(const cpu_status status);
    void set_displayed_register(const enum rgisters reg);
};

#endif