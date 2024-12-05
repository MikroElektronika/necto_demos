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
 *   Port Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Simple Port example, utilizing mikroSDK 2.0 GPIO port library.
 *   All defined ports will be toggled.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5045/port-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "board.h"
#include "drv_port.h"
#include "ports.h"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

/*
 * PORT_NUM is generated as number of all ports defined in mikroSDK in mcu_definitions.h file for selected MCU.
 *
 */

#if (PORT_SIZE==8)
#define PORT_MASK 0xFF
#define VALUE_ON 0xFF
#define VALUE_OFF 0x00
#else
#define PORT_MASK 0xFFFF
#define VALUE_ON 0xFFFF
#define VALUE_OFF 0x0000
#endif

// ------------------------------------------------------------------ VARIABLES
static port_t port; // Port driver context structure.

// ------------------------------------------------------------------ USER CODE
int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    int i, j;

    while( 1 ) {
        for( i = 0; i < PORT_NUM; i++ ) {
            port_init( &port, PORT_ARRAY[i], PORT_MASK, (pin_direction_t)PIN_DIRECTION_DIGITAL_OUTPUT );
            port_write( &port, VALUE_ON );
            Delay_ms(500);
        }

        for( j = PORT_NUM-1; j >= 0; j-- ) {
            port_init( &port, PORT_ARRAY[j], PORT_MASK, (pin_direction_t)PIN_DIRECTION_DIGITAL_OUTPUT );
            port_write( &port, VALUE_OFF );
            Delay_ms(500);
        }
    }

    return 0;
}
