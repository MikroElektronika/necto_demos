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
 *   CycloneTCP FTP Client Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating FTP client functionality using
 *   CycloneTCP TCP/IP stack and mikroSDK 2.0. Make sure to connect Ethernet
 *   cable to the ETH connector and USB-C cable to the USB-UART connector on
 *   your board. Configure the UART terminal and connect to it. When prompted
 *   press the apropriate button and observe the process of connecting to
 *   the FTP server and reading a file from it on UART/Application output.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5492/cyclonetcp-ftp-client-demo>
 */

// ------------------------------------------------------------------ INCLUDES

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

// BEGIN HW initialization.
#include "hw_eth.h"
// END HW initialization.

#ifdef SYSTICK_PRESENT
// Include systick for ARM and RISC-V MCUs.
#include "systick.h"
#else
// Include timer for PIC32 MCUs.
#include "timer.h"
#endif

#include "log.h"
#include "board.h"
#include "debug.h"
#include "drv_digital_in.h"
#include "drv_digital_out.h"
#include "ftp/ftp_client.h"

// Include currently active driver header.
#include "eth_driver.h"

// User defined PHY driver.
#include "drivers/phy/lan8720_driver.h"

// -------------------------------------------------------------------- MACROS

// Return value in case of error.
#define RET_FAIL (-1)

// Defined test name.
#define TEST_NAME "MikroE CycloneTCP FTP Client Demo"

// Current MCU passed as PLD via CMAKE.
#define MCU_NAME MCU_NAME_CMAKE

// Define debug console settings.
#define LED_SYSCTICK_CHECK PD2
#define LED_SANITY_CHECK PD3
#define LED_DEBUG_CHECK PD4
#define LED_SYSCTICK_CHECK_POS 0
#define LED_SANITY_POS 1
#define LED_DEBUG_POS 2
#define CONSOLE_BAUD 115200

// Timeout value in ms.
// Used to exit an infinite loop if there is any net error.
#define TIMEOUT_VALUE_MS 10000

// Ethernet interface configuration.
#define APP_IF_NAME "eth0"
#define APP_HOST_NAME "ftp-client-demo"
#define APP_MAC_ADDR "00-AB-CD-EF-55-00"

// Application configuration.
#define APP_FTP_SERVER_NAME "test.rebex.net"
#define APP_FTP_SERVER_PORT 21
#define APP_FTP_LOGIN "demo"
#define APP_FTP_PASSWORD "password"
#define APP_FTP_FILENAME "readme.txt"

// ----------------------------------------------------------------- VARIABLES

// Pointer function typedef.
typedef int (*fun_ptr)(void);
fun_ptr fun_ptr_client_init;
fun_ptr fun_ptr_slac_configure;

// Global variables.
static NetInterface *interface;
static DhcpClientSettings dhcpClientSettings;
static DhcpClientContext dhcpClientContext;
static SlaacSettings slaacSettings;
static SlaacContext slaacContext;
static FtpClientContext ftpClientContext;

// ETH state signal.
static bool ethInitialized = false;
static uint32_t msCount = 0;
uint32_t timeout = 0;

// Debug array with pins.
static digital_out_t debugErrArray[3];
static pin_name_t debugPins[3] = {LED_SYSCTICK_CHECK, LED_SANITY_CHECK, LED_DEBUG_CHECK};

// Test runner button.
static digital_in_t button;

// Debug handle.
log_t console;
static log_cfg_t console_cfg;

// ----------------------------------------------------------------- USER CODE

static void testInitDebug(void) {
    for (int i = 0; i < (sizeof(debugErrArray) / sizeof(digital_out_t)); i++) {
        digital_out_init(&debugErrArray[i], debugPins[i]);
        digital_out_low(&debugErrArray[i]);
    }

    LOG_MAP_USB_UART( console_cfg );
    log_init( &console, &console_cfg );

    digital_in_init(&button, TEST_BUTTON);

    // Start-up message
    TRACE_INFO("\r\n***************************************\r\n");
    TRACE_INFO("******* %s *******\r\n", TEST_NAME);
    TRACE_INFO("***************************************\r\n");
    TRACE_INFO("Copyright: 2023 MikroElektronika d.o.o.\r\n");
    TRACE_INFO("https://github.com/MikroElektronika/mikrosdk_v2\r\n");
    TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
    TRACE_INFO("Target: %s\r\n", MCU_NAME);
}

