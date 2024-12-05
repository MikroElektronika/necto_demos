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
 *   SPI demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating SPI functionality using mikroSDK 2.0.
 *   Make sure to place SRAM click in mikroBUS1 and TESTER click in mikroBUS2.
 *   Pin MIKROBUS_2_INT is set to high if transfer was successful. Written and read data
 *   is also printed out on the Standard output (determined by the chosen setup).
 *   Use a logic analyzer for more detailed insight into the trasfer details.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5397/spi-demo>
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
#include "MikroSDK.Board"
#include "MikroSDK.Log"
#include "MikroSDK.Driver.SPI.Master"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

// -------------------------------------------------------------------- MACROS
// Return value in case of error.
#define RET_FAIL (-1)
#define RET_SUCCESS (0)

// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_1_CS
#define MIKROBUS_1_CS NC
#endif
// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_1_SCK
#define MIKROBUS_1_SCK NC
#endif
// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_1_MISO
#define MIKROBUS_1_MISO NC
#endif
// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_1_MOSI
#define MIKROBUS_1_MOSI NC
#endif
// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_2_INT
#define MIKROBUS_2_INT NC
#endif

// SPI pins
#define TEST_PIN_SPI_CS   MIKROBUS_1_CS
#define TEST_PIN_SPI_SCK  MIKROBUS_1_SCK
#define TEST_PIN_SPI_MISO MIKROBUS_1_MISO
#define TEST_PIN_SPI_MOSI MIKROBUS_1_MOSI

// SRAM Click specific macros.
#define DATA_LENGTH (20)

// WRITE instruction
#define SRAM_CLICK_WRITE_CMD (2)

// READ instruction
#define SRAM_CLICK_READ_CMD (3)

// Write sequence consists of:
// WRITE instruction, 24-bit address (3 bytes) and data to be written (5 bytes total)
#define SRAM_CLICK_WRITE_DATA_LENGTH (5)

// Read sequence consists of:
// READ instruction and 24-bit address (3 bytes) (4 bytes total)
#define SRAM_CLICK_WRITE_READ_DATA_LENGTH (4)

// Dummy write data for this click
#define SRAM_CLICK_DEFAULT_WRITE_DATA (0)

// SPI speed
#define SRAM_CLICK_SPEED (100000)

#define signal_success(port,pin) digital_out_init( &port, pin ); \
                                 digital_out_write( &port, 1 )

// ----------------------------------------------------------------- VARIABLES
static digital_out_t output_pin;

static spi_master_t sram_click;
static spi_master_config_t sram_click_config;

// SRAM Click specific variables.
// Array for data to be written - SRAM Click.
static uint8_t write_buffer[DATA_LENGTH];

// Array for data to be read - SRAM Click.
static uint8_t read_buffer[DATA_LENGTH];

static log_t logger;
static log_cfg_t logger_cfg;
// ----------------------------------------------------------------- USER CODE
// SRAM Click Write procedure.
int8_t sram_click_write(uint32_t address, uint8_t data_to_be_written) {
    // Local array for storing appropriate data, which will be sent to SRAM Click.
    uint8_t local_array[SRAM_CLICK_WRITE_DATA_LENGTH];

    memset( &local_array, sizeof( local_array ), 0 );

    // Write sequence - WRITE instruction, 24-bit address and data to be written.
    local_array[0] = SRAM_CLICK_WRITE_CMD;
    local_array[1] = address >> 16;
    local_array[2] = address >> 8;
    local_array[3] = address;
    local_array[4] = data_to_be_written;

    // Select SRAM Click.
    spi_master_select_device(TEST_PIN_SPI_CS);

    // Write data to SRAM Click.
    if( SPI_MASTER_ERROR == spi_master_write(&sram_click, local_array, SRAM_CLICK_WRITE_DATA_LENGTH)) {
        log_printf( &logger, "SPI master write failed.\n" );
        return RET_FAIL;
    }

    // Deselect SRAM Click.
    spi_master_deselect_device(TEST_PIN_SPI_CS);

    return RET_SUCCESS;
}

// SRAM Click Read procedure.
int8_t sram_click_read( uint32_t address ) {
    // Local array for storing appropriate data, which will be sent to SRAM Click.
    uint8_t local_array[4];

    memset( &local_array, sizeof( local_array ), 0 );

    // Read sequence - READ instruction and 24-bit address.
    local_array[0] = SRAM_CLICK_READ_CMD;
    local_array[1] = address >> 16;
    local_array[2] = address >> 8;
    local_array[3] = address;

    // Select SRAM Click.
    spi_master_select_device(TEST_PIN_SPI_CS);

    // Write data to SRAM Click.
    if( SPI_MASTER_ERROR == spi_master_write_then_read( &sram_click, local_array, SRAM_CLICK_WRITE_READ_DATA_LENGTH, read_buffer, DATA_LENGTH )) {
        log_printf( &logger, "SPI master write then read failed.\n" );
        return RET_FAIL;
    }

    // Deselect SRAM Click.
    spi_master_deselect_device( TEST_PIN_SPI_CS );

    return RET_SUCCESS;
}

