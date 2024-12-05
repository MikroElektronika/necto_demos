/*
 * MikroSDK - MikroE Software Development Kit
 * Copyright© 2023 MikroElektronika d.o.o.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE. *
 *
 */

/*!
 * \file digital_in_out.h
 * \brief This file contains functions for demonstration of working with digital input and output SDK driver libraries.
 */

// ----------------------------------------------------------------------------
#ifndef _DIGI_IN_OUT_H_
#define _DIGI_IN_OUT_H_
// ------------------------------------------------------------------- INCLUDES
#include "drv_digital_out.h"
#include "drv_digital_in.h"
// -------------------------------------------------------------- PUBLIC MACROS

// If not defined, define PIN_HIGH as High Voltage level for pin.
#ifndef PIN_HIGH
#define PIN_HIGH 1
#endif

// If not defined, define PIN_LOW as Low Voltage level for pin.
#ifndef PIN_LOW
#define PIN_LOW 0
#endif

// If hardware without PA0 pin is used, make sure to define adequate pin.
#ifndef PA0
#define LED_0 NC
#else
#define LED_0 PA0
#endif

// If hardware without PA1 pin is used, make sure to define adequate pin.
#ifndef PA1
#define LED_1 NC
#else
#define LED_1 PA1
#endif

// If hardware without PA2 pin is used, make sure to define adequate pin.
#ifndef PA2
#define LED_2 NC
#else
#define LED_2 PA2
#endif

// If hardware without PA3 pin is used, make sure to define adequate pin.
#ifndef PA3
#define LED_3 NC
#else
#define LED_3 PA3
#endif

// If hardware without PA4 pin is used, make sure to define adequate pin.
#ifndef PA4
#define LED_4 NC
#else
#define LED_4 PA4
#endif

// If hardware without PB0 pin is used, make sure to define adequate pin.
#ifndef PB0
#define BUTTON_0 NC
#else
#define BUTTON_0 PB0
#endif

// If hardware without PB1 pin is used, make sure to define adequate pin.
#ifndef PB1
#define BUTTON_1 NC
#else
#define BUTTON_1 PB1
#endif

// If hardware without PB2 pin is used, make sure to define adequate pin.
#ifndef PB2
#define BUTTON_2 NC
#else
#define BUTTON_2 PB2
#endif

// If hardware without PB3 pin is used, make sure to define adequate pin.
#ifndef PB3
#define BUTTON_3 NC
#else
#define BUTTON_3 PB3
#endif

// If hardware without PB4 pin is used, make sure to define adequate pin.
#ifndef PB4
#define BUTTON_4 NC
#else
#define BUTTON_4 PB4
#endif

// --------------------------------------------------------------- PUBLIC TYPES
/**
 * Structure containing 5 output type pins
 */
typedef struct
{
    digital_out_t out0;
    digital_out_t out1;
    digital_out_t out2;
    digital_out_t out3;
    digital_out_t out4;
} outputs_t;

// ----------------------------------------------- PUBLIC FUNCTION DECLARATIONS

/**
 * @brief Puts pins on high logic level
 * @details Function that puts five pins from structure on high logic level
 * @param[in] output_pins - Structure that contains pins that should be put on high logic level
 */
void pins_low( outputs_t *output_pins );

/**
 * @brief Puts pins on low logic level
 * @details Function that puts five pins from structure on low logic level
 * @param[in] output_pins - Structure that contains pins that should be put on low logic level
 */
void pins_high( outputs_t *output_pins );

/**
 * @brief Toggles pin values
 * @details Function that toggles values on five pins from the structure
 * @param[in] output_pins - Structure that contains pins that should be toggled
 */
void pins_toggle( outputs_t *output_pins );

#endif //_DIGI_IN_OUT_H_

// ------------------------------------------------------------------------ END
