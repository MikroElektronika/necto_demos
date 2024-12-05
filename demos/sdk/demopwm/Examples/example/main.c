/****************************************************************************
** Copyright (C) ${COPYRIGHT_YEAR} MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** This file is part of the mikroSDK package
**
** Commercial License Usage
**
** Licensees holding valid commercial NECTO compilers AI licenses may use this
** file in accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The mikroElektronika Company.
** For licensing terms and conditions see
** https://www.mikroe.com/legal/software-license-agreement.
** For further information use the contact form at
** https://www.mikroe.com/contact.
**
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used for
** non-commercial projects under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** OF MERCHANTABILITY, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
** TO THE WARRANTIES FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
** OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
/* Project name:
 *   PWM Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   This program demonstrates how to enable PWM module and its output pin
 *   correctly and how to change PWM duty by pressing a single button.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5046/pwm-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "drv_pwm.h"
#include "board.h"
#include "drv_digital_in.h"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_1_PWM
#define MIKROBUS_1_PWM NC
#endif

// Make sure to define adequate pins.
#ifndef PA0
#define BUTTON1 NC
#else
#define BUTTON1 PA0
#endif

#ifndef PB0
#define BUTTON2 NC
#else
#define BUTTON2 PB0
#endif

#define PWM_FREQ_HZ 1000         // PWM frequency in Hz.

// ------------------------------------------------------------------ VARIABLES
static pwm_t pwm;                // PWM driver context structure.
static pwm_config_t pwm_cfg;     // PWM driver configuration structure.
static digital_in_t input_pin1;
static digital_in_t input_pin2;

// ------------------------------------------------------------------ USER CODE
int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    uint8_t button1_value;
    uint8_t button2_value;
    uint8_t duty = 0;

    // Initializes digital input driver context structure and individual GPIO pin as digital input.
    digital_in_init( &input_pin1, BUTTON1 );
    digital_in_init( &input_pin2, BUTTON2 );

    // Initializes PWM configuration structure to default values.
    pwm_configure_default( &pwm_cfg );

    // Set PWM pin.
    pwm_cfg.pin = MIKROBUS_1_PWM;

    // Allocate resources for PWM module.
    if( PWM_ERROR == pwm_open( &pwm, &pwm_cfg )) {
        // Error handling.
        return PWM_ERROR;
    }

    // Set PWM frequency and initialize PWM module on hardware level.
    pwm_set_freq( &pwm, PWM_FREQ_HZ );

    // Start PWM module.
    pwm_start( &pwm );

    while(1) {
        button1_value = digital_in_read( &input_pin1 );
        button2_value = digital_in_read( &input_pin2 );

        if( 1 == button1_value ) {
            if( duty < 100 ) {
                duty++;
                pwm_set_duty( &pwm, (float)duty / 100 );
                Delay_ms(5);
            } else {
                duty = 100;
            }
        }

        if( 1 == button2_value ) {
            if( duty > 0 && duty <= 100 ) {
                duty--;
                pwm_set_duty( &pwm, (float)duty / 100 );
                Delay_ms(5);
            }
            else {
                duty = 0;
            }
        }
    }

    return PWM_SUCCESS;
}
