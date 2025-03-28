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

// If hardware without defined LCD pins is used, make sure to define them.
#ifndef LCD_CS
#define LCD_CS HAL_PIN_NC
#endif
#ifndef LCD_RST
#define LCD_RST HAL_PIN_NC
#endif
#ifndef LCD_BPWM
#define LCD_BPWM HAL_PIN_NC
#endif
#ifndef LCD_D4
#define LCD_D4 HAL_PIN_NC
#endif
#ifndef LCD_D5
#define LCD_D5 HAL_PIN_NC
#endif
#ifndef LCD_D6
#define LCD_D6 HAL_PIN_NC
#endif
#ifndef LCD_D7
#define LCD_D7 HAL_PIN_NC
#endif

// ----------------------------------------------------------------- VARIABLES
static lcd_config_t lcd_cfg; // LCD config structure.
static lcd_handle_t lcd;     // LCD handle.

static char i;
// ----------------------------------------- PRIIVATE FUNCTIONS - DECLARATIONS

/**
 * @brief Writes out a set of characters.
 * @details Tests @ref lcd_write API.
 * @return Nothing.
 */
static void lcd_demo_hello_world();

/**
 * @brief Clears LCD, then writes text.
 * @details Tests @ref lcd_clear and @ref lcd_write_text API.
 * @return Nothing.
 */
static void lcd_demo_write_first_line();

/**
 * @brief Sets row 2 as active and writes out text.
 * @details Tests @ref lcd_set_row and @ref lcd_write with variables as parameters.
 * @return Nothing.
 */
static void lcd_demo_write_second_line();

/**
 * @brief Shifts data to the left.
 * @details Tests @ref lcd_shift_left API.
 * @return Nothing.
 */
static void lcd_demo_shift_left();

/**
 * @brief Shifts data to the right.
 * @details Tests @ref lcd_shift_right API.
 * @return Nothing.
 */
static void lcd_demo_shift_right();

/**
 * @brief Turns LCD ON and OFF.
 * @details Tests @ref lcd_turn_on and @ref lcd_turn_off API.
 * @return Nothing.
 */
static void lcd_demo_on_off();

/**
 * @brief Turns cursor ON and OFF.
 * @details Tests @ref lcd_cursor_on and @ref lcd_cursor_off API.
 * @return Nothing.
 */
static void lcd_demo_cursor_on_off();

/**
 * @brief Shifts cursor to the left, moves it home and then to the right.
 * @details Tests @ref lcd_cursor_move_left, @ref lcd_cursor_move_right and
 * @ref lcd_cursor_move_home API.
 * @return Nothing.
 */
static void lcd_demo_move_cursor();

/**
 * @brief Turns LCD backlight ON and OFF.
 * @details Tests @ref lcd_backlight_off, @ref lcd_backlight_on and API.
 * @return Nothing.
 */
static void lcd_demo_backlight_switch();

// ----------------------------------------------------------------- USER CODE

void application_init() {
    lcd_configure_default(&lcd_cfg);
    LCD_MAP_PINS(lcd_cfg);

    // Increase wait between data transmission to 65K NOPs.
    lcd_cfg.waitBetweenWrites = 0xFFFF;

    if (LCD_ERROR == lcd_configure(&lcd, &lcd_cfg))
        while(1);

    /**
     * @brief Map controller initialization sequence.
     */
    lcd_init(lcd, hd44780_lcd_init);
}

void application_task() {
    // Expected output is the following text in row 1:
    // HELLO WORLD!
    lcd_demo_hello_world();

    // Expected output is the following text in row 1:
    // LCD_demo
    lcd_demo_write_first_line();

    // Expected output is the following text in row 1:
    // LCD_demo
    // And following output in row 2:
    // mikroElektronika
    lcd_demo_write_second_line();

    // Expected output is text being shifted to the left.
    lcd_demo_shift_left();

    // Expected output is text being shifted to the right.
    lcd_demo_shift_right();

    // Expected output is text disappearing and reappearing a couple of times.
    lcd_demo_on_off();

    // Expected output is cursor disappearing and reappearing a couple of times
    // at the last position. Next to last character in row 2.
    lcd_demo_cursor_on_off();

    // Expected output is cursor moving to the left 3 times,
    // then moving to first position and finally moving
    // 3 times to the right.
    lcd_demo_move_cursor();

    // Expected output is the LCD blinking.
    lcd_demo_backlight_switch();

    // Signal END on LCD.
    lcd_clear(lcd);
    lcd_cursor_off(lcd);
    lcd_write_text(lcd, "DEMO COMPLETED!!", LCD_ROW_1);
}

