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

#ifndef MOTOR_DEMO_H
#define MOTOR_DEMO_H

/*!
 * @file motor_demo.h
 * @brief This file contains API and structures for Renesas boards configuration for Bare Metal Motor Demo.
 */

/**
* Any initialization code needed for MCU to function properly.
* Do not remove this line or clock might not be set correctly.
*/
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "mcu.h"

typedef enum {
    MOTOR_DIRECTION_CW = 0,
    MOTOR_DIRECTION_CCW
} motor_direction_t;

typedef enum {
    MOTOR_SPEED_DECREASE = 0,
    MOTOR_SPEED_INCREASE
} motor_speed_t;

/**
 * @brief Motor rotation direction configuration function.
 * @details This function sets direction for motor rotation.
 * @param[in] direction : Direction enum handler.
 * See #motor_direction_t object definition for detailed explanation.
 *
 * @return None
 */
void set_motor_direction( motor_direction_t direction );

/**
 * @brief Motor rotation speed configuration function.
 * @details This function sets motor rotation speed.
 * @param[in] duration : Sweep duration.
 * @param[in] mode : Sweep mode - increasing or decreasing sweep speed.
 * See #motor_speed_t object definition for detailed explanation.
 *
 * @return None
 */
void pwm_sweep( float duration, motor_speed_t mode );

/**
 * @brief Mikrobus configuration function.
 * @details This function configures necessary Mikrobus pins based on
 * the board that is being used.
 *
 * @return None
 */
void configure_mikrobus();

#endif
