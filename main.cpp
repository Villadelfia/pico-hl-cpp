#include "pico/stdlib.h"
#include "Pin.h"

int main() {
    Pin led(PICO_DEFAULT_LED_PIN);
    while (true) {
        led = !led;
        sleep_ms(250);
    }
}
