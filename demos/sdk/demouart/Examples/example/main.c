/****************************************************************************
** Copyright (C) ${COPYRIGHT_YEAR} MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** This file is part of the mikroSDK package.
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
 *  UART demo
 * Copyright:
 *  (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *  Example is meant for demonstrating UART functionality using mikroSDK 2.0.
 *  If you're using one of the MIKROE v8 boards, make sure to plug in a USB Type C
 *  cable into USB-UART port. Otherwise you'll need the USB UART click. If the latter,
 *  place it in a mikroBUS socket on the board you're using.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5396/uart-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "MikroSDK.Driver.GPIO.Out"
#include "MikroSDK.Driver.UART"
#include "MikroSDK.Board"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

// -------------------------------------------------------------------- MACROS
// Return value in case of error.
#define RET_FAIL (-1)

// If hardware without USB_UART port is used, make sure to define adequate pin.
#ifndef USB_UART_TX
#define USB_UART_TX NC
#endif

// If hardware without USB_UART port is used, make sure to define adequate pin.
#ifndef USB_UART_RX
#define USB_UART_RX NC
#endif

#define TEST_PIN_UART_TX USB_UART_TX
#define TEST_PIN_UART_RX USB_UART_RX

// Define test pins yourself. If an error happens, assigned pin will go high and the program will stop there.
#define TEST_PIN_1 NC
#define TEST_PIN_2 NC
#define TEST_PIN_3 NC
#define TEST_PIN_4 NC
#define TEST_PIN_5 NC
#define TEST_PIN_6 NC
#define TEST_PIN_7 NC
#define TEST_PIN_8 NC
#define TEST_PIN_9 NC
#define TEST_PIN_10 NC
#define TEST_PIN_11 NC
#define TEST_PIN_12 NC
#define TEST_PIN_13 NC
#define TEST_PIN_14 NC

#define signal_error(pin) digital_out_init( &test_pin, pin ); \
                          digital_out_high( &test_pin ); \
                          while(1)

// ----------------------------------------------------------------- VARIABLES
static uart_t uart;  // UART driver context structure.
static uart_config_t uart_cfg;  // UART driver config context structure.

static uint8_t uart_rx_buffer[128];
static uint8_t uart_tx_buffer[128];
static uint8_t buffer[32];

static int32_t read_size = 0;

static digital_out_t test_pin;
// ----------------------------------------------------------------- USER CODE
int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    if( TEST_PIN_UART_TX == HAL_PIN_NC || TEST_PIN_UART_RX == HAL_PIN_NC ) {
        printf_me( "Pins are not defined!\n" );
        signal_error( TEST_PIN_1 );
    }

    // Default config
    uart_configure_default( &uart_cfg );

    // Ring buffer mapping
    uart.tx_ring_buffer = uart_tx_buffer;
    uart.rx_ring_buffer = uart_rx_buffer;

    uart_cfg.tx_pin = TEST_PIN_UART_TX;  // UART TX pin.
    uart_cfg.rx_pin = TEST_PIN_UART_RX;  // UART RX pin.

    uart_cfg.tx_ring_size = sizeof( uart_tx_buffer );
    uart_cfg.rx_ring_size = sizeof( uart_rx_buffer );

    if( ACQUIRE_FAIL == uart_open( &uart, &uart_cfg ) ) {
        printf_me( "UART open failed.\n" );
        signal_error( TEST_PIN_2 );
    }

    //------------------------------------------------------------------------
    // UART settings
    //------------------------------------------------------------------------

    // Set baud rate.
    if ( UART_SUCCESS != uart_set_baud( &uart, 115200 ) ) {
        printf_me( "UART set baud failed.\n" );
        signal_error( TEST_PIN_3 );
    }

    // Set data parity. ( no parity )
    if ( UART_SUCCESS != uart_set_parity( &uart, UART_PARITY_DEFAULT ) ) {
        printf_me( "UART set parity failed.\n" );
        signal_error( TEST_PIN_4 );
    };

    // Set stop bits. ( one stop bit )
    if ( UART_SUCCESS != uart_set_stop_bits( &uart, UART_STOP_BITS_DEFAULT ) ) {
        printf_me( "UART set stop bits failed.\n" );
        signal_error( TEST_PIN_5 );
    }

    // Set data bits. ( 8-bit data )
    if ( UART_SUCCESS != uart_set_data_bits( &uart, UART_DATA_BITS_DEFAULT ) ) {
        printf_me( "UART set data bits failed.\n" );
        signal_error( TEST_PIN_6 );
    }

    Delay_100ms();

    //------------------------------------------------------------------------
    // EOF UART settings
    //------------------------------------------------------------------------

    // Write out data with new line.
    if ( UART_ERROR == uart_println( &uart, "UART DEMO" ) ) {
        printf_me( "UART print failed.\n" );
        signal_error( TEST_PIN_7 );
    }

    // Delay needed because of `uart_clear` function, so all data is transmitted.
    Delay_100ms();

    // Write out data.
    if ( UART_ERROR == uart_print( &uart, "Hello!" ) ) {
        printf_me( "UART println failed.\n" );
        signal_error( TEST_PIN_8 );
    }

    // Print new line.
    if ( UART_ERROR == uart_println( &uart, "" ) ) {
        printf_me( "UART print failed.\n" );
        signal_error( TEST_PIN_9 );
    }

    // Delay needed because of `uart_clear` function, so all data is transmitted.
    Delay_100ms();

    // Clear rx and tx buffers.
    uart_clear( &uart );

    // Check if buffers have been cleared
    if ( uart.config.tx_buf.head ||
         uart.config.tx_buf.tail ||
         uart.config.tx_buf.size )
    {
        printf_me( "UART TX buffer has not been cleared.\n" );
        signal_error( TEST_PIN_10 );
    }
    if ( uart.config.rx_buf.head ||
         uart.config.rx_buf.tail ||
         uart.config.rx_buf.size )
    {
        printf_me( "UART RX buffer has not been cleared.\n" );
        signal_error( TEST_PIN_11 );
    }

    // Set blocking mode.
    uart_set_blocking( &uart, true );

    // Infinite number of reads - echo example.
    while(1)
    {
        read_size = uart_read( &uart, buffer, sizeof( buffer ) );  // Read data.

        if ( read_size > 0 )  // If anything was read.
        {
            read_size = uart_write( &uart, buffer, read_size );  // Write what you read.
        } else {
            printf_me( "UART read failed.\n" );
            signal_error( TEST_PIN_12 );
        }

        if ( uart.is_blocking && ( UART_ERROR == read_size ) ) {
            printf_me( "UART write failed.\n" );
            signal_error( TEST_PIN_13 );
        }
    }

    return 0;
}

// ----------------------------------------------------------------------- END
