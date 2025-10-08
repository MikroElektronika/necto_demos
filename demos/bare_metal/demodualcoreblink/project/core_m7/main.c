/**
 * @file main.c
 * @brief Main function for PRIMARY Core of Dual-Core LED Blink application.
 * @details This application was developed to toggle user LEDs on NUCLEO-144
 *          Board with Dual Core STM32H755ZI MCU using primary Cortex-M7 Core
 *          to control the toggling of LD1 and LD3 LEDs and secondary Cortex-M4
 *          Core to control the toggling of LD2 LED. Different speed of GPIO
 *          toggling is achieved by adjusting DELAY_COUNTER macro value in
 *          source files for both cores.
 *
 *          This file configures as outputs PB0 and PB14 pins that are used as
 *          LD1 and LD3 LEDs and toggles them using PRIMARY Cortex-M7 Core
 *          of STM32H755ZI MCU.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "mcu.h"

typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin_number;
} LedConfig;

LedConfig leds[] = {
    // LD1 is PB0
    { GPIOB, 0 },
    // LD3 is PB14
    { GPIOB, 14 }
};

#define NUM_LEDS            ( sizeof( leds ) / sizeof( leds[0] ) )
#define MODER_MODE_MASK     0x3U
#define MODER_MODE_OUTPUT   0x1U

// TODO - change value to adjust delay duration
#define DELAY_COUNTER 1000000

void delay( uint32_t duration )
{
    for ( volatile uint32_t i = 0; i < duration; ++i ) {
        asm( "nop" );
    }
}

void configure_output_pin( LedConfig *led )
{
    // Enable GPIOB clock
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;

    uint32_t shift = led->pin_number * 2;
    // Clear mode bits for requested pin
    led->port->MODER &= ~( MODER_MODE_MASK << shift );
    // Set requested pin as output
    led->port->MODER |=  ( MODER_MODE_OUTPUT << shift );
}

void toggle_all_leds( LedConfig *leds, int count )
{
    uint32_t mask = 0;

    // Toggle LD1 and LD3 pins
    for ( int i = 0; i < count; ++i ) {
        mask = ( 1 << leds[i].pin_number );
        leds[i].port->ODR ^= mask;
    }
}

int main(void)
{
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Local variable for controlling blink duration
    uint32_t local_delay = DELAY_COUNTER;

    // Configure LD1 and LD3 as output pins
    for ( int i = 0; i < NUM_LEDS; ++i ) {
        configure_output_pin( &leds[i] );
    }

    while ( 1 )
    {
        // Toggle LD1 and LD3 pins
        toggle_all_leds( leds, NUM_LEDS );

        // Call delay with local variable
        delay( local_delay );
    }

    return 0;
}
