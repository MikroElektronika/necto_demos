/*!
 * @file main.c
 * @brief Unified LED toggle example for Bare-Metal Boards.
 *
 * @section overview Overview
 * This example demonstrates basic configuration of defined pins as output and toggles
 * them every second.
 *
 * @section app_structure Application Structure
 * The demo application is divided into two key sections:
 *
 * @subsection initilize_pins Configure Pins
 * Initializes and configures the pins declared in mikrobus_pins and board_leds structures:
 * - AN, RST, CS, SCK, CIPO, COPI, PWM, INT, RX, TX, SCL, SDA, etc.
 * - LD1, LD2, LD3, etc.
 *
 * @subsection toggle_pins Toggle the Pins
 * Every pin defined in mikrobus_pins structure is being toggled every second one-by-one.
 * All pins defined in board_leds structure are being toggled every second.
 *
 * @author MIKROE SDK Team
 */

#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "bm_board.h"

int main(void)
{
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    initilize_pins();

    while (1)
    {
        toggle_pins();
    }

    return 0;
}
