#include "main_screen.h"
#include "second_screen.h"
#include "scr_second_screen.h"

static vtft_t *_vtft;

// Variable for defining progress bar position.
uint8_t progress_bar_count = 0;

void second_screen_show(vtft_t *vtft)
{
    _vtft = vtft;
    if( !get_second_screen_initialized() )
        initialize_second_screen();

    vtft_set_current_screen(_vtft, second_screen.screen_data);
}

// Function call of RED! button press event.
void red_trigger()
{
	second_screen_data.color = GL_RED;
    second_screen.Label_0->text.caption = "Screen is now RED!";
    second_screen.Label_0->text.font.color = GL_WHITE;
    vtft_draw_component(_vtft, second_screen.Label_0);
    vtft_refresh_current_screen(_vtft);
}

// Function call of GREEN! button press event.
void green_trigger()
{
	second_screen_data.color = GL_GREEN;
    second_screen.Label_0->text.caption = "Screen is now GREEN!";
    second_screen.Label_0->text.font.color = GL_BLUE;
    vtft_draw_component(_vtft, second_screen.Label_0);
    vtft_refresh_current_screen(_vtft);
}

// Function call of BLACK! button press event.
void black_trigger()
{
	second_screen_data.color = GL_BLACK;
    second_screen.Label_0->text.caption = "Screen is now BLACK!";
    second_screen.Label_0->text.font.color = GL_GREEN;
    vtft_draw_component(_vtft, second_screen.Label_0);
    vtft_refresh_current_screen(_vtft);
}

// Function call of WHITE! button press event.
void white_trigger()
{
	second_screen_data.color = GL_WHITE;
    second_screen.Label_0->text.caption = "Screen is now WHITE!";
    second_screen.Label_0->text.font.color = GL_BLACK;
    vtft_draw_component(_vtft, second_screen.Label_0);
    vtft_refresh_current_screen(_vtft);
}

// Function call of BLUE! button press event.
void blue_trigger()
{
	second_screen_data.color = GL_BLUE;
    second_screen.Label_0->text.caption = "Screen is now BLUE!";
    second_screen.Label_0->text.font.color = GL_RED;
    vtft_draw_component(_vtft, second_screen.Label_0);
    vtft_refresh_current_screen(_vtft);
}

// Function call of INCREASE! button press event.
void increase_trigger()
{
	progress_bar_count += 1;
    vtft_set_progress_bar_position(_vtft, second_screen.ProgressBar_0, progress_bar_count);
    if (progress_bar_count == 10)
		vtft_refresh_current_screen(_vtft);
    else if (progress_bar_count >= 100){
        main_screen_show(_vtft);
		Delay_ms(3000);
        second_screen_show(_vtft);
        progress_bar_count = 0;
        vtft_set_progress_bar_position(_vtft, second_screen.ProgressBar_0, progress_bar_count);
    }
}

// Function call of DECREASE! button press event.
void decrease_trigger()
{
    if (progress_bar_count != 0) {
        progress_bar_count -= 1;
        vtft_set_progress_bar_position(_vtft, second_screen.ProgressBar_0, progress_bar_count);
    }
    if (progress_bar_count == 9)
		vtft_refresh_current_screen(_vtft);
}
