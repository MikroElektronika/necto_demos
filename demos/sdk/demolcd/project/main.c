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
 *  LCD demo
 * Copyright:
 *  (c) Mikroelektronika, ${COPYRIGHT_YEAR}.
 * Description:
 *  Example is meant for demonstrating LCD functionality using mikroSDK 2.0.
 *  If you're using one of the MIKROE v8 boards, Put LCD display into LCD socket.
 */

/**
 * @brief For a detailed explanation of this demo, please visit:
 * <https://github.com/MikroElektronika/necto_demos/blob/main/demos/sdk/demolcd/project/README.md>
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

#include "board.h"
#include "lcd.h"
#include "drv_digital_out.h"
#include "mikrosdk_version.h"

// ----------------------------------------------------------------- MACROS
#if mikroSDK_GET_VERSION < 21000ul
    #error "SDK version must be 2.10.0 or higher."
#endif

#define DELAY_LCD 300

// ----------------------------------------------------------------- VARIABLES
static lcd_config_t lcd_cfg; // LCD config structure.
static lcd_handle_t lcd;     // LCD handle.

static char txt1[] = "LCD_demo";
static char txt2[] = "mikroElektronika";

static char i;

// ----------------------------------------------------------------- USER CODE
void lcd_demo_init(void) {
    // Default configuration.
    lcd_configure_default(&lcd_cfg);

    // Mapping LCD pins.
    LCD_MAP_PINS(lcd_cfg);

    // Initialize handle and pins.
    lcd_configure(&lcd, &lcd_cfg);

    // Initialize LCD.
    lcd_init(lcd, hd44780_lcd_init);

    // LCD turning on.
    lcd_turn_on(lcd);
    Delay_ms(DELAY_LCD);

    // Backlight turning on.
    lcd_backlight_on(lcd);
    Delay_ms(DELAY_LCD);

    // Clearing LCD.
    lcd_clear(lcd);
    Delay_ms(DELAY_LCD);

    // LCD cursor off.
    lcd_cursor_off(lcd);
    Delay_ms(DELAY_LCD);
}

int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    lcd_demo_init();

    // Clearing LCD.
    lcd_clear(lcd);
    Delay_ms(DELAY_LCD);

    // Setting first row.
    lcd_set_row(lcd, LCD_ROW_1);
    Delay_ms(DELAY_LCD);

    // Writing text in first row.
    lcd_write_text(lcd, txt1, LCD_ROW_1);
    Delay_ms(DELAY_LCD);

    // Setting second row.
    lcd_set_row(lcd, LCD_ROW_2);
    Delay_ms(DELAY_LCD);

    // Writing text in second row.
    lcd_write_text(lcd, txt2, LCD_ROW_2);
    Delay_ms(DELAY_LCD);

    while(1) {
        // Shifting characters left 5 times.
        for(i=0; i<5; i++) {
            lcd_shift_left(lcd);
            Delay_ms(DELAY_LCD);
        }

        // Shifting characters right 5 times.
        for(i=0; i<5; i++) {
            lcd_shift_right(lcd);
            Delay_ms(DELAY_LCD);
        }
    }

  return 0;
}
// ----------------------------------------------------------------------- END
