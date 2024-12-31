/**
 * @file main.c
 * @brief Main function for LVGL Keyboard Designer Application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#ifdef __GNUC__
#include "delays.h"
#endif

#include "display_lvgl.h"
#include "lv_port_indev.h"
#include "1ms_Timer.h"
#include "screens.h"

void board_init()
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    // Configure 1ms interrupt.
    timerInit();
}

void application_init()
{
    //Initialize driver
    board_init();
    //Initialize all available screens
    init_screens();
    //Show main screen. If you want any other screen to show call its show function
    show_main_screen();
}

extern uint8_t check_flag;

#if IS_PD_SETUP
// Macro that is used to define Enter Button position
#define ENTER_CHARACTER_BUTTON 22
// Macro that is used to define Space Bar Button position
#define SPACE_CHARACTER_BUTTON 37

// Function to simulate button clicks on the keyboard
void simulate_button_clicks(lv_obj_t *keyboard, const char *text) {
    // Map for lower cased keyboard
    char btn_map_lower[] = " qwertyuiop  asdfghjkl _-zxcvbnm.,:";
    // Map for upper cased keyboard
    char btn_map_upper[] = " QWERTYUIOP  ASDFGHJKL _-ZXCVBNM.,:";
    // Map for number keyboard
    char btn_map_number[] = "1234567890";
    // Map for special case keyboard
    char btn_map_special[] = "1234567890  +-/";
    // Flag for found character
    bool char_found;

    for (size_t i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        char_found = false;

        // Case if we need to press Space Bar
        if (c == ' ') {
            // Set Keyboard Button to be in pressed state
            lv_btnmatrix_set_btn_ctrl(keyboard, SPACE_CHARACTER_BUTTON, LV_BTNMATRIX_CTRL_CHECKED);
            // Pass the selected button value to the keyboard object
            lv_btnmatrix_set_selected_btn(keyboard, SPACE_CHARACTER_BUTTON);
            // Display the changes
            lv_task_handler();
            // Trigger the Keyboard Click event
            lv_event_send(keyboard, LV_EVENT_CLICKED, (void *)(uintptr_t)SPACE_CHARACTER_BUTTON);
            // Set Keyboard Button to be in released state
            lv_btnmatrix_clear_btn_ctrl(keyboard, SPACE_CHARACTER_BUTTON, LV_BTNMATRIX_CTRL_CHECKED);
            // Display the changes
            lv_task_handler();
            continue;
        }

        // Case if we need to press Enter Button
        if (c == '\n') {
            // Set Keyboard Button to be in pressed state
            lv_btnmatrix_set_btn_ctrl(keyboard, ENTER_CHARACTER_BUTTON, LV_BTNMATRIX_CTRL_CHECKED);
            // Pass the selected button value to the keyboard object
            lv_btnmatrix_set_selected_btn(keyboard, ENTER_CHARACTER_BUTTON);
            // Display the changes
            lv_task_handler();
            // Trigger the Keyboard Click event
            lv_event_send(keyboard, LV_EVENT_CLICKED, (void *)(uintptr_t)ENTER_CHARACTER_BUTTON);
            // Set Keyboard Button to be in released state
            lv_btnmatrix_clear_btn_ctrl(keyboard, ENTER_CHARACTER_BUTTON, LV_BTNMATRIX_CTRL_CHECKED);
            // Display the changes
            lv_task_handler();
            continue;
        }

        for (int j = 0; btn_map_lower[j] != '\0'; j++) {
            printf_me("%c   %c\n", btn_map_lower[j], c);
            if (btn_map_lower[j] == c) {
                // Set Keyboard Button to be in pressed state
                lv_btnmatrix_set_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                // Pass the selected button value to the keyboard object
                lv_btnmatrix_set_selected_btn(keyboard, j);
                // Display the changes
                lv_task_handler();
                // Trigger the Keyboard Click event
                lv_event_send(keyboard, LV_EVENT_CLICKED, (void *)(uintptr_t)j);
                // Set Keyboard Button to be in released state
                lv_btnmatrix_clear_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                // Display the changes
                lv_task_handler();
                char_found = true;
                break;
            }
        }

        // If in previous itterations char was not found, try different Keyboard Mode
        if (!char_found) {
            // Set the Keyboard to Upper Cased Text Mode
            lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_UPPER);
            // Display the changes
            lv_task_handler();
            for (int j = 0; btn_map_upper[j] != '\0'; j++) {
                if (btn_map_upper[j] == c) {
                    // Set Keyboard Button to be in pressed state
                    lv_btnmatrix_set_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                    // Pass the selected button value to the keyboard object
                    lv_btnmatrix_set_selected_btn(keyboard, j);
                    // Display the changes
                    lv_task_handler();
                    // Trigger the Keyboard Click event
                    lv_event_send(keyboard, LV_EVENT_CLICKED, (void *)(uintptr_t)j);
                    // Set Keyboard Button to be in released state
                    lv_btnmatrix_clear_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                    // Display the changes
                    lv_task_handler();
                    char_found = true;
                    break;
                }
            }
            // Return the Keyboard to Lower Cased Text Mode
            lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
            // Display the changes
            lv_task_handler();
        }

        // If in previous itterations char was not found, try different Keyboard Mode
        if (!char_found) {
            // Set the Keyboard to Special Cased Text Mode
            lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_SPECIAL);
            // Display the changes
            lv_task_handler();
            for (int j = 0; btn_map_special[j] != '\0'; j++) {
                if (btn_map_special[j] == c) {
                    // Set Keyboard Button to be in pressed state
                    lv_btnmatrix_set_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                    // Pass the selected button value to the keyboard object
                    lv_btnmatrix_set_selected_btn(keyboard, j);
                    // Display the changes
                    lv_task_handler();
                    // Trigger the Keyboard Click event
                    lv_event_send(keyboard, LV_EVENT_CLICKED, (void *)(uintptr_t)j);
                    // Set Keyboard Button to be in released state
                    lv_btnmatrix_clear_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                    // Display the changes
                    lv_task_handler();
                    char_found = true;
                    break;
                }
            }
            // Return the Keyboard to Lower Cased Text Mode
            lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
            // Display the changes
            lv_task_handler();
        }

        // If in previous itterations char was not found, try different Keyboard Mode
        if (!char_found) {
            // Set the Keyboard to Number Mode
            lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_NUMBER);
            // Display the changes
            lv_task_handler();
            for (int j = 0; btn_map_number[j] != '\0'; j++) {
                printf_me("%c   %c\n", btn_map_number[j], c);
                if (btn_map_number[j] == c) {
                    // Set Keyboard Button to be in pressed state
                    lv_btnmatrix_set_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                    // Pass the selected button value to the keyboard object
                    lv_btnmatrix_set_selected_btn(keyboard, j);
                    // Display the changes
                    lv_task_handler();
                    // Trigger the Keyboard Click event
                    lv_event_send(keyboard, LV_EVENT_CLICKED, (void *)(uintptr_t)j);
                    // Set Keyboard Button to be in released state
                    lv_btnmatrix_clear_btn_ctrl(keyboard, j, LV_BTNMATRIX_CTRL_CHECKED);
                    // Display the changes
                    lv_task_handler();
                    char_found = true;
                    break;
                }
            }
            // Return the Keyboard to Lower Cased Text Mode
            lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
            // Display the changes
            lv_task_handler();
        }
    }
}
#endif

int main(void)
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

#if IS_PD_SETUP
    char line_1[] = "You are looking at MIKROE LVGL Keyboard Demo.\n\nIf you want to find more Demos, visit:\n";
    char github[] = "https://github.com/MikroElektronika/necto_demos";
    char line_2[] = "\n\nThis demo will restart in 2 seconds...";
#else
    char restart[] = "Restarting Demo...";
#endif

    //Call all initialization methods
    application_init();

    lv_obj_set_style_bg_color(lvgl_second_screen_ui.keyboard_0, lv_color_make(0, 0, 0), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(lvgl_second_screen_ui.keyboard_0, lv_color_make(0, 0, 0), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_textarea_set_cursor_pos(lvgl_second_screen_ui.textarea_0, LV_TEXTAREA_CURSOR_LAST);
    lv_obj_add_state(lvgl_second_screen_ui.textarea_0, LV_STATE_FOCUSED);
    lv_event_send(lvgl_second_screen_ui.textarea_0, LV_EVENT_FOCUSED, NULL);
    lv_textarea_set_text(lvgl_second_screen_ui.textarea_0, "");

#if IS_PD_SETUP
    lv_textarea_set_text(lvgl_second_screen_ui.textarea_0, "");
    lv_task_handler();
    Delay_ms(3000);
    show_second_screen();
    lv_task_handler();
#endif

    /////////////////////////////LVGL specific timing routine (DO NOT DELETE)/////////////////////////
    while (1)
    {
#if !IS_PD_SETUP
        if (check_flag) {
            for (uint8_t counter = 0; counter < sizeof(restart); counter++) {
                lv_textarea_add_char(lvgl_second_screen_ui.textarea_0, restart[counter]);
                lv_task_handler();
                Delay_ms(20);
            }
            Delay_ms(2000);
            show_main_screen();
            lv_textarea_set_text(lvgl_second_screen_ui.textarea_0, "");
            check_flag = 0;
        }
#else
        simulate_button_clicks(lvgl_second_screen_ui.keyboard_0, line_1);
        simulate_button_clicks(lvgl_second_screen_ui.keyboard_0, github);
        simulate_button_clicks(lvgl_second_screen_ui.keyboard_0, line_2);
        Delay_ms(2000);
        lv_textarea_set_text(lvgl_second_screen_ui.textarea_0, "");
#endif
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
}

static volatile uint32_t msCount = 0;
INTERRUPT_ROUTINE
{
    msCount++;

    if (5 == msCount) {
        msCount = 0;
        lv_tick_inc(5);
        process_tp();
    }
    CLEAR_FLAG;
}
