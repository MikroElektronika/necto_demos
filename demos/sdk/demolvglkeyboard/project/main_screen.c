#include "main_screen.h"

lvgl_main_screen_ui_t lvgl_main_screen_ui;

void init_main_screen()
{
  init_main_screen_ui(&lvgl_main_screen_ui);  
}

void show_main_screen()
{
    lv_scr_load(lvgl_main_screen_ui.main_screen);
}

void button_clicked(lv_event_t * event)
{
	//Place your event code here
    show_second_screen();
}
