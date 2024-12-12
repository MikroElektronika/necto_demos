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
 *   Led Curtain (The simple example).
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Simple Led curtain example, utilizing mikroSDK 2.0 Port driver library.
 *   The specified LEDs will be toggled. Take into consideration that your
 *   hardware might not have the specified port.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5016/led-curtain-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "drv_port.h"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

// If hardware without port `B` is used, make sure to define adequate port.
#ifndef PORT_B
#define PORT_B NC
#endif

#if (PORT_SIZE==16)
#define PORT_MASK 0xFFFF
#define START 0x8000
#define END 0x0001
#define PORT_MASK_HIGH 0xFF00
#define PORT_MASK_LOW 0x00FF
#else
#define PORT_MASK 0xFF
#define START 0x80
#define END 0x01
#define PORT_MASK_HIGH 0xF0
#define PORT_MASK_LOW 0x0F
#endif

static port_t configured_port;

static int counter = 0;
static int port_val;

int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Set PORTC as digital output.
    port_init( &configured_port, PORT_B, ( uint16_t )PORT_MASK, (pin_direction_t)GPIO_DIGITAL_OUTPUT );

    while( 1 ) {
        // TURN ON LEDs.
        for( counter = 0; counter < 8; counter++ ) {
            port_val |= ( START >> counter ) | ( END << counter );

            port_write( &configured_port, port_val );
            Delay_100ms();
        }

        // TURN OFF LEDs.
        for( counter = 0; counter < 8; counter++ ) {
            port_val &= ( PORT_MASK_HIGH << counter ) | ( PORT_MASK_LOW >> counter );

            port_write( &configured_port, port_val );
            Delay_100ms();
        }
    }

    return 0;
}
