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
 *   CycloneTCP HTTP Client Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating HTTP client functionality using
 *   CycloneTCP TCP/IP stack and mikroSDK 2.0. Make sure to connect Ethernet
 *   cable to the ETH connector and USB-C cable to the USB-UART connector on
 *   your board. Configure the UART terminal and connect to it. When prompted,
 *   press the apropriate button and observe the process of connecting to
 *   the HTTP server, then by using POST method, request to accept and store
 *   the "Hello world!" plain text message. Response from the HTTP server
 *   is then read and its status, type and contents(Hello World!) are written
 *   over UART/Application output.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://libstock.mikroe.com/projects/view/5491/cyclonetcp-http-client-demo>
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
#include "http/http_client.h"

// Include currently active driver header.
#include "eth_driver.h"

// User defined PHY driver.
#include "drivers/phy/lan8720_driver.h"

// -------------------------------------------------------------------- MACROS

// Return value in case of error.
#define RET_FAIL (-1)

// Defined test name.
#define TEST_NAME "MikroE CycloneTCP HTTP Client Demo"

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
#define APP_HOST_NAME "http-client-demo"
#define APP_MAC_ADDR "00-AB-CD-EF-07-69"

// Application configuration.
#define APP_HTTP_SERVER_NAME "www.httpbin.org"
#define APP_HTTP_SERVER_PORT 80
#define APP_HTTP_URI "/anything"

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
static HttpClientContext httpClientContext;

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

static error_t httpClientTest(void) {
    error_t error;
    size_t length;
    uint_t status;
    const char_t *value;
    IpAddr ipAddr;
    char_t buffer[128];
    Ipv4Addr ipv4Addr;

    // Initialize HTTP client context.
    httpClientInit(&httpClientContext);

    // Start of exception handling block.
    do {
        // Debug message
        TRACE_INFO("\r\n\r\nResolving server name...\r\n");

        // Resolve HTTP server name
        do {
            if (TIMEOUT_VALUE_MS == timeout) {
                timeout = 0;
                break;
            }
            error = getHostByName(NULL, APP_HTTP_SERVER_NAME, &ipAddr, 0);
        } while (error);

        // Select HTTP protocol version
        error = httpClientSetVersion(&httpClientContext, HTTP_VERSION_1_1);
        // Any error to report?
        if (error)
            break;

        // Set timeout value for blocking operations
        error = httpClientSetTimeout(&httpClientContext, 20000);
        // Any error to report?
        if (error)
            break;

        // Debug message.
        TRACE_INFO("Connecting to HTTP server %s...\r\n", ipAddrToString(&ipAddr, NULL));

        // Connect to the HTTP server.
        timeout = 0;
        do {
            if (TIMEOUT_VALUE_MS == timeout) {
                timeout = 0;
                break;
            }
            error = httpClientConnect(&httpClientContext, &ipAddr,
                                      APP_HTTP_SERVER_PORT);
        } while (error);

        // Any error to report?
        if (error) {
            // Debug message.
            TRACE_INFO("Failed to connect to HTTP server!\r\n");
            break;
        }

        // Display IPv4 host address.
        ipv4GetHostAddr(interface, &ipv4Addr);
        TRACE_INFO("Assigned IP address: %s\r\n", ipv4AddrToString(ipv4Addr, buffer));

        // Create an HTTP request.
        httpClientCreateRequest(&httpClientContext);
        httpClientSetMethod(&httpClientContext, "POST");
        httpClientSetUri(&httpClientContext, APP_HTTP_URI);

        // Set query string.
        httpClientAddQueryParam(&httpClientContext, "param1", "value1");
        httpClientAddQueryParam(&httpClientContext, "param2", "value2");

        // Add HTTP header fields.
        httpClientAddHeaderField(&httpClientContext, "Host", APP_HTTP_SERVER_NAME);
        httpClientAddHeaderField(&httpClientContext, "User-Agent", "Mozilla/5.0");
        httpClientAddHeaderField(&httpClientContext, "Content-Type", "text/plain");
        httpClientAddHeaderField(&httpClientContext, "Transfer-Encoding", "chunked");

        // Send HTTP request header.
        error = httpClientWriteHeader(&httpClientContext);

        // Any error to report?
        if (error) {
            // Debug message
            TRACE_INFO("Failed to write HTTP request header!\r\n");
            break;
        }

        // Send HTTP request body.
        error = httpClientWriteBody(&httpClientContext, "Hello World!", 12,
                                    NULL, 0);
        // Any error to report?
        if (error) {
            // Debug message.
            TRACE_INFO("Failed to write HTTP request body!\r\n");
            break;
        }

        // Receive HTTP response header.
        timeout = 0;
        do {
            if (TIMEOUT_VALUE_MS <= timeout) {
                timeout = 0;
                break;
            }
            error = httpClientReadHeader(&httpClientContext);
        } while (error);

        // Any error to report?
        if (error) {
            // Debug message.
            TRACE_INFO("Failed to read HTTP response header!\r\n");
            break;
        }

        // Retrieve HTTP status code.
        status = httpClientGetStatus(&httpClientContext);

        // Debug message.
        TRACE_INFO("HTTP status code: %u\r\n", status);

        // Retrieve the value of the Content-Type header field.
        value = httpClientGetHeaderField(&httpClientContext, "Content-Type");

        // Header field found?
        if (value != NULL) {
            // Debug message.
            TRACE_INFO("Content-Type header field value: %s\r\n", value);
        } else {
            // Debug message.
            TRACE_INFO("Content-Type header field not found!\r\n");
        }

        // Receive HTTP response body.
        while (!error) {
            // Read data.
            error = httpClientReadBody(&httpClientContext, buffer,
                                       sizeof(buffer) - 1, &length, 0);

            // Check status code.
            if (!error) {
                // Properly terminate the string with a NULL character
                buffer[length] = '\0';
                // Dump HTTP response body
                TRACE_INFO("%s", buffer);
            }
        }

        // Terminate the HTTP response body with a CRLF.
        TRACE_INFO("\r\n");

        // Any error to report?
        if (error != ERROR_END_OF_STREAM)
            break;

        // Close HTTP response body.
        error = httpClientCloseBody(&httpClientContext);

        // Any error to report?
        if (error) {
            // Debug message
            TRACE_INFO("Failed to read HTTP response trailer!\r\n");
            break;
        }

        // Gracefully disconnect from the HTTP server.
        httpClientDisconnect(&httpClientContext);

        // Debug message.
        TRACE_INFO("Connection closed\r\n");

        // End of exception handling block.
    } while (0);

    // Release HTTP client context.
    httpClientDeinit(&httpClientContext);

    // Return status code.
    return error;
}

static int testRun(void) {
    // Error code.
    error_t error;

    // Run HTTP Client test.
    while (!digital_in_read(&button));
    error = httpClientTest();
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
        TRACE_INFO( "Application init failed.\n" );
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
