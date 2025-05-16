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

#include "motor_demo.h"

/**
 * @defgroup MikroBUS_Pin_Control MikroBUS Pin Control Macros
 * @brief User-defined macros for configuring and controlling MikroBUS pins.
 *
 * These macros provide control over the direction and state of specific MikroBUS pins
 * such as RST, PWM, AN, CS, and INT.
 * @{
 */

/** @brief Set the RST pin as output. */
#define MIKROBUS_RST_SET_OUTPUT R_PORT4->PDR_b.PDR7 = 1
/** @brief Drive the RST pin low. */
#define MIKROBUS_RST_SET_LOW    R_PORT4->PODR_b.PODR7 = 0
/** @brief Drive the RST pin high. */
#define MIKROBUS_RST_SET_HIGH   R_PORT4->PODR_b.PODR7 = 1

/** @brief Set the PWM pin as output. */
#define MIKROBUS_PWM_SET_OUTPUT R_PORT1->PDR_b.PDR7 = 1
/** @brief Drive the PWM pin low. */
#define MIKROBUS_PWM_SET_LOW    R_PORT1->PODR_b.PODR7 = 0
/** @brief Drive the PWM pin high. */
#define MIKROBUS_PWM_SET_HIGH   R_PORT1->PODR_b.PODR7 = 1

/** @brief Set the AN pin as output. */
#define MIKROBUS_AN_SET_OUTPUT  R_PORT0->PDR_b.PDR0 = 1
/** @brief Drive the AN pin high. */
#define MIKROBUS_AN_SET_HIGH    R_PORT0->PODR_b.PODR0 = 1

/** @brief Set the CS pin as output. */
#define MIKROBUS_CS_SET_OUTPUT  R_PORT1->PDR_b.PDR3 = 1
/** @brief Drive the CS pin high. */
#define MIKROBUS_CS_SET_HIGH    R_PORT1->PODR_b.PODR3 = 1

/** @brief Set the INT pin as output. */
#define MIKROBUS_INT_SET_OUTPUT R_PORT3->PDR_b.PDR2 = 1
/** @brief Drive the INT pin high. */
#define MIKROBUS_INT_SET_HIGH   R_PORT3->PODR_b.PODR2 = 1

/** @} */ // end of MikroBUS_Pin_Control

/**
 * @def DELAY_PARAMETER_US
 * @brief Delay parameter in microseconds for frequency adjustment.
 *
 * This macro defines the base delay in microseconds used to control timing
 * in motor control routines, such as frequency adjustment or sweeping cycles.
 *
 * @note Changing this value directly impacts the duration and frequency of
 * motor sweeping operations. A lower value increases the sweep speed, while
 * a higher value slows it down.
 */
#define DELAY_PARAMETER_US 1875

void set_motor_direction( motor_direction_t direction )
{
    // Clockwise direction.
    if ( MOTOR_DIRECTION_CW == direction )
        MIKROBUS_RST_SET_LOW;

    // Counter-clockwise direction.
    if ( MOTOR_DIRECTION_CCW == direction )
        MIKROBUS_RST_SET_HIGH;
}

void pwm_sweep( float duration, motor_speed_t mode )
{
    uint16_t period_us = DELAY_PARAMETER_US;
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

            MIKROBUS_PWM_SET_HIGH;

            // Delay for high signal duration.
            Delay_us( pulse_us );

            MIKROBUS_PWM_SET_LOW;

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

            MIKROBUS_PWM_SET_HIGH;

            // Delay for high signal duration.
            Delay_us( pulse_us );

            MIKROBUS_PWM_SET_LOW;

            // Delay for low signal duration.
            if ( period_us != pulse_us )
                Delay_us( period_us - pulse_us );

            current_step--;
        }
    }
}

void configure_mikrobus()
{
    MIKROBUS_AN_SET_OUTPUT;
    MIKROBUS_AN_SET_HIGH;

    MIKROBUS_PWM_SET_OUTPUT;

    MIKROBUS_RST_SET_OUTPUT;

    MIKROBUS_CS_SET_OUTPUT;
    MIKROBUS_CS_SET_HIGH;

    MIKROBUS_INT_SET_OUTPUT;
    MIKROBUS_INT_SET_HIGH;
}
