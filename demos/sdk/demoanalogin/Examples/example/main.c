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
 *   Analog input
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   This example demonstrates the functionality of ADC (Analog-to-Digital Converter)
 *   using mikroSDK 2.0. The value read from the analog input pin is displayed on
 *   PORT A in the case of a 16-bit or 32-bit port, or on PORTs A and B in the case of an 8-bit port.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5044/analog-input-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "MikroSDK.Driver.ADC"
#include "MikroSDK.Board"
#include "MikroSDK.Driver.GPIO.Port"
#include "MikroSDK.Log"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

// -------------------------------------------------------------------- MACROS
// Return value in case of error.
#define RET_FAIL (-1)

// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_1_AN
#define MIKROBUS_1_AN NC
#endif

// Voltage reference value. It can be changed in CODEGRIP: POWER -> Outputs -> Voltage Reference -> Set Voltage.
#define ADC_VREF_VALUE 3.3f

// If hardware without PORT A is used, make sure to define adequate port.
#ifndef PORT_A
#define PORT_A NC
#endif

#define ADC_ERROR_VALUE 0xAA

#if (8==PORT_SIZE)
#define _8BIT_
#define PORT_MASK 0xFF
static port_t configured_port_higher;
// If hardware without PORT B is used, make sure to define adequate port.
#ifndef PORT_B
#define PORT_B NC
#endif
#else
#define _16_32BIT_
#define PORT_MASK 0xFFFF
#endif

// ------------------------------------------------------------------ VARIABLES
static analog_in_t analog_in;               // Analog input driver context structure.
static analog_in_config_t analog_in_cfg;    // Analog input configuration structure.

static port_t configured_port;

static log_t logger;
// ------------------------------------------------------------------ USER CODE
int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    log_cfg_t log_cfg;
    uint16_t read_data_buf; // Analog input data buffer.
    uint8_t num_of_readings = 50; // Demo ends after 50 readings.

    LOG_MAP_USB_UART( log_cfg );
    log_init( &logger, &log_cfg );

    #ifdef _8BIT_
    if( PORT_ERROR == port_init( &configured_port_higher, PORT_B, PORT_MASK, (pin_direction_t)GPIO_DIGITAL_OUTPUT )) {
        log_printf( &logger, "Port init failed.\n" );
        return RET_FAIL;
    }
    #endif
    if( PORT_ERROR == port_init( &configured_port, PORT_A, PORT_MASK, (pin_direction_t)GPIO_DIGITAL_OUTPUT )) {
        log_printf( &logger, "Port init failed.\n" );
        return RET_FAIL;
    }

    analog_in_configure_default( &analog_in_cfg );
    analog_in_cfg.input_pin  = MIKROBUS_1_AN;
    analog_in_cfg.resolution = ANALOG_IN_RESOLUTION_12_BIT;
    analog_in_cfg.vref_value = ADC_VREF_VALUE;

    if( ACQUIRE_FAIL == analog_in_open( &analog_in, &analog_in_cfg ) ) {
        // Error handling.
        log_printf( &logger, "ADC open failed.\n" );
        return RET_FAIL;
    }

    if( ADC_ERROR == analog_in_set_resolution( &analog_in, ANALOG_IN_RESOLUTION_12_BIT ) ) {
        // Error handling.
        log_printf( &logger, "ADC set resolution failed.\n" );
        return RET_FAIL;
    }

    if( ADC_ERROR == analog_in_set_vref_input( &analog_in, ANALOG_IN_VREF_EXTERNAL) ) {
        // Error handling.
        log_printf( &logger, "ADC set vref input failed.\n" );
        return RET_FAIL;
    }

    if( ADC_ERROR == analog_in_set_vref_value( &analog_in, ADC_VREF_VALUE ) ) {
        // Error handling.
        log_printf( &logger, "ADC set vref value failed.\n" );
        return RET_FAIL;
    }

    while( num_of_readings-- ) {
        if( ADC_ERROR == analog_in_read( &analog_in, &read_data_buf ) ) {
            if( PORT_ERROR == port_write( &configured_port, ADC_ERROR_VALUE )) {
               log_printf( &logger, "Port write failed.\n" );
               return RET_FAIL;
            }
            log_printf( &logger, "ADC read failed.\n" );
            return RET_FAIL;
        }

        if( PORT_ERROR == port_write( &configured_port, read_data_buf & PORT_MASK )) {
            log_printf( &logger, "Port write failed.\n" );
            return RET_FAIL;
        }
        #ifdef _8BIT_
        if( PORT_ERROR == port_write( &configured_port_higher, read_data_buf >> PORT_SIZE )) {
            log_printf( &logger, "Port write failed.\n" );
            return RET_FAIL;
        }
        #endif
        log_printf( &logger, "%.2f\n", (float)read_data_buf );
        Delay_100ms();
    }

    if( ADC_ERROR == analog_in_close( &analog_in )) {
        log_printf( &logger, "ADC close failed.\n" );
        return RET_FAIL;
    }

    log_printf( &logger, "End of ADC demo.\n" );

    return 0;
}
