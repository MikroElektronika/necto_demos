/**
 * @file main.c
 * @brief Main function for demo_lvgl_widgets LVGL Designer Application.
 */

#include "lv_demo_widgets.h"

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
    // Initialize driver
    board_init();
    // Initialize all available screens
    init_screens();
    // Show main screen. If you want any other screen to show call its show function
    show_main_screen();
    // Call task handler once to draw the demo image
    lv_task_handler();
}

int main(void)
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    // Call all initialization methods
    application_init();

    // 3 second delay to show image
    Delay_ms(3000);

    // Initialize demo screen and widgets
    lv_demo_widgets();

#if PD_SETUP
    // Fetch all widget screen coordinates
    coordinates[0] = lv_obj_get_y(chart1_cont);
    coordinates[1] = lv_obj_get_y(chart2_cont);
    coordinates[2] = lv_obj_get_y(lv_obj_get_parent(meter1)) + METER1_OFFSET;
    coordinates[3] = lv_obj_get_y(lv_obj_get_parent(meter2)) + METER2_OFFSET;
    coordinates[4] = lv_obj_get_y(lv_obj_get_parent(meter3)) + METER3_OFFSET;
#endif

    /////////////////////////////LVGL specific timing routine (DO NOT DELETE)/////////////////////////
    while (1)
    {
#if PD_SETUP
        if (secondCount) 
        {
            // Every defined PD_SCROLL_DELAY_MS seconds, scroll automaticaly to next widget
            secondCount = false;
            if (widget_num > WIDGET_COUNT)
                // Reset counter if it comes to the end
                widget_num = 0;
            // Scroll screen to next widget
            lv_obj_scroll_to_y(t1, coordinates[widget_num++], LV_ANIM_OFF);
        }
#endif
        lv_task_handler();
        Delay_ms(1);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
}

static volatile uint32_t msCount = 0, secondsCount = 0;
INTERRUPT_ROUTINE
{
    msCount++;
#if PD_SETUP
    secondsCount++;
#endif

    if (5 == msCount) {
        msCount = 0;
        lv_tick_inc(5);
        process_tp();
    }

#if PD_SETUP
    if (PD_SCROLL_DELAY_MS == secondsCount) {
        secondsCount = 0;
        secondCount = true;
    }
#endif

    CLEAR_FLAG;
}
