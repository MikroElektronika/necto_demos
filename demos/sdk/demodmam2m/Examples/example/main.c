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
 *  DMA Memory to Memory demo
 * Copyright:
 *  (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *  Example is meant for demonstrating DMA Memory to Memory functionality
 *  using mikroSDK 2.0. If the DMA transfer was successful, assigned LED
 *  will be blinking, otherwise it will be turn on permanently.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5634/dma-memory-to-memory-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "MikroSDK.mikroSDKVersion"
#if mikroSDK_GET_VERSION < 21000ul
    #error "SDK version must be 2.10.0 or higher."
#endif

#include "drv_digital_out.h"
#include "drv_dma.h"
#include "board.h"
#include "delays.h"

// -------------------------------------------------------------------- MACROS

// TODO PB0 tested on TIVA PIC32 and PIC18. Confirm it for STM32, Kinetis and RISCV
#ifndef PB0
#define TEST_PIN_NAME HAL_PIN_NC
#else
#define TEST_PIN_NAME PB0
#endif

#define BUFFER_SIZE 32

#if defined(PIC18F57Q43)       || defined(PIC18F57K42) || \
    defined(PIC32MZ2048EFH144) || defined(PIC32MX795F512L) || \
    defined(GD32VF103VBT6)
    #define TEST_DMA_MODULE 0
    #define TEST_DMA_STREAM 0
    #define TEST_DMA_CHANNEL 0
#elif defined(TM4C129XNCZAD)
    #define TEST_DMA_MODULE 0
    #define TEST_DMA_STREAM 30
    #define TEST_DMA_CHANNEL 0
#elif defined(STM32F407ZG)
    #define TEST_DMA_MODULE 1
    #define TEST_DMA_STREAM 3
    #define TEST_DMA_CHANNEL 3
#elif defined(MK64FN1M0VDC12)
    #define TEST_DMA_MODULE 0
    #define TEST_DMA_STREAM 0
    #define TEST_DMA_CHANNEL 60
#else
    #define TEST_DMA_MODULE "provide the MCU appropriate module number"
    #define TEST_DMA_STREAM "provide the MCU appropriate steam number"
    #define TEST_DMA_CHANNEL "provide the MCU appropriate channel number"
#endif

// ----------------------------------------------------------------- VARIABLES

static dma_config_t dma_cfg;
static dma_t dma;
static dma_channel_t channels;

static digital_out_t pin;

int check_result = 0xFF;
static const uint32_t buffer_src_flash[ BUFFER_SIZE ]= {
    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80
};

static uint32_t buffer_dst_ram[ BUFFER_SIZE ];

// ----------------------------------------------------------------- USER CODE

/**
 * @brief Simple function that is called if something fails.
 * @details Function will turn the LED on to signal that something went wrong,
 *          and then enter infinite loop
 * @return Nothing.
*/
void test_fail();

int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    digital_out_init( &pin, TEST_PIN_NAME );
    digital_out_low( &pin );

    //------------------------------------------------------------------------
    // DMA settings
    //------------------------------------------------------------------------

    // Set default configuration values first.
    dma_configure_default( &dma_cfg );

    // Set data alignment to 1 byte.
    dma_cfg.data_align_source = DMA_DATA_ALIGN_BYTES_1;
    dma_cfg.data_align_destination = DMA_DATA_ALIGN_BYTES_1;

    // Set memory addresses to be incremented for both source and destination.
    dma_cfg.src_inc = true;
    dma_cfg.dst_inc = true;

    // Check if the desired DMA channel is free.
    dma_get_channels( &channels );
    if ( !channels[ TEST_DMA_MODULE ][ TEST_DMA_STREAM ][ TEST_DMA_CHANNEL ] ) {
        dma_cfg.module = TEST_DMA_MODULE;
        dma_cfg.stream = TEST_DMA_STREAM;
        dma_cfg.channel = TEST_DMA_CHANNEL;
    } else {
        test_fail();
    }

    // Initialize the DMA HW.
    if ( DMA_SUCCESS != dma_open( &dma, &dma_cfg ) ) {
        test_fail();
    }

    // Set priority to very high.
    if ( DMA_SUCCESS != dma_set_priority( &dma, DMA_PRIORITY_VERY_HIGH ) ) {
        test_fail();
    }

    // Set the DMA transfer direction to memory to memory.
    if ( DMA_SUCCESS != dma_set_direction( &dma, DMA_DIRECTION_MEMORY_TO_MEMORY ) ) {
        test_fail();
    }

    // Set the DMA transfer mode to normal.
    if ( DMA_SUCCESS != dma_set_mode( &dma, DMA_MODE_NORMAL ) ) {
        test_fail();
    }

    // Configure the DMA transfer source and destination addresses, as well as its size.
    if ( DMA_SUCCESS != dma_set_transfer_config(
                            &dma,
                            (uint32_t)&buffer_src_flash,
                            (uint32_t)&buffer_dst_ram,
                            sizeof( buffer_src_flash ),
                            DMA_SOURCE_MEMORY_REGION_PFM
                        ) ) {
        test_fail();
    }

    //------------------------------------------------------------------------
    // EOF DMA settings
    //------------------------------------------------------------------------

    // Start transmission.
    if ( DMA_SUCCESS != dma_transfer_start( &dma ) ) {
        test_fail();
    }

    // Disable the transfer when it is completed.
    dma_transfer_stop( &dma );

    // Check if transfer was completed successfully.
    check_result = memcmp( buffer_src_flash, buffer_dst_ram, sizeof( buffer_src_flash ) );

    // Deinitilize the DMA HW.
    dma_close( &dma );

    // Blink if the DMA transfer was completed successfully.
    if ( !check_result ) {
        while( 1 ) {
            digital_out_toggle( &pin );
            Delay_100ms();
        }
    } else {
        test_fail();
    }

    return 0;
}

void test_fail() {
    // Turn on the LED if something went wrong.
    digital_out_high( &pin );
    while( 1 );
}

// ----------------------------------------------------------------------- END
