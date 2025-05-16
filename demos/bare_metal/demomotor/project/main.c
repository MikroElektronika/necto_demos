/*!
 * @file main.c
 * @brief Bare Metal Motor Demo
 *
 * @mainpage Bare Metal Brushless 17 Click Demo
 *
 * @section overview Overview
 * This example demonstrates basic control of the **Brushless 17 Click** board using a
 * bare-metal project setup in **NECTO Studio**. The motor is rotated at various speeds
 * by modifying the PWM duty cycle.
 *
 * @section app_structure Application Structure
 * The demo application is divided into two key sections:
 *
 * @subsection configure_mikrobus Configure MikroBUS
 * Initializes and configures the MikroBUS pins used by the Brushless 17 Click board:
 * - RST, PWM, INT, AN and CS
 *
 * @subsection motor_control Rotate the Motor
 * The motor is driven by adjusting the PWM duty cycle. The duty cycle ranges from 40%
 * up to 100%, then back to 40%, simulating acceleration and deceleration.
 *
 * @note The demo assumes the motor is connected properly to the Click board and that
 * proper power is supplied.
 *
 * @author MIKROE SDK Team
 */

#include "motor_demo.h"

int main( void )
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    configure_mikrobus();

    while (1)
    {
        // Set counter-clockwise direction.
        set_motor_direction( MOTOR_DIRECTION_CCW );
        // Increase motor speed from 40% to 100% for 10 seconds.
        pwm_sweep( 10.0, MOTOR_SPEED_INCREASE );
        // Decrease motor speed from 100% to 40% for 10 seconds.
        pwm_sweep( 10.0, MOTOR_SPEED_DECREASE );
    }

    return 0;
}
