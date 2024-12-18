/**
 * @file lv_demo_widgets.h
 *
 */

#ifndef LV_DEMO_WIDGETS_H
#define LV_DEMO_WIDGETS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include <preinit.h>
#endif

#ifdef __GNUC__
#include <delays.h>
#endif

#include "display_lvgl.h"
#include "lv_port_indev.h"
#include "1ms_Timer.h"
#include "screens.h"

/*********************
 *      DEFINES
 *********************/
#define PD_SETUP IS_PD_SETUP

#if PD_SETUP
#define PD_SCROLL_DELAY_MS 7000
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
#if PD_SETUP
static volatile bool secondCount = false;
static uint8_t widget_num = 1;
static lv_coord_t coordinates[5];
#endif

/**********************
 *  GLOBAL VARIABLES
 **********************/
#if PD_SETUP
extern lv_obj_t * t1;
extern lv_obj_t * chart1_cont;
extern lv_obj_t * chart2_cont;
extern lv_obj_t * meter1;
extern lv_obj_t * meter2;
extern lv_obj_t * meter3;
#endif

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_demo_widgets(void);

/**********************
 *      MACROS
 **********************/
#if PD_SETUP
#if TFT_WIDTH==800
#define WIDGET_COUNT 2
#else
#define WIDGET_COUNT 4
#endif
#define METER1_OFFSET 10
#define METER2_OFFSET METER1_OFFSET
#define METER3_OFFSET METER1_OFFSET
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_WIDGETS_H*/
