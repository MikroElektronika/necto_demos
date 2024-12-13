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
 *   Digital Input Output
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   This example demonstrates digital input and digital output functionalities.
 *   Turn on switches for PORT LEDS and BUTTONS in the BOARD SETUP section of your board
 *   and see how the LEDs respond to pushing adequate buttons.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5047/digital-input-output-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "digital_in_out.h"

// ------------------------------------------------------------------ VARIABLES
static digital_in_t input_pin_0;    // Digital input driver context structure.
static digital_in_t input_pin_1;    // Digital input driver context structure.
static digital_in_t input_pin_2;    // Digital input driver context structure.
static digital_in_t input_pin_3;    // Digital input driver context structure.
static digital_in_t input_pin_4;    // Digital input driver context structure.

static digital_out_t output_pin_0;  // Digital output driver context structure.
static digital_out_t output_pin_1;  // Digital output driver context structure.
static digital_out_t output_pin_2;  // Digital output driver context structure.
static digital_out_t output_pin_3;  // Digital output driver context structure.
static digital_out_t output_pin_4;  // Digital output driver context structure.

static outputs_t out_pins;

/**
 * @brief This function initializes pins A0 - A4 as output pins and pins B0 - B4 as input.
 */
void pin_init( void ) {
    // Initializes digital output driver context structure and individual GPIO pin as digital outputs.
    digital_out_init( &output_pin_0, LED_0 );
    digital_out_init( &output_pin_1, LED_1 );
    digital_out_init( &output_pin_2, LED_2 );
    digital_out_init( &output_pin_3, LED_3 );
    digital_out_init( &output_pin_4, LED_4 );
    // Initializes digital input driver context structure and individual GPIO pin as digital inputs.
    digital_in_init( &input_pin_0, BUTTON_0 );
    digital_in_init( &input_pin_1, BUTTON_1 );
    digital_in_init( &input_pin_2, BUTTON_2 );
    digital_in_init( &input_pin_3, BUTTON_3 );
    digital_in_init( &input_pin_4, BUTTON_4 );

    out_pins.out0 = output_pin_0;
    out_pins.out1 = output_pin_1;
    out_pins.out2 = output_pin_2;
    out_pins.out3 = output_pin_3;
    out_pins.out4 = output_pin_4;
}

// ------------------------------------------------------------------ USER CODE
int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    pin_init();

    while( 1 ) {
        if(digital_in_read( &input_pin_0 )) {
            pins_low( &out_pins );
        }
        if( digital_in_read( &input_pin_1 )) {
            pins_high( &out_pins );
        }
        if( digital_in_read( &input_pin_2 )) {
            pins_toggle( &out_pins );
        }
        if( digital_in_read( &input_pin_3 )) {
            digital_out_write( &output_pin_3, PIN_HIGH );
        }
        if( digital_in_read( &input_pin_4 )) {
            digital_out_write( &output_pin_4, PIN_LOW );
        }
    }

    return 0;
}

// ------------------------------------------------------------------------ END
