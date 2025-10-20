#include <array>
#include <stdint.h>
/*Shift register pins*/
#define DATA_PIN 16
#define CLOCK_PIN 17
#define LATCH_PIN 18

/*Button pins*/

const std::array<uint8_t, 12> button_pins = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

/*Encoder configuration*/

#define ENC_S1 19
#define ENC_S2 20
#define ENC_SW 21
#define ENC_SENSIVITY 4


/*LCD pins*/

#define LCD_SDA 12
#define LCD_SCL 13

