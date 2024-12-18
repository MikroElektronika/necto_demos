#include "second_screen.h"

lvgl_second_screen_ui_t lvgl_second_screen_ui;

void init_second_screen()
{
  init_second_screen_ui(&lvgl_second_screen_ui);
}

void show_second_screen()
{
    lv_scr_load(lvgl_second_screen_ui.second_screen);
}

uint8_t check_flag = 0;

void keyboard_clicked(lv_event_t * event)
{
	//Place your event code here
    const char * txt = lv_btnmatrix_get_btn_text(lvgl_second_screen_ui.keyboard_0, lv_btnmatrix_get_selected_btn(lvgl_second_screen_ui.keyboard_0));

    if (!check_flag) {
        if (!strcmp(txt, LV_SYMBOL_BACKSPACE))
            lv_textarea_del_char(lvgl_second_screen_ui.textarea_0);
        else if (!strcmp(txt, LV_SYMBOL_NEW_LINE))
            lv_textarea_add_char(lvgl_second_screen_ui.textarea_0, '\n');
        else if (!strcmp(txt, LV_SYMBOL_LEFT))
            lv_textarea_cursor_left(lvgl_second_screen_ui.textarea_0);
        else if (!strcmp(txt, LV_SYMBOL_RIGHT))
            lv_textarea_cursor_right(lvgl_second_screen_ui.textarea_0);
        else if (!strcmp(txt, LV_SYMBOL_KEYBOARD))
            lv_textarea_set_text(lvgl_second_screen_ui.textarea_0, "");
        else if (!strcmp(txt, LV_SYMBOL_OK)) {
            lv_textarea_set_text(lvgl_second_screen_ui.textarea_0, "");
            check_flag = 1;
        } else if (strcmp(txt, "\x41\x42\x43") && strcmp(txt, "\x61\x62\x63") && strcmp(txt, "\x31\x23"))
            lv_textarea_add_text(lvgl_second_screen_ui.textarea_0, txt);
    }
}
