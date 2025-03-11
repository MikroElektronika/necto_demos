#include "main_screen.h"

lvgl_main_screen_ui_t lvgl_main_screen_ui;

void init_main_screen()
{
    init_main_screen_ui(&lvgl_main_screen_ui);
    // Adjust the size of the elements for the 4 inch display
    #if (_TFT_WIDTH_ == 480)
    lv_obj_set_x(lvgl_main_screen_ui.img_0, 170);
    lv_obj_set_y(lvgl_main_screen_ui.img_0, 120);
    lv_img_set_zoom(lvgl_main_screen_ui.img_0, 0);
    lv_obj_set_x(lvgl_main_screen_ui.btn_0, 100);
    lv_obj_set_y(lvgl_main_screen_ui.btn_0, 200);
    lv_obj_set_style_text_font(lvgl_main_screen_ui.btn_0, &Ubuntu_Regular_s24_r32_127, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(lvgl_main_screen_ui.btn_0, 305);
    lv_obj_set_height(lvgl_main_screen_ui.btn_0, 50);
    lv_obj_set_x(lvgl_main_screen_ui.label_0, 15);
    lv_obj_set_y(lvgl_main_screen_ui.label_0, 15);
    lv_obj_set_width(lvgl_main_screen_ui.label_0, 450);
    lv_obj_set_height(lvgl_main_screen_ui.label_0, 100);
    lv_obj_set_style_text_align(lvgl_main_screen_ui.label_0, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lvgl_main_screen_ui.label_0, &Ubuntu_Regular_s24_r32_127, LV_PART_MAIN | LV_STATE_DEFAULT);
    #endif
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