int main(void) {
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    application_init();

    while(1) {
        application_task();
    }

    return 0;
}

// ----------------------------------------------------------------- PRIVATE FUNCTIONS

static void lcd_demo_hello_world(void) {
    lcd_write(lcd, 'H', LCD_SELECT_DATA);
    lcd_write(lcd, 'E', LCD_SELECT_DATA);
    lcd_write(lcd, 'L', LCD_SELECT_DATA);
    lcd_write(lcd, 'L', LCD_SELECT_DATA);
    lcd_write(lcd, 'O', LCD_SELECT_DATA);
    lcd_write(lcd, ' ', LCD_SELECT_DATA);
    lcd_write(lcd, 'W', LCD_SELECT_DATA);
    lcd_write(lcd, 'O', LCD_SELECT_DATA);
    lcd_write(lcd, 'R', LCD_SELECT_DATA);
    lcd_write(lcd, 'L', LCD_SELECT_DATA);
    lcd_write(lcd, 'D', LCD_SELECT_DATA);
    lcd_write(lcd, '!', LCD_SELECT_DATA);
}

static void lcd_demo_write_first_line(void) {
    Delay_ms(1000);
    lcd_clear(lcd);
    lcd_write_text(lcd, "LCD_demo", LCD_ROW_1);
    Delay_ms(1000);
}

static void lcd_demo_write_second_line(void) {
    char i,testArray[]={"mikroElektronika"};

    lcd_set_row(lcd, LCD_ROW_2);
    for(i=0;testArray[i]!=0;i++)
    {
        lcd_write(lcd, testArray[i], LCD_SELECT_DATA);
    }
}

static void lcd_demo_shift_left(void) {
    uint8_t i = 0;

    for(i=0; i<8; i++) {
        lcd_shift_left(lcd);
        Delay_ms(250);
    }
}

static void lcd_demo_shift_right(void) {
    uint8_t i = 0;

    for (i=0; i<8; i++) {
        lcd_shift_right(lcd);
        Delay_ms(250);
    }
}

static void lcd_demo_on_off(void) {
    lcd_turn_off(lcd);
    Delay_ms(500);
    lcd_turn_on(lcd);
    Delay_ms(500);
    lcd_turn_off(lcd);
    Delay_ms(500);
    lcd_turn_on(lcd);
    Delay_ms(500);
}

static void lcd_demo_cursor_on_off(void) {
    lcd_cursor_off(lcd);
    Delay_ms(1000);
    lcd_cursor_on(lcd);
    Delay_ms(1000);
    lcd_cursor_off(lcd);
    Delay_ms(1000);
    lcd_cursor_on(lcd);
    Delay_ms(1000);
}

static void lcd_demo_move_cursor(void) {
    lcd_cursor_move_left(lcd);
    Delay_ms(500);
    lcd_cursor_move_left(lcd);
    Delay_ms(500);
    lcd_cursor_move_left(lcd);
    Delay_ms(500);
    lcd_cursor_move_home(lcd);
    Delay_ms(500);
    lcd_cursor_move_right(lcd);
    Delay_ms(500);
    lcd_cursor_move_right(lcd);
    Delay_ms(500);
    lcd_cursor_move_right(lcd);
    Delay_ms(500);
}

static void lcd_demo_backlight_switch(void) {
    for(int8_t cnt = 0; cnt < 10; cnt++) {
        lcd_backlight_off(lcd);
        Delay_ms(200);
        lcd_backlight_on(lcd);
        Delay_ms(200);
    }
}

// ----------------------------------------------------------------------- END
