/****************************************************************************
** Copyright (C) 2025 MikroElektronika d.o.o.
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

#include "../motor_demo.h"

// Delay parameter for frequency adjustment.
// Note: Changing this value also affects duration of motor sweeping.
#define DELAY_PARAMETER 1875

void set_motor_direction( motor_direction_t direction )
{
    // Clockwise direction.
    if ( MOTOR_DIRECTION_CW == direction )
        // Set MIKROBUS RST to low.
        R_PORT4->PODR_b.PODR7 = 0;

    // Counter-clockwise direction.
    if ( MOTOR_DIRECTION_CCW == direction )
        // Set MIKROBUS RST to high.
        R_PORT4->PODR_b.PODR7 = 1;
}

void pwm_sweep( float duration, motor_speed_t mode )
{
    uint16_t period_us = DELAY_PARAMETER;
    float num_steps = duration * 1000; // Convert sweep time to milliseconds.
    float duty_cycle;
    float current_step;
    uint32_t pulse_us;

    // If increasing speed.
    if ( MOTOR_SPEED_INCREASE == mode )
    {
        // Motor starts spinning at 40% duty cycle.
        current_step = 0.4 * num_steps;
        while ( current_step < num_steps )
        {
            duty_cycle = ( current_step * 100 ) / num_steps;
            pulse_us = ( uint32_t )( ( duty_cycle * period_us ) / 100 );

            // Set MIKROBUS PWM to high.
            R_PORT1->PODR_b.PODR7 = 1;
            // Delay for high signal duration.
            Delay_us( pulse_us );

            // Set MIKROBUS PWM to low.
            R_PORT1->PODR_b.PODR7 = 0;
            // Delay for low signal duration.
            if ( period_us != pulse_us )
                Delay_us( period_us - pulse_us );

            current_step++;
        }
    }
    // If decreasing speed.
    if ( MOTOR_SPEED_DECREASE == mode )
    {
        // Motor stops spinning at 40% duty cycle.
        current_step = num_steps;
        while ( current_step >= ( num_steps * 0.4 ) )
        {
            duty_cycle = ( current_step * 100 ) / num_steps;
            pulse_us = ( uint32_t )( ( duty_cycle * period_us ) / 100 );

            // Set MIKROBUS PWM to high.
            R_PORT1->PODR_b.PODR7 = 1;
            // Delay for high signal duration.
            Delay_us( pulse_us );

            // Set MIKROBUS PWM to low.
            R_PORT1->PODR_b.PODR7 = 0;
            // Delay for low signal duration.
            if ( period_us != pulse_us )
                Delay_us( period_us - pulse_us );

            current_step--;
        }
    }
}

void configure_mikrobus()
{
    // Set MIKROBUS AN pin as output high.
    R_PORT0->PDR_b.PDR0 = 1;
    R_PORT0->PODR_b.PODR0 = 1;

    // Set MIKROBUS PWM as output.
    R_PORT1->PDR_b.PDR7 = 1;

    // Set MIKROBUS RST as output.
    R_PORT4->PDR_b.PDR7 = 1;

    // Set MIKROBUS CS as output high.
    R_PORT1->PDR_b.PDR3 = 1;
    R_PORT1->PODR_b.PODR3 = 1;

    // Set MIKROBUS INT as output high.
    R_PORT3->PDR_b.PDR2 = 1;
    R_PORT3->PODR_b.PODR2 = 1;
}