static int testInitClientDhcp(void) {
    // Error code.
    error_t error;

    // Get default settings.
    dhcpClientGetDefaultSettings(&dhcpClientSettings);
    // Set the network interface to be configured by DHCP.
    dhcpClientSettings.interface = interface;
    // Disable rapid commit option.
    dhcpClientSettings.rapidCommit = FALSE;

    // DHCP client initialization.
    error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
    // Failed to initialize DHCP client?
    if(error) {
        TRACE_INFO("Failed to initialize DHCP client!\r\n");
        return error;
    }

    // Start DHCP client.
    error = dhcpClientStart(&dhcpClientContext);
    // Failed to start DHCP client?
    if(error) {
        TRACE_INFO("Failed to start DHCP client!\r\n");
        return error;
    }

    return NO_ERROR;
}

static int testIpv6SlaacEnable(void) {
    // Error code.
    error_t error;

    // Get default settings.
    slaacGetDefaultSettings(&slaacSettings);
    // Set the network interface to be configured.
    slaacSettings.interface = interface;

    // SLAAC initialization.
    error = slaacInit(&slaacContext, &slaacSettings);
    // Failed to initialize SLAAC?
    if(error) {
        TRACE_INFO("Failed to initialize SLAAC!\r\n");
        return error;
    }

    // Start IPv6 address autoconfiguration process.
    error = slaacStart(&slaacContext);
    // Failed to start SLAAC process?
    if(error) {
        TRACE_INFO("Failed to start SLAAC!\r\n");
        return error;
    }

    return NO_ERROR;
}

static void testSetPointers(void) {
    fun_ptr_client_init = testInitClientDhcp;
    fun_ptr_slac_configure = testIpv6SlaacEnable;
}

static int testInitStack(void) {
    error_t error;
    MacAddr macAddr;

    // TCP/IP stack initialization.
    error = netInit();
    // Any error to report?
    if(error) {
        TRACE_INFO("Failed to initialize TCP/IP stack!\r\n");
        return error;
    }

    // Configure the first Ethernet interface.
    interface = &netInterface[0];

    // Set interface name.
    netSetInterfaceName(interface, APP_IF_NAME);
    // Set host name.
    netSetHostname(interface, APP_HOST_NAME);
    // Set host MAC address.
    macStringToAddr(APP_MAC_ADDR, &macAddr);
    netSetMacAddr(interface, &macAddr);

    /**
     * @brief Select the relevant network adapter.
     * @note Set automatically by mikroSDK based on
     * active setup.
     * @warning Selected setup MUST have ETHERNET module.
     */
    netSetDriver(interface, &ETHERNET_DRIVER_HANDLER);

    /**
     * @brief Map PHY chip.
     * @note Set automatically by mikroSDK based on
     * active setup.
     * @note Set to NULL if you wish to use
     * internal PHY.
     * @warning Selected setup MUST have ETHERNET module.
     */
    netSetPhyDriver(interface, ETHERNET_PHY_CHIP);

    // Initialize network interface.
    error = netConfigInterface(interface);
    // Any error to report?
    if(error) {
        TRACE_INFO("Failed to configure interface %s!\r\n", interface->name);
        return error;
    }

    error = netStartInterface(interface);
    // Any error to report?
    if(error) {
        TRACE_INFO("Failed to start interface %s!\r\n", interface->name);
        return error;
    }

    return NO_ERROR;
}

