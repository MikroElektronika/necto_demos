/**
 * @file main.c
 * @brief Main function for SECONDARY Core of Dual-Core LED Blink application.
 * @details This application was developed to toggle user LEDs on Nucleo-144
 *          Board with Dual Core STM32H755ZI MCU using primary Cortex-M7 Core
 *          to control the toggling of LD1 and LD3 LEDs and secondary Cortex-M4
 *          Core to control the toggling of LD2 LED. Different speed of GPIO
 *          toggling is achieved by adjusting DELAY_COUNTER macro value in
 *          source files for both cores.
 *
 *          This file configures as output PE1 pin that is used as LD2 LED and
 *          toggles it using SECONDARY Cortex-M4 Core of STM32H755ZI MCU.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "mcu.h"

// TODO - change value to change delay duration
#define DELAY_COUNTER 100000;

// Busy-wait delay function.
void delay( volatile uint32_t count )
{
    volatile uint8_t i = 0;
    while( count-- ) {
        // Simulate a CPU-consuming task
        for ( i = 0; i < 18; ++i ) {
            asm( "nop" );
        }
    }
}

void configure_output_pin( void )
{
    // Enable GPIOE clock
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOEEN;
    // Clear mode bits for LD2 pin
    GPIOE->MODER = ~GPIO_MODER_MODER1_Msk;
    // Set LD2 as output
    GPIOE->MODER |= GPIO_MODER_MODER1_0;
}

void toggle_led(void)
{
    // Toggle LD2 pin
    GPIOE->ODR ^= GPIO_ODR_OD1;
}

int main( void )
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Local variable for controlling blink duration
    uint32_t local_delay = DELAY_COUNTER;

    // Configure LD2 as output pin
    configure_output_pin();

    while ( 1 )
    {
        // Toggle LED
        toggle_led();

        // Call delay with local variable
        delay( local_delay );
    }

    return 0;
}

