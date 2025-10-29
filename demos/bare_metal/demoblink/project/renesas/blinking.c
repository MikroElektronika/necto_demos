/****************************************************************************
** Copyright (C) ${COPYRIGHT_YEAR} MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
**  USE OR OTHER DEALINGS IN THE SOFTWARE.
****************************************************************************/

#include "bm_board.h"

/**
 * @brief Bare Metal Pin Configuration routine.
 * @details This function sets pins as outputs and drives them to low state.
 * @return None
 */
static inline void pin_init_output( const Pin *p, uint8_t init_state );

/**
 * @brief Bare Metal Pin Toggling routine.
 * @details This function toggles defined pins every second.
 * @return None
 */
static inline void pin_toggle( const Pin *p );

void initilize_pins() {
    // Initialize all pins
    for ( uint32_t i = 0; i < sizeof( mikrobus_pins )/sizeof( Pin ); i++ )
        pin_init_output( &mikrobus_pins[i], 0 );

    for ( uint32_t i = 0; i < sizeof( board_leds )/sizeof( Pin ); i++ )
        pin_init_output( &board_leds[i], 0 );

    return;
}

void toggle_pins() {
    for ( uint32_t i = 0; i < sizeof( mikrobus_pins )/sizeof( Pin ); i++ )
    {
        // Toggle board LEDs
        for ( uint32_t i = 0; i < sizeof( board_leds )/sizeof( Pin ); i++ )
            pin_toggle( &board_leds[i] );

        // Toggle MikroBUS pins
        pin_toggle( &mikrobus_pins[i] );
        Delay_ms( 1000 );
    }

    return;
}

static inline void pin_init_output( const Pin *p, uint8_t init_state )
{
    if ( p->port == R_PORT0 )
        R_PORT0->PDR |= p->pin_mask;
    else if ( p->port == R_PORT1 )
        R_PORT1->PDR |= p->pin_mask;
    else if ( p->port == R_PORT2 )
        R_PORT2->PDR |= p->pin_mask;
    else if ( p->port == R_PORT3 )
        R_PORT3->PDR |= p->pin_mask;
    else if ( p->port == R_PORT4 )
        R_PORT4->PDR |= p->pin_mask;
    else if ( p->port == R_PORT5 )
        R_PORT5->PDR |= p->pin_mask;
    else if ( p->port == R_PORT6 )
        R_PORT6->PDR |= p->pin_mask;
    else if ( p->port == R_PORT7 )
        R_PORT7->PDR |= p->pin_mask;
    else if ( p->port == R_PORT8 )
        R_PORT8->PDR |= p->pin_mask;
    else if ( p->port == R_PORT9 )
        R_PORT9->PDR |= p->pin_mask;

    if ( init_state )
        p->port->PODR |= p->pin_mask;
    else
        p->port->PODR &= ~p->pin_mask;
}

static inline void pin_toggle( const Pin *p )
{
    p->port->PODR ^= p->pin_mask;
}