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
 *   CAN Transmit Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating CAN Transmit functionality using mikroSDK 2.10.0 and higher versions.
 *   This program works in pair with CAN Receive Demo. For this example to work properly,
 *   please follow the steps listed on the link below.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5632/can-transmit-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "MikroSDK.Driver.CAN"
#include "MikroSDK.Driver.GPIO.Out"
#include "MikroSDK.Driver.GPIO.In"
#include "MikroSDK.Driver.GPIO.Port"
#include "MikroSDK.Board"
#include "MikroSDK.mikroSDKVersion"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

#if mikroSDK_GET_VERSION < 21000ul
    #error "SDK version must be 2.10.0 or higher."
#endif

// -------------------------------------------------------------------- MACROS
#if defined(CAN_RX) && defined(CAN_TX)
#define TEST_PIN_CAN_RX CAN_RX
#define TEST_PIN_CAN_TX CAN_TX
#else
#define TEST_PIN_CAN_RX HAL_PIN_NC // TODO define CAN RX pin
#define TEST_PIN_CAN_TX HAL_PIN_NC // TODO define CAN TX pin
#endif

// TODO Define test pins according to hardware.
#define TEST_PIN_1  HAL_PIN_NC
#define TEST_PIN_2  HAL_PIN_NC
#define TEST_PIN_3  HAL_PIN_NC
#define TEST_PIN_4  HAL_PIN_NC
#define TEST_PIN_5  HAL_PIN_NC

#define BUTTON_STOP_TRANSMISSION HAL_PIN_NC // TODO define input pin to be used for stopping transmission

/*
 * IMPORTANT: Devices can communicate properly over CAN only if the same frequency is set on all nodes.
 *            If you change this value, make the same change in CAN Receive Demo example.
 */
#define CAN_FREQUENCY 125000

#define signal_error(pin) digital_out_init( &test_pin, pin ); \
                          digital_out_high( &test_pin ); \
                          while(1)
// ----------------------------------------------------------------- VARIABLES
static digital_in_t input_pin;
static digital_out_t test_pin;
static can_config_t can_config_struct;
static can_filter_config_t can_filter_config_struct;
static can_t can_struct;
uint8_t tx_data[3];
uint8_t data_byte;
can_transmit_message_struct tx_message;
// ----------------------------------------------------------------- USER CODE
int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Default config.
    can_configure_default( &can_config_struct );
    can_filter_configure_default( &can_filter_config_struct );

    can_config_struct.rx_pin = TEST_PIN_CAN_RX;
    can_config_struct.tx_pin = TEST_PIN_CAN_TX;
    if( ACQUIRE_FAIL == can_open( &can_struct, &can_config_struct )) {
        signal_error( TEST_PIN_1 );
    }

    if( CAN_ERROR == can_init( &can_struct )) {
        signal_error( TEST_PIN_2 );
    }

    // It may not be possible to reach the desired frequency with the set MCU speed.
    // In that case, this function returns CAN_ERROR.
    if( CAN_ERROR == can_set_frequency( &can_struct, CAN_FREQUENCY )) {
        signal_error( TEST_PIN_4 );
    }

    // Button for stopping transmission initiation.
    digital_in_init( &input_pin, BUTTON_STOP_TRANSMISSION );

    tx_message.message.data_len = 3;
    tx_message.message.frame_format = CAN_FRAME_FORMAT_STANDARD_11BITS;
    tx_message.message.std_id = 0x123;
    while(1) {
            tx_message.message.message_data[0] = 0xAA;
            tx_message.message.message_data[1] = data_byte++;
            tx_message.message.message_data[2] = 0xBB;

            /* Send the data */
            if( CAN_ERROR == can_transmit( &can_struct, &tx_message )) {
                signal_error( TEST_PIN_5 );
            }

            // By pressing the button transmission should stop for 3 seconds.
            if ( digital_in_read( &input_pin )) {
                can_transmission_stop( &can_struct );
                Delay_ms(3000);
            }

            Delay_10ms();
    }

    return 0;
}

// ----------------------------------------------------------------------- END