// Application initialization procedure.
int8_t application_init() {
    // Initialize debug console.
    testInitDebug();

    #ifndef SYSTICK_PRESENT
    // Configure 1ms interrupt.
    if (!init_1ms_timer()) {
        TRACE_INFO("No valid Timer1 configuration possible!\r\n");
        return RET_FAIL;
    }
    #else
    // Configure SYSTICK to 1ms interrupt.
    if (!sysTickConfig(GET_TICK_NUMBER_PER_CLOCK)) {
        sysTickInit(15); // Maximum priority - level 15.
    } else {
        return RET_FAIL;
    }
    #endif

    // Set appropriate function pointers.
    testSetPointers();

    // Initialize net stack.
    if (testInitStack()) {
        return RET_FAIL;
    }

    // Initialize DHCP client.
    if ((*fun_ptr_client_init)()) {
        return RET_FAIL;
    }

    // Initialize SLAAC.
    if ((*fun_ptr_slac_configure)()) {
        return RET_FAIL;
    }

    // If we get here, ETH module is considered initialized.
    ethInitialized = true;

    return 0;
}

static error_t ftpClientTest(void) {
    error_t error;
    size_t n;
    IpAddr ipAddr;
    char_t buffer[128];

    // Set initial time counter to 0.
    timeout = 0;

    // Initialize FTP client context.
    ftpClientInit(&ftpClientContext);

    // Start of exception handling block.
    do {
        // Debug message
        TRACE_INFO("\r\n\r\nResolving server name %s...\r\n", APP_FTP_SERVER_NAME);

        // Resolve FTP server name.
        do {
            if (TIMEOUT_VALUE_MS == timeout) {
                timeout = 0;
                break;
            }
            error = getHostByName(NULL, APP_FTP_SERVER_NAME, &ipAddr, 0);
        } while (error);

        // Any error to report?
        if (error) {
            TRACE_INFO("Failed to resolve FTP server name!\r\n");
            break;
        }

        // Set timeout value for blocking operations.
        error = ftpClientSetTimeout(&ftpClientContext, 20000);

        // Any error to report?
        if (error)
            break;

        // Debug message.
        TRACE_INFO("Connecting to FTP server %s...\r\n",
                   ipAddrToString(&ipAddr, NULL));

        timeout = 0;
        // Connect to the FTP server.
        do {
            if (TIMEOUT_VALUE_MS == timeout) {
                timeout = 0;
                break;
            }
            error = ftpClientConnect(&ftpClientContext, &ipAddr,
                                     APP_FTP_SERVER_PORT,
                                     FTP_MODE_PLAINTEXT | FTP_MODE_PASSIVE);
        } while (error);

        // Any error to report?
        if (error) {
            // Debug message.
            TRACE_INFO("Failed to connect to FTP server!\r\n");
            break;
        }

        // Debug message.
        TRACE_INFO("Logging in to FTP server %s...\r\nusername: %s\r\npassword: %s\r\n",
                   ipAddrToString(&ipAddr, NULL), APP_FTP_LOGIN, APP_FTP_PASSWORD);

        timeout = 0;
        // Login to the FTP server using the provided username and password.
        do {
            if (TIMEOUT_VALUE_MS == timeout) {
                timeout = 0;
                break;
            }
            error = ftpClientLogin(&ftpClientContext, APP_FTP_LOGIN, APP_FTP_PASSWORD);
        } while (error);

        // Any error to report?
        if(error) {
            TRACE_INFO("Wrong username or password!\r\n");
            break;
        }

        // Debug message.
        TRACE_INFO("Opening the file %s...\r\n\r\n", APP_FTP_FILENAME);

        timeout = 0;
        // Open the specified file for reading.
        do {
            if (TIMEOUT_VALUE_MS == timeout) {
                timeout = 0;
                break;
            }
            error = ftpClientOpenFile(&ftpClientContext,
                                    APP_FTP_FILENAME,
                                    FTP_FILE_MODE_READ | FTP_FILE_MODE_BINARY);
        } while (error);

        // Any error to report?
        if(error) {
            TRACE_INFO("Failed to open the file!\r\n");
            break;
        }

        // Read the contents of the file.
        while(!error)
        {
            timeout = 0;
            // Read data.
            do {
                if (TIMEOUT_VALUE_MS == timeout) {
                    timeout = 0;
                    break;
                }
                error = ftpClientReadFile(&ftpClientContext, buffer, sizeof(buffer) - 1, &n, 0);
            } while ((error != NO_ERROR) && (error != ERROR_END_OF_STREAM));

            // Check status code.
            if(NO_ERROR == error)
            {
                // Properly terminate the string with a NULL character.
                buffer[n] = '\0';
                // Dump the contents of the file.
                TRACE_INFO("%s", buffer);
            }
        }

        // Terminate the string with a line feed.
        TRACE_INFO("\r\n");

        // Any error to report?
        if(error != ERROR_END_OF_STREAM) {
            TRACE_INFO("Failed while reading the file!\r\n");
            break;
        }

        // Debug message.
        TRACE_INFO("Closing the file %s...\r\n", APP_FTP_FILENAME);

        timeout = 0;
        // Close file.
        do {
            if (TIMEOUT_VALUE_MS == timeout) {
                timeout = 0;
                break;
            }
            error = ftpClientCloseFile(&ftpClientContext);
        } while (error);

        // Any error to report?
        if(error) {
            TRACE_INFO("Failed to close the file!\r\n");
            break;
        }

        // Gracefully disconnect from the FTP server.
        ftpClientDisconnect(&ftpClientContext);

        // Debug message.
        TRACE_INFO("Connection closed\r\n");

        // End of exception handling block.
    } while (0);

    // Release FTP client context.
    ftpClientDeinit(&ftpClientContext);

    // Return status code.
    return error;
}

