/**
 * @file main.c
 * @brief Main function for Untitled6 LVGL Designer Application.
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

int main(void)
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

#if IS_PD_SETUP
    char restart[] = "MIKROE LVGL Keyboard demo...";
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

    /////////////////////////////LVGL specific timing routine (DO NOT DELETE)/////////////////////////
    while (1)
    {
#if !IS_PD_SETUP
        if (check_flag) {
#endif
            for (uint8_t counter = 0; counter < sizeof(restart); counter++) {
                lv_textarea_add_char(lvgl_second_screen_ui.textarea_0, restart[counter]);
                lv_task_handler();
                Delay_ms(20);
            }
            Delay_ms(2000);
            show_main_screen();
            lv_textarea_set_text(lvgl_second_screen_ui.textarea_0, "");
            check_flag = 0;
#if !IS_PD_SETUP
        }
#endif
        lv_task_handler();
        Delay_ms(1);

#if IS_PD_SETUP
        Delay_ms(3000);
        show_second_screen();
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
