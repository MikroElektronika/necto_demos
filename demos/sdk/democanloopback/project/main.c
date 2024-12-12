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
 *   CAN Loopback Mode Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating CAN Loopback Mode functionality using mikroSDK 2.10.0
 *   If pin that is defined as TEST_PIN_SUCCESS is blinking, then data transmission was a success.
 *   For NXP, PIC32 and RISC-V architectures you need to provide CAN signals with the same
 *   baud rate to HIGH and LOW CAN pins in order to make Loopback Mode work.
 *   If you don't set baud rate manually using can_set_frequency() API, 125000 will be set by default.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5624/can-loopback-mode-demo>
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
#include "MikroSDK.mikroSDKVersion"
#include "MikroSDK.Driver.GPIO.Out"
#include "MikroSDK.Driver.GPIO.Port"

#if mikroSDK_GET_VERSION < 21000ul
    #error "SDK version must be 2.10.0 or higher."
#endif

#if mikroSDK_GET_VERSION >= 21101
    #define UPDATED_CAN_IMPLEMENTATION
#endif

// -------------------------------------------------------------------- MACROS

#ifndef CAN_RX
#define CAN_RX HAL_PIN_NC // TODO define CAN RX pin
#endif
#ifndef CAN_TX
#define CAN_TX HAL_PIN_NC // TODO define CAN TX pin
#endif

#ifndef UPDATED_CAN_IMPLEMENTATION
// HW specific as TIVA architecture is using FIFO1 for transmitting.
#ifndef tiva
#define CAN_FILTER_FIFO_DEFAULT CAN_FILTER_FIFO1
#else
#define CAN_FILTER_FIFO_DEFAULT CAN_FILTER_FIFO2
#endif
#define CAN_FILTER_BANK_DEFAULT 0
#define CAN_BAUDRATE_VALUE      125000
#endif

#define TEST_PIN_CAN_RX CAN_RX // TODO define CAN RX pin
#define TEST_PIN_CAN_TX CAN_TX // TODO define CAN TX pin

// TODO Define test pins according to hardware.
#define TEST_PIN_1        HAL_PIN_NC
#define TEST_PIN_2        HAL_PIN_NC
#define TEST_PIN_3        HAL_PIN_NC
#define TEST_PIN_4        HAL_PIN_NC
#define TEST_PIN_5        HAL_PIN_NC
#define TEST_PIN_6        HAL_PIN_NC
#define TEST_PIN_7        HAL_PIN_NC
#define TEST_PIN_8        HAL_PIN_NC
#define TEST_PIN_SUCCESS  HAL_PIN_NC

// TODO Test different byte values with different filter configurations.
#define BYTE1 0x01
#define BYTE2 0x32
#define BYTE3 0xAB
#define BYTE4 0xCD
#define BYTE5 0x45
#define BYTE6 0x89
#define BYTE7 0xEF
#define BYTE8 0x67

#define signal_error(pin) digital_out_init( &test_pin, pin ); \
                          digital_out_high( &test_pin ); \
                          while(1)
#define signal_success    digital_out_high( &test_pin_success ); \
                          Delay_1sec(); \
                          digital_out_low( &test_pin_success ); \
                          Delay_1sec();

// ----------------------------------------------------------------- VARIABLES

static can_config_t can_config_struct;
static can_filter_config_t can_filter_config_struct;
static can_t can_struct;
static uint8_t byte_check[8];
can_transmit_message_struct tx_message;
can_receive_message_struct rx_message;
static digital_out_t test_pin, test_pin_success;

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
    can_filter_config_struct.can_filter_id = 0x0000123ul;
    can_filter_config_struct.can_filter_mask_id = 0x00000FFFul;
    can_filter_config_struct.can_filter_mode = CAN_FILTER_MODE_IDMASK;
    can_filter_config_struct.can_filter_fifo = CAN_FILTER_FIFO_DEFAULT;
    can_filter_config_struct.can_filter_bank = CAN_FILTER_BANK_DEFAULT;
    if( CAN_ERROR == can_set_filter( &can_struct, &can_filter_config_struct ) ) {
        signal_error( TEST_PIN_3 );
    }

    // Set the Loopback Mode operation.
    if( CAN_ERROR == can_set_mode( &can_struct, CAN_MODE_LOOPBACK )) {
        signal_error( TEST_PIN_4 );
    }

    #ifndef UPDATED_CAN_IMPLEMENTATION
    // Set the baud rate value.
    if( CAN_ERROR == can_set_frequency( &can_struct, CAN_BAUDRATE_VALUE )) {
        signal_error( TEST_PIN_5 );
    }
    #endif

    // Set bytes of data for transmission.
    tx_message.message.message_data[0] = BYTE1;
    tx_message.message.message_data[1] = BYTE2;
    tx_message.message.message_data[2] = BYTE3;
    tx_message.message.message_data[3] = BYTE4;
    tx_message.message.message_data[4] = BYTE5;
    tx_message.message.message_data[5] = BYTE6;
    tx_message.message.message_data[6] = BYTE7;
    tx_message.message.message_data[7] = BYTE8;
    tx_message.message.data_len = 8;
    // Set the format and ID of the transmit message.
    tx_message.message.frame_format = CAN_FRAME_FORMAT_STANDARD_11BITS;
    tx_message.message.std_id = 0x48cUL;
    tx_message.message.ext_id = 0x01;
    // Send the data.
    if( CAN_ERROR == can_transmit( &can_struct, &tx_message )) {
        signal_error( TEST_PIN_6 );
    }

    Delay_100ms();

    // Define an RX FIFO that should have the received message in it.
    // NOTE if no data was received by this FIFO can_receive will return error.
    rx_message.rx_fifo_number = CAN_FILTER_FIFO_DEFAULT;
    // Get the data.
    if( CAN_ERROR == can_receive( &can_struct, &rx_message )) {
        signal_error( TEST_PIN_7 );
    }

    Delay_100ms();

    digital_out_init( &test_pin_success, TEST_PIN_SUCCESS );

    // Define an array for checking received bytes.
    byte_check[0] = BYTE1;
    byte_check[1] = BYTE2;
    byte_check[2] = BYTE3;
    byte_check[3] = BYTE4;
    byte_check[4] = BYTE5;
    byte_check[5] = BYTE6;
    byte_check[6] = BYTE7;
    byte_check[7] = BYTE8;

    // Check if received data is the same as expected.
    if( !memcmp( &byte_check, &rx_message.message.message_data, sizeof( byte_check )))
    {
        while(1) {
            signal_success;
        }
    }
    else {
        signal_error( TEST_PIN_8 );
    }

    return 0;
}

// ----------------------------------------------------------------------- END
