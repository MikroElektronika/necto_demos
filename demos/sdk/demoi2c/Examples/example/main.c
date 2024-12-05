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
 *   I2C demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating I2C functionality using mikroSDK 2.0.
 *   Make sure to place EEPROM click in mikroBUS1 and TESTER click in mikroBUS2.
 *   Pin MIKROBUS_2_INT is set to high if transfer was successful. Written and read data
 *   is also printed out on the Standard output (determined by the chosen setup).
 *   Use a logic analyzer for more detailed insight into the trasfer details.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5398/i2c-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "MikroSDK.Driver.I2C.Master"
#include "MikroSDK.Driver.GPIO.In"
#include "MikroSDK.Driver.GPIO.Out"
#include "MikroSDK.Board"
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
#ifndef MIKROBUS_1_SCL
#define MIKROBUS_1_SCL NC
#endif
// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_1_SDA
#define MIKROBUS_1_SDA NC
#endif
// If hardware without mikroBUS socket is used, make sure to define adequate pin.
#ifndef MIKROBUS_2_INT
#define MIKROBUS_2_INT NC
#endif

// I2C pins
#define TEST_PIN_I2C_SCL MIKROBUS_1_SCL
#define TEST_PIN_I2C_SDA MIKROBUS_1_SDA

// Number of bytes which shall be sent/read.
// Take into consideration that the max size is 1 byte( 255 )
#define ARRAY_LENGTH 20

// Slave address for EEPROM click.
#define I2C_MASTER_SLAVE_ADDRESS 0x50

#define signal_success(port,pin) digital_out_init( &port, pin ); \
                                 digital_out_write( &port, 1 )

// ----------------------------------------------------------------- VARIABLES
static digital_out_t scl_pin;
static digital_in_t sda_pin;

static i2c_master_t i2c_master;
static i2c_master_config_t i2c_master_cfg;

static digital_out_t output_pin;

static uint8_t write_buffer[ARRAY_LENGTH];
static uint8_t read_buffer[ARRAY_LENGTH];

static log_t logger;
static log_cfg_t logger_cfg;

static uint8_t i;
static int8_t read_data;
// ----------------------------------------------------------------- USER CODE
err_t i2c_demo_write( uint8_t wAddr, uint8_t wData ) {
    uint8_t wr_dat[2];

    wr_dat[0] = wAddr;
    wr_dat[1] = wData;

    if( I2C_MASTER_ERROR == i2c_master_write( &i2c_master, wr_dat, 2 )) {
        log_printf( &logger, "I2C master write failed.\n" );
        return RET_FAIL;
    }

    return 0;
}

err_t i2c_demo_read( uint8_t rAddr ) {
    uint8_t rd_dat;

    if( I2C_MASTER_ERROR == i2c_master_write( &i2c_master, &rAddr, 1 )) {
        log_printf( &logger, "I2C master write failed.\n" );
        return RET_FAIL;
    }

    if( I2C_MASTER_ERROR == i2c_master_read( &i2c_master, &rd_dat, 1 )) {
        log_printf( &logger, "I2C master read failed.\n" );
        return RET_FAIL;
    }

    return rd_dat;
}

int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Initialize logger.
    LOG_MAP_USB_UART( logger_cfg );
    log_init( &logger, &logger_cfg );

    // Initializes I2C master configuration structure to default values.
    i2c_master_configure_default( &i2c_master_cfg );

    i2c_master_cfg.scl = TEST_PIN_I2C_SCL;
    i2c_master_cfg.sda = TEST_PIN_I2C_SDA;

    /*
     * The following piece of code is waiting for the slave device
     * to release the clock line (SCL) and acknowledge the I2C bus
     * is ready for communication. This is crucial because the master
     * must wait for the acknowledgment before starting any
     * communication on the I2C bus.
     */
    digital_out_init( &scl_pin, TEST_PIN_I2C_SCL );
    digital_in_init( &sda_pin, TEST_PIN_I2C_SDA );
    digital_out_high( &scl_pin );
    while ( digital_in_read( &sda_pin ) == 0 )
    {
        digital_out_low( &scl_pin );
        Delay_1ms();
        digital_out_high( &scl_pin );
        Delay_1ms();
    }

    // I2C master open.
    if( ACQUIRE_FAIL == i2c_master_open( &i2c_master, &i2c_master_cfg )) {
        log_printf( &logger, "I2C master open failed.\n" );
        return RET_FAIL;
    }

    // Set i2c timeout.
    if( I2C_MASTER_ERROR == i2c_master_set_timeout( &i2c_master, 0 )) {
        log_printf( &logger, "I2C master set timeout failed.\n" );
        return RET_FAIL;
    }

    // I2C master set speed.
    if( I2C_MASTER_ERROR == i2c_master_set_speed( &i2c_master, I2C_MASTER_SPEED_STANDARD )) {
        log_printf( &logger, "I2C master set speed failed.\n" );
        return RET_FAIL;
    }

    // I2C master set slave address.
    if( I2C_MASTER_ERROR == i2c_master_set_slave_address( &i2c_master, I2C_MASTER_SLAVE_ADDRESS )) {
        log_printf( &logger, "I2C master set slave address failed.\n" );
        return RET_FAIL;
    }

    log_printf( &logger, "Written data:\n" );
    for( i = 0; i < ARRAY_LENGTH; i++ ) {
        if( I2C_MASTER_ERROR == i2c_demo_write( i, i )) {
            return RET_FAIL;
        }
        write_buffer[i] = i;
        if( ARRAY_LENGTH - 1 != i ) {
            log_printf( &logger, "%d, ", (uint16_t)write_buffer[i] );
        } else {
            log_printf( &logger, "%d\n", (uint16_t)write_buffer[i] );
        }
        Delay_1ms();
        if( RET_FAIL != (read_data = i2c_demo_read(i)) ) {
            read_buffer[i] = read_data;
        } else return RET_FAIL;
    }

    log_printf( &logger, "Read data:\n" );
    for( i = 0; i < ARRAY_LENGTH; i++ ) {
        if( ARRAY_LENGTH - 1 != i ) {
            log_printf( &logger, "%d, ", (uint16_t)read_buffer[i] );
        } else {
            log_printf( &logger, "%d\n", (uint16_t)read_buffer[i] );
        }
    }

    /*
     *  memcmp compares buffers and returns 0 if they are the same.
     *  So, here, if it does return 0, it means that the transfer
     *  was successful and signal_success should set output_pin to high.
     */
    if ( !memcmp( write_buffer, read_buffer, sizeof( write_buffer ))){
        signal_success( output_pin, MIKROBUS_2_INT );
    }

    // Close I2C module.
    if( I2C_MASTER_ERROR == i2c_master_close( &i2c_master )) {
        log_printf( &logger, "I2C master close failed.\n" );
        return RET_FAIL;
    }

    log_printf( &logger, "End of I2C demo.\n" );

    return 0;
}

// ----------------------------------------------------------------------- END