// Application initialization procedure.
int8_t application_init() {
    // Initialize logger.
    LOG_MAP_USB_UART( logger_cfg );
    log_init( &logger, &logger_cfg );

    // Set default properties for SRAM Click.
    spi_master_configure_default( &sram_click_config );

    // Set desired basic properties for SRAM Click which is on mikroBUS1.
    sram_click_config.sck = TEST_PIN_SPI_SCK;
    sram_click_config.miso = TEST_PIN_SPI_MISO;
    sram_click_config.mosi = TEST_PIN_SPI_MOSI;
    sram_click_config.speed = SRAM_CLICK_SPEED;
    sram_click_config.mode = SPI_MASTER_MODE_DEFAULT;
    sram_click_config.default_write_data = SRAM_CLICK_DEFAULT_WRITE_DATA;

    if( ACQUIRE_FAIL == spi_master_open(&sram_click, &sram_click_config)) {
        log_printf( &logger, "SPI master open failed.\n" );
        return RET_FAIL;
    }

    // Set Chip Select polarity (SRAM Click requires active low).
    spi_master_set_chip_select_polarity(SPI_MASTER_CHIP_SELECT_DEFAULT_POLARITY);

    // Set desired default write (dummy) data.
    if( SPI_MASTER_ERROR == spi_master_set_default_write_data(&sram_click, SRAM_CLICK_DEFAULT_WRITE_DATA)) {
        log_printf( &logger, "SPI master set default write failed.\n" );
        return RET_FAIL;
    }

    // Set desired baud rate (speed).
    if( SPI_MASTER_ERROR == spi_master_set_speed(&sram_click, SRAM_CLICK_SPEED)) {
        log_printf( &logger, "SPI master set speed failed.\n" );
        return RET_FAIL;
    }

    // Set desired mode (SRAM Click requires mode 0).
    if( SPI_MASTER_ERROR == spi_master_set_mode(&sram_click, SPI_MASTER_MODE_DEFAULT)) {
        log_printf( &logger, "SPI master set mode failed.\n" );
        return RET_FAIL;
    }

    return RET_SUCCESS;
}

// Application task procedure.
int8_t application_task() {
    uint8_t address_and_data = 0;

    log_printf( &logger, "Written data:\n");
    // Write sequence - SRAM Click.
    for (address_and_data = 0; address_and_data < DATA_LENGTH; address_and_data++) {

        // Write data via SPI protocol.
        if( RET_FAIL == sram_click_write(address_and_data, address_and_data)) {
            log_printf( &logger, "SPI master write failed.\n" );
            return RET_FAIL;
        }

        write_buffer[address_and_data] = address_and_data;

        if( DATA_LENGTH - 1 != address_and_data ) {
            log_printf( &logger, "%d, ", (uint16_t)write_buffer[address_and_data] );
        } else {
            log_printf( &logger, "%d\n", (uint16_t)write_buffer[address_and_data] );
        }

        Delay_ms(1);
    }

    // Read data from SRAM Click.
    if( RET_FAIL == sram_click_read(0)) {
        log_printf( &logger, "SPI master read failed.\n" );
        return RET_FAIL;
    }

    log_printf( &logger, "Read data:\n" );
    for( address_and_data = 0; address_and_data < DATA_LENGTH; address_and_data++ ) {
        if( DATA_LENGTH - 1 != address_and_data ) {
            log_printf( &logger, "%d, ", (uint16_t)read_buffer[address_and_data] );
        } else {
            log_printf( &logger, "%d\n", (uint16_t)read_buffer[address_and_data] );
        }
    }

    /*
     *  memcmp compares buffers and returns 0 if they are the same.
     *  So, here, if it does return 0, it means that the transfer
     *  was successful and signal_success should set output_pin to high.
     */
    if ( !memcmp(write_buffer, read_buffer, sizeof( read_buffer ))) {
        signal_success( output_pin, MIKROBUS_2_INT );
    }

    // Erase object instance configuration for SRAM Click.
    if( SPI_MASTER_ERROR == spi_master_close( &sram_click )) {
        log_printf( &logger, "SPI master close failed.\n" );
        return RET_FAIL;
    }

    return RET_SUCCESS;
}

int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Initialize necessary SPI protocol properties.
    if( RET_FAIL == application_init()) {
        log_printf( &logger, "Application init failed.\n" );
        return RET_FAIL;
    }

    // Write, read, then subsequently display results on test pin.
    if( RET_FAIL == application_task()) {
        log_printf( &logger, "Application task failed.\n" );
        return RET_FAIL;
    }

    log_printf( &logger, "End of SPI demo.\n" );

    return RET_SUCCESS;
}

// ----------------------------------------------------------------------- END
