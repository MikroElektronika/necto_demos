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
 *   Designer Tool Demo
 * Copyright:
 *   (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *   Example is meant for demonstrating Designer Tool functionality in
 *   NECTOStusio IDE.
 *   It shows the basic use cases for:
 *    + Buttons, Progress Bar, Labels, Images, Screens
 *    + Button Press Events
 *    + Text and Color changes for screens and elements
 *   Example changes colors of the screen and progress bar value by clicking
 *   corresponding buttons. When Progress Bar reaches 100%, demo "restarts" -
 *   it shows the main screen and then application screen again.
 *   This Demo was tested on TFT 4 Capacitive Display. For other displays
 *   components placement should be adjusted for better view.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * TODO
 */

// ------------------------------------------------------------------ INCLUDES

#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include "gl.h"
#include "vtft.h"
#include "main_screen.h"
#include "second_screen.h"
#include "scr_second_screen.h"
#include "touch_controller.h"

// ----------------------------------------------------------------- DEFINES

#define PD_SETUP IS_PD_SETUP

// ----------------------------------------------------------------- VARIABLES

static gl_driver_t display_driver;
static tp_t tp;
static vtft_t vtft;
static tp_drv_t tp_interface;

// ----------------------------------------------------------------- USER CODE

extern void _tp_event_handler( tp_event_t event, uint16_t x, uint16_t y, tp_touch_id_t i);

void board_init()
{
    touch_controller_init(&display_driver, TFT_MAX_BACKLIGHT);
    // Initialize Graphic library.
    gl_set_driver(&display_driver);

    touch_controller_tp_init(&tp, &tp_interface);
}

void immitate_touches()
{
    // Immitate the touch of RED! button.
    vtft_process(&vtft);
    _tp_event_handler(TP_EVENT_PRESS_DOWN, 16, 18, 0);
    Delay_ms(500);
    _tp_event_handler(TP_EVENT_PRESS_UP, 16, 18, 0);
    Delay_ms(1000);

    // Immitate the touch of BLUE! button.
    _tp_event_handler(TP_EVENT_PRESS_DOWN, 113, 18, 0);
    Delay_ms(500);
    _tp_event_handler(TP_EVENT_PRESS_UP, 113, 18, 0);
    Delay_ms(1000);

    // Immitate the touch of GREEN! button.
    _tp_event_handler(TP_EVENT_PRESS_DOWN, 207, 18, 0);
    Delay_ms(500);
    _tp_event_handler(TP_EVENT_PRESS_UP, 207, 18, 0);
    Delay_ms(1000);

    // Immitate the touch of BLACK! button.
    _tp_event_handler(TP_EVENT_PRESS_DOWN, 302, 18, 0);
    Delay_ms(500);
    _tp_event_handler(TP_EVENT_PRESS_UP, 302, 18, 0);
    Delay_ms(1000);

    // Immitate the touch of WHITE! button.
    _tp_event_handler(TP_EVENT_PRESS_DOWN, 399, 18, 0);
    Delay_ms(500);
    _tp_event_handler(TP_EVENT_PRESS_UP, 399, 18, 0);
    Delay_ms(1000);

    // Immitate the touch of INCREASE! button 10 times.
    for (uint8_t counter = 0; counter < 10; counter++) {
        _tp_event_handler(TP_EVENT_PRESS_DOWN, 369, 114, 0);
        Delay_ms(100);
        _tp_event_handler(TP_EVENT_PRESS_UP, 369, 114, 0);
        Delay_ms(200);
    }

    // Immitate the touch of DECREASE! button 2 times.
    for (uint8_t counter = 0; counter < 2; counter++) {
        _tp_event_handler(TP_EVENT_PRESS_DOWN, 15, 114, 0);
        Delay_ms(100);
        _tp_event_handler(TP_EVENT_PRESS_UP, 15, 114, 0);
        Delay_ms(200);
    }
}

/**
 * @brief Application initialization function.
 */
void application_init()
{
    board_init();

    // Initialize VTFT library.
    vtft_init(&vtft, &tp);
    main_screen_show(&vtft);

    // Wait 3 seconds and show the second screen.
    Delay_ms(3000);
    second_screen_show(&vtft);
    vtft_set_progress_bar_position(&vtft, second_screen.ProgressBar_0, 0);
}

/**
 * @brief Application task function.
 */
void application_task()
{
    vtft_process(&vtft);

    // If project is running on PD - immitate the button pushes.
    #if PD_SETUP
    immitate_touches();
    #endif
}

/**
 * @brief Application main function.
 */
void main()
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    application_init();

    while (1)
    {
        application_task();
    }
}
