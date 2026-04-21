#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"

void toggle_led_callback(void) {
    // Comuta starea pinului la fiecare intrerupere (1 ms implicit)
    GPIO_Toggle(GPIO_PORTB, 5);
}

int main(void) {
    GPIO_Init(GPIO_PORTB, 5, GPIO_OUTPUT);
    GPIO_Init(GPIO_PORTD, 2, GPIO_OUTPUT);
    
    // Initializare Timer0 cu pointer la functia callback
    Timer0_InitCallback(toggle_led_callback);  
    
    uint32_t last_time_2 = 0; 
    
    while (1) {
        uint32_t current_time = Millis();

        // LED-ul de pe PORTB5 este comutat automat in intreruperea de timer,
        // asadar nu mai e nevoie sa verificam last_time_1 in bucla principala.

        if (current_time - last_time_2 >= 1000) {
            last_time_2 = current_time;
            GPIO_Toggle(GPIO_PORTD, 2); // Toggle LED 2 at 1s intervals
        }
    }
}