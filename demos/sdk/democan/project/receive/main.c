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
 *   CAN Receive Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating CAN Receive functionality using mikroSDK 2.10.0 and higher versions.
 *   If data is displayed on the port defined as TEST_PORT, then data reception was a success.
 *   For some architectures terminal resistance is needed on CANH and CANL pins.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5633/can-receive-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

#include "MikroSDK.Board"
#include "MikroSDK.Driver.CAN"
#include "MikroSDK.Driver.GPIO.Out"
#include "MikroSDK.Driver.GPIO.Port"
#include "MikroSDK.mikroSDKVersion"

// -------------------------------------------------------------------- MACROS

#if mikroSDK_GET_VERSION < 21000ul
    #error "SDK version must be 2.10.0 or higher."
#endif

#if mikroSDK_GET_VERSION >= 21101
    #define UPDATED_CAN_IMPLEMENTATION
#endif

#if defined(CAN_RX) && defined(CAN_TX)
#define TEST_PIN_CAN_RX CAN_RX
#define TEST_PIN_CAN_TX CAN_TX
#else
#define TEST_PIN_CAN_RX HAL_PIN_NC // TODO define CAN RX pin
#define TEST_PIN_CAN_TX HAL_PIN_NC // TODO define CAN TX pin
#endif

#ifndef UPDATED_CAN_IMPLEMENTATION
#define CAN_FILTER_FIFO_DEFAULT CAN_FILTER_FIFO_NUM
#define CAN_FILTER_BANK_DEFAULT 0
#endif

// TODO Define test pins according to hardware.
#define TEST_PIN_1  HAL_PIN_NC
#define TEST_PIN_2  HAL_PIN_NC
#define TEST_PIN_3  HAL_PIN_NC
#define TEST_PIN_4  HAL_PIN_NC
#define TEST_PIN_5  HAL_PIN_NC
#define TEST_PIN_6  HAL_PIN_NC
#define TEST_PORT   HAL_PORT_NC

/*
 * IMPORTANT: Devices can communicate properly over CAN only if the same frequency is set on all nodes.
 *            If you change this value, make the same change in CAN Transmit Demo example.
 */
#define CAN_FREQUENCY 125000
#define PORT_MASK     0xFF

#define signal_error(pin) digital_out_init( &test_pin, pin ); \
                          digital_out_high( &test_pin ); \
                          while(1)
#define signal_receive_error(pin) digital_out_init( &test_pin, pin ); \
                                  digital_out_high( &test_pin ); \
                                  Delay_ms(3000); \
                                  digital_out_low( &test_pin );
// ----------------------------------------------------------------- VARIABLES

static digital_out_t test_pin;
static port_t port;
static can_config_t can_config_struct;
static can_filter_config_t can_filter_config_struct;
static can_t can_struct;
can_receive_message_struct rx_message;

// ----------------------------------------------------------------- USER CODE
int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Set default configuration for CAN module.
    can_configure_default( &can_config_struct );
    can_filter_configure_default( &can_filter_config_struct );

    // Check defined pins and get the CAN module number.
    can_config_struct.rx_pin = TEST_PIN_CAN_RX;
    can_config_struct.tx_pin = TEST_PIN_CAN_TX;
    if( ACQUIRE_FAIL == can_open( &can_struct, &can_config_struct )) {
        signal_error( TEST_PIN_1 );
    }

    // Initialize the CAN module.
    if( CAN_ERROR == can_init( &can_struct )) {
        signal_error( TEST_PIN_2 );
    }

    // Set CAN filer parameters.
    can_filter_config_struct.can_filter_scale = CAN_FILTER_SCALE_32BIT;
    // NOTE: If you don't have any filters enabled for FIFO, message won't be received.
    can_filter_config_struct.can_filter_enable = CAN_FILTER_ENABLE;
    can_filter_config_struct.can_filter_frame_type[0] = CAN_FRAME_TYPE_DATA;
    can_filter_config_struct.can_filter_frame_type[1] = CAN_FRAME_TYPE_DATA;
    can_filter_config_struct.can_filter_id = 0x00000123ul;
    can_filter_config_struct.can_filter_mask_id = 0x00000FFFul;
    can_filter_config_struct.can_filter_mode = CAN_FILTER_MODE_IDMASK;
    can_filter_config_struct.can_filter_fifo = CAN_FILTER_FIFO_DEFAULT;
    can_filter_config_struct.can_filter_bank = CAN_FILTER_BANK_DEFAULT;
    if( CAN_ERROR == can_set_filter( &can_struct, &can_filter_config_struct ) ) {
        signal_error( TEST_PIN_3 );
    }

    // Set the Normal Mode operation.
    if( CAN_ERROR == can_set_mode( &can_struct, CAN_MODE_NORMAL)) {
        signal_error( TEST_PIN_4 );
    }

    // Set the baud rate value.
    if( CAN_ERROR == can_set_frequency( &can_struct, CAN_FREQUENCY )) {
        signal_error( TEST_PIN_5 );
    }

    port_init( &port, TEST_PORT, PORT_MASK, PIN_DIRECTION_DIGITAL_OUTPUT );

    Delay_ms(1000);

    // Define an RX FIFO that should have the received message in it.
    // NOTE if no data was received by this FIFO can_receive will return error.
    rx_message.rx_fifo_number = CAN_FILTER_FIFO_DEFAULT;
    while(1) {
        // Get the data.
        if( CAN_ERROR != can_receive( &can_struct, &rx_message )) {
            if( rx_message.message.message_data[0] == 0xAA && rx_message.message.message_data[2] == 0xBB ) {
                // Display received data on the port.
                port_write( &port, rx_message.message.message_data[1] );
            } else {
                signal_receive_error( TEST_PIN_6 );
            }
        }
    }

    return 0;
}

// ----------------------------------------------------------------------- END
