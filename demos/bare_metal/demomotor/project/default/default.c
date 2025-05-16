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

#include "../motor_demo.h"

/*
 *  If you see this note, it means that you have chosen a setup without predefined
 *  bare metal implementation for this demo. You can either create a setup with one
 *  of supported boards defined in the README.md file or follow TODO comments and
 *  configure code manually.
*/

// Delay parameter for frequency adjustment.
// Note: Changing this value also affects duration of motor sweeping.
#define DELAY_PARAMETER 1875

void set_motor_direction( motor_direction_t direction )
{
    // Clockwise direction.
    if ( MOTOR_DIRECTION_CW == direction )
        // TODO: set MIKROBUS RST to low.
        ;

    // Counter-clockwise direction.
    if ( MOTOR_DIRECTION_CCW == direction )
        // TODO: set MIKROBUS RST to high.
        ;

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

            // TODO: set MIKROBUS PWM to high.

            // Delay for high signal duration.
            Delay_us( pulse_us );

            // TODO: set MIKROBUS PWM to low.

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

            // TODO: set MIKROBUS PWM to high.

            // Delay for high signal duration.
            Delay_us( pulse_us );

            // TODO: set MIKROBUS PWM to low.

            // Delay for low signal duration.
            if ( period_us != pulse_us )
                Delay_us( period_us - pulse_us );

            current_step--;
        }
    }
}

void configure_mikrobus()
{
    // TODO: set MIKROBUS AN pin as output high.

    // TODO: set MIKROBUS PWM as output.

    // TODO: set MIKROBUS RST as output.

    // TODO: set MIKROBUS CS as output high.

    // TODO: set MIKROBUS INT as output high.
}
