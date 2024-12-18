/**
 * @file lv_port_disp_templ.c
 *
 */
#include "gl.h"
#include "touch_controller.h"
#include "gl.h"
#include "ssd1963.h"
#include <me_built_in.h>

#define PORTG_ODR 0x40021814
#define WR_PIN_BSRR 0x40021418

#define CS_PIN_BSRR 0x40021418
#define CMD_SEL_BSRR 0x40021418

static gl_driver_t display_driver;



/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "display_lvgl.h"
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    _TFT_WIDTH_
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    _TFT_HEIGHT_
#endif

#define PORTG_BSRR 
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);
static inline void write_array_data(uint16_t *array, uint16_t length);
static inline void write_command(uint8_t command);
static inline void write_param(uint8_t param);


inline void write_command(uint8_t command)
{
    *(uint32_t *)(CS_PIN_BSRR) = (uint32_t)(0x1000 << 16); //cs_active 
    *(uint32_t *)(CMD_SEL_BSRR) = (uint32_t)(0x2000 << 16); //cmd sel
    *(uint32_t *)(PORTG_ODR) = command;
    *(uint32_t *)(WR_PIN_BSRR) = (uint32_t)(0x8000 << 16); //clear wr pin
    *(uint32_t *)(WR_PIN_BSRR) = (uint32_t)(0x8000); //set wr pin
    *(uint32_t *)(CS_PIN_BSRR) = (uint32_t)(0x1000); //cs_active 
}

inline void write_param(uint8_t param)
{
    *(uint32_t *)(CS_PIN_BSRR) = (uint32_t)(0x1000 << 16); //cs_active 
    *(uint32_t *)(CMD_SEL_BSRR) = (uint32_t)(0x2000); //cmd sel
    *(uint32_t *)(PORTG_ODR) = param;
    *(uint32_t *)(WR_PIN_BSRR) = (uint32_t)(0x8000 << 16); //clear wr pin
    *(uint32_t *)(WR_PIN_BSRR) = (uint32_t)(0x8000); //set wr pin
    *(uint32_t *)(CS_PIN_BSRR) = (uint32_t)(0x1000); //cs_active 
}


inline void write_array_data(uint16_t *array, uint16_t length)
{
    uint32_t i;
    for (i =0; i < length; i++)
    {
//         uint32_t val = (*array & 0xFFFF) | ( (uint32_t)(~(*array) & 0xFFFF) << 16);
        *(uint32_t *)(PORTG_ODR) = *array;
        *(uint32_t *)(WR_PIN_BSRR) = (uint32_t)(0x8000 << 16); //clear wr pin
        *(uint32_t *)(WR_PIN_BSRR) = (uint32_t)(0x8000); //set wr pin
//         strobe();
//         gl_write_frame(*array);
        array++;
    }
}
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();
    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    /* Example for 1) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[20000];                          /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, 20000);   /*Initialize the display buffer*/

    /* Example for 2) */
//     static lv_disp_draw_buf_t draw_buf_dsc_2;
//     static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
//     static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
//     lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
//     static lv_disp_draw_buf_t draw_buf_dsc_3;
//     static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
//     static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
//     lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
//                           MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
// //     /*You code here*/
    touch_controller_init(&display_driver, TFT_MAX_BACKLIGHT);
// //     // Initialize Graphic library.
    gl_set_driver(&display_driver);
    
//     parallel_init();
//     Delay_ms(100);
//     drv_ssd1963_init();


}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*Return if the area is out the screen*/
    if(area->x2 < 0) return;
    if(area->y2 < 0) return;
    if(area->x1 > MY_DISP_HOR_RES - 1) return;
    if(area->y1 > MY_DISP_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > MY_DISP_HOR_RES - 1 ? MY_DISP_HOR_RES - 1 : area->x2;
    int32_t act_y2 = area->y2 > MY_DISP_VER_RES - 1 ? MY_DISP_VER_RES - 1 : area->y2;

    //begin frame

    uint16_t full_w = area->x2 - area->x1 + 1;
    uint16_t act_w = act_x2 - act_x1 + 1;
    
        /// Orientation dependent.
    uint16_t start_column = (MY_DISP_HOR_RES - 1) - (act_x1 + act_w - 1);
    uint16_t end_column =  (MY_DISP_HOR_RES - 1) - act_x1;
    uint16_t start_page = (MY_DISP_VER_RES - 1) - (act_y1 + act_y2 - act_y1);
    uint16_t end_page = (MY_DISP_VER_RES - 1) - act_y1;
    //start frame
    write_command(0x2A);
    write_param(Hi(start_column));
    write_param(Lo(start_column));
    write_param(Hi(end_column));
    write_param(Lo(end_column));

    write_command(0x2B);
    write_param(Hi(start_page));
    write_param(Lo(start_page));
    write_param(Hi(end_page));
    write_param(Lo(end_page));

    write_command(0x2C);

    *(uint32_t *)(CS_PIN_BSRR) = (uint32_t)(0x1000 << 16);
    *(uint32_t *)(CS_PIN_BSRR) = (uint32_t)(0x2000);
    //start frame
    
    int16_t i;
    for(i = act_y1; i <= act_y2; i++) {
     write_array_data((uint16_t *)color_p, act_w);
     color_p += full_w;
    }
    *(uint32_t *)(CS_PIN_BSRR) = (uint32_t)(0x1000); //cs_active 
    
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
