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
 * OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*!
 * \file digital_in_out.c
 *
 */

#include "digital_in_out.h"

#ifdef MikroCCoreVersion
    #if MikroCCoreVersion >= 1
        #include "delays.h"
    #endif
#endif

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

void pins_low( outputs_t *output_pins ) {
    digital_out_low( &output_pins->out0);
    digital_out_low( &output_pins->out1);
    digital_out_low( &output_pins->out2);
    digital_out_low( &output_pins->out3);
    digital_out_low( &output_pins->out4);
}

void pins_high( outputs_t *output_pins ) {
    digital_out_high( &output_pins->out0);
    digital_out_high( &output_pins->out1);
    digital_out_high( &output_pins->out2);
    digital_out_high( &output_pins->out3);
    digital_out_high( &output_pins->out4);
}

void pins_toggle( outputs_t *output_pins ) {
    digital_out_toggle( &output_pins->out0);
    digital_out_toggle( &output_pins->out1);
    digital_out_toggle( &output_pins->out2);
    digital_out_toggle( &output_pins->out3);
    digital_out_toggle( &output_pins->out4);
    Delay_ms( 300 );
}

// ------------------------------------------------------------------------- END