static int testRun(void) {
    // Error code.
    error_t error;

    // Run FTP Client test.
    while (!digital_in_read(&button));
    error = ftpClientTest();
    while (digital_in_read(&button));

    return (int)error;
}

// Application task procedure.
int8_t application_task() {
    TRACE_INFO("\r\nPress the button %s to continue\r\n", TEST_BUTTON_NAME);

    // Run the test once user clicks defined TEST_BUTTON.
    if (!testRun()) {
        TRACE_INFO("Demo completed.\r\n" );
    }

    return 0;
}

int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Initialize necessary SPI protocol properties.
    if( RET_FAIL == application_init()) {
        log_printf( &console, "Application init failed.\n" );
        return RET_FAIL;
    }

    for ( ; ; ) {
        application_task();
    }

    return 0;
}

// Sanity check - is the MCU still running? - Called from ISR.
static void ledTask(void) {
    // Check if interrupt is 1ms.
    digital_out_toggle(&debugErrArray[LED_SYSCTICK_CHECK_POS]);
    // 1000ms counted.
    if (msCount == 1000) {
        msCount = 0;
        digital_out_toggle(&debugErrArray[LED_SANITY_POS]);
    }
}

/**
 * @note Using the stack without an OS requires @ref systemTicks
 * to be incremented every 1ms.
 */
extern volatile systime_t systemTicks;
TIMER_SYSTICK_HANDLER() {
    #ifndef SYSTICK_PRESENT
    // Clear the timer interrupt.
    IFS0bits.T1IF = 0;
    #endif
    msCount++;
    timeout++;
    systemTicks++;

    ledTask();

    if (ethInitialized)
        netTask();
}


#ifdef PIC32MZ
void __attribute__((interrupt(IPL2AUTO), vector(_ETHERNET_VECTOR))) _ETHERNET_HANDLER(void) {
    digital_out_toggle(&debugErrArray[LED_DEBUG_POS]);
    pic32mzEthIrqHandler();
}
#endif

#ifdef PIC32MX
void __attribute__((interrupt(IPL2AUTO), vector(_ETH_VECTOR))) _ETH_HANDLER(void) {
    digital_out_toggle(&debugErrArray[LED_DEBUG_POS]);
    pic32mxEthIrqHandler();
}
#endif

// ----------------------------------------------------------------------- END
