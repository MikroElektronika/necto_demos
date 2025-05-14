/*!
 * @file main.c
 * @brief Bare Metal Motor Demo
 *
 * # Description
 * This example demonstrates the use of the Brushless 17 Click Board by driving the
 * motor in both directions at different speeds using Bare Metal NECTOStudio setup.
 *
 * The demo application is composed of two sections:
 *
 * ## Configure Mikrobus
 * Configures Mikrobus pins used by Brushless 17 Click Board.
 *
 * ## Rotate the motor
 * Controls the motor speed by changing the PWM duty cycle.
 * The duty cycle ranges from 40% to 100% and back.
 *
 * @author MIKROE SDK Team
 *
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
