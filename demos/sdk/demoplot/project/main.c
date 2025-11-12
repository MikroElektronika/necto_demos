/**
 * @file main.c
 * @brief Waveform generator and visualizer using MikroSDK.Plot.
 *
 * This example demonstrates generation of various waveform types
 * (sine, triangle, square, sawtooth, pulse, random) with values in
 * the range 0?64. The generated data can be visualized using the
 * plot() function provided by MikroSDK.Plot.
 *
 * Each waveform type can be enabled by setting its respective
 * WAVE_* macro to 1. Only one should be enabled at a time for clarity.
 *
 * @note In order to get visualized data in NECTO Studio, run the demo
 *       in debug mode. Alternatively, if the MCU has software simulator
 *       supported, you can run a simulation as well.
 */

#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#include <math.h>
#include <stdint.h>
#include <stddef.h>

#ifndef PI
    #define PI 3.14159265358979
#endif

#include "MikroSDK.Plot"

/* ------------------------------------------------------------------------- */
/*                         Waveform Selection Macros                         */
/* ------------------------------------------------------------------------- */
/**
 * @brief Select which waveform(s) to generate.
 *
 * Set one of these to 1 to activate a waveform.
 * Although multiple can be enabled simultaneously, it is usually clearer
 * to visualize one waveform at a time.
 *
 * @note Set desired signals to 1 in order to plot them out.
 *       If a single signal is active, demo runs indefinitely.
 *       If multiple signals are active, runs each signal defined
 *       number of samples, i.e. 64 samples.
 */
#define WAVE_SINE       (1)
#define WAVE_TRIANGLE   (0)
#define WAVE_SQUARE     (0)
#define WAVE_SAWTOOTH   (0)
#define WAVE_PULSE      (0)
#define WAVE_RANDOM     (0)

/* Each bit position corresponds to one waveform */
#define WAVE_SINE_VALUE      (WAVE_SINE << 0)
#define WAVE_TRIANGLE_VALUE  (WAVE_TRIANGLE << 1)
#define WAVE_SQUARE_VALUE    (WAVE_SQUARE << 2)
#define WAVE_SAWTOOTH_VALUE  (WAVE_SAWTOOTH << 3)
#define WAVE_PULSE_VALUE     (WAVE_PULSE << 4)
#define WAVE_RANDOM_VALUE    (WAVE_RANDOM << 5)

/**
 * @brief Combined bitmask representing all enabled signals.
 */
#define ACTIVE_SIGNALS (WAVE_SINE_VALUE | WAVE_SQUARE_VALUE | WAVE_TRIANGLE_VALUE | \
                        WAVE_SAWTOOTH_VALUE | WAVE_PULSE_VALUE | WAVE_RANDOM_VALUE)

/**
 * @brief Number of samples per waveform period.
 */
#define sample_size 64

/* ------------------------------------------------------------------------- */
/*                            Waveform Type Enum                             */
/* ------------------------------------------------------------------------- */
/**
 * @enum WaveformType
 * @brief Enumerates supported waveform types for the generator.
 *
 * The enum values are defined as bit positions so they can be
 * combined or tested directly with ACTIVE_SIGNALS.
 */
typedef enum {
    WAVEFORM_SINE      = 1 << 0,  /**< Smooth sine wave. */
    WAVEFORM_TRIANGLE  = 1 << 1,  /**< Linear up?down triangle wave. */
    WAVEFORM_SQUARE    = 1 << 2,  /**< Sharp-edged square wave (50% duty). */
    WAVEFORM_SAWTOOTH  = 1 << 3,  /**< Linearly ramping sawtooth wave. */
    WAVEFORM_PULSE     = 1 << 4,  /**< Pulse wave with adjustable duty cycle. */
    WAVEFORM_RANDOM    = 1 << 5,  /**< Random waveform (noise-like output). */

    WAVEFORM_COUNT     = 6        /**< Total number of waveform types. */
} WaveformType;

/* ------------------------------------------------------------------------- */
/*                              Function Declarations                        */
/* ------------------------------------------------------------------------- */

/**
 * @brief Initializes the waveform generator application.
 *
 * This function precomputes and fills global buffers for each selected
 * waveform type based on the configuration macros (WAVE_*). Each waveform
 * type is generated using the `generate_waveform()` helper function.
 *
 * @note Should be called once before entering the main loop.
 */
static void application_init(void);

/**
 * @brief Executes the main waveform visualization loop.
 *
 * If exactly one waveform is enabled, it continuously plots that waveform
 * in an infinite loop.
 * If more than one waveform is enabled, it plots each waveform once and
 * then terminates the function (ideal for comparative demos).
 */
static void application_task(void);

/**
 * @brief Generates a waveform of the specified type into a sample buffer.
 *
 * @param type        Type of waveform to generate (see @ref WaveformType)
 * @param buffer      Pointer to output buffer to store waveform samples
 * @param size        Number of samples (one complete period)
 * @param start_high  Defines the phase:
 *                    - 1 ? starts high or positive
 *                    - 0 ? starts low or negative
 * @param duty_cycle  Duty cycle for pulse waveforms (0.0?1.0); ignored for others
 *
 * This function fills the provided buffer with waveform values ranging
 * from 0 to 255. The output can be used directly with the `plot()` API
 * for waveform visualization.
 */
static void generate_waveform(WaveformType type, uint8_t *buffer,
                              size_t size, int start_high, double duty_cycle);

/**
 * @brief Sends a single sample from the selected waveform to the plot.
 *
 * @param type  Type of waveform being plotted
 * @param step  Sample index within the waveform buffer
 *
 * This function selects the appropriate precomputed waveform buffer
 * and sends one sample value to the `plot()` function for display.
 */
static void plot_wave_signal(WaveformType type, size_t step);

/**
 * @brief Counts the number of active waveforms based on macros.
 *
 * @return Number of enabled waveform types.
 */
static uint8_t count_active_waveforms(void);

/**
 * @brief  Generates a pseudo-random number within the range 0 to max.
 *
 * Uses a simple linear congruential generator (LCG) algorithm to produce a
 * pseudo-random number. The function maintains an internal static seed that
 * updates on each call. The result is always within the range [0, max].
 *
 * @param  max  The maximum value (inclusive) that the random number can reach.
 *
 * @return A pseudo-random number between 0 and max (inclusive).
 */
static uint32_t random_simple(uint32_t max);

/* ------------------------------------------------------------------------- */
/*                              Global Buffers                               */
/* ------------------------------------------------------------------------- */

static uint8_t wave_down[sample_size], wave_up[sample_size];
static uint8_t square_down[sample_size], square_up[sample_size];
static uint8_t tri_down[sample_size], tri_up[sample_size];
static uint8_t saw_up[sample_size], saw_down[sample_size];
static uint8_t pulse_25[sample_size], pulse_75[sample_size];
static uint8_t rand_wave1[sample_size], rand_wave2[sample_size];

/* ------------------------------------------------------------------------- */
/*                                   Main                                    */
/* ------------------------------------------------------------------------- */
int main(void)
{
    /* Perform MCU-specific preinitialization if required */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    /* Initialize and run waveform visualization */
    application_init();

    application_task();

    return 0;
}

/* ------------------------------------------------------------------------- */
/*                              Function Definitions                         */
/* ------------------------------------------------------------------------- */

static void application_init(void)
{
    #if WAVE_SINE
    generate_waveform(WAVEFORM_SINE, wave_down, sample_size, 0, 0);
    generate_waveform(WAVEFORM_SINE, wave_up,   sample_size, 1, 0);
    #endif

    #if WAVE_SQUARE
    generate_waveform(WAVEFORM_SQUARE, square_down, sample_size, 0, 0);
    generate_waveform(WAVEFORM_SQUARE, square_up,   sample_size, 1, 0);
    #endif

    #if WAVE_TRIANGLE
    generate_waveform(WAVEFORM_TRIANGLE, tri_down, sample_size, 0, 0);
    generate_waveform(WAVEFORM_TRIANGLE, tri_up,   sample_size, 1, 0);
    #endif

    #if WAVE_SAWTOOTH
    generate_waveform(WAVEFORM_SAWTOOTH, saw_up,   sample_size, 1, 0);
    generate_waveform(WAVEFORM_SAWTOOTH, saw_down, sample_size, 0, 0);
    #endif

    #if WAVE_PULSE
    generate_waveform(WAVEFORM_PULSE, pulse_25, sample_size, 1, 0.25);
    generate_waveform(WAVEFORM_PULSE, pulse_75, sample_size, 1, 0.75);
    #endif

    #if WAVE_RANDOM
    generate_waveform(WAVEFORM_RANDOM, rand_wave1, sample_size, 0, 0);
    generate_waveform(WAVEFORM_RANDOM, rand_wave2, sample_size, 0, 0);
    #endif
}

/**
 * @brief Count the number of enabled waveform types.
 */
static uint8_t count_active_waveforms(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < WAVEFORM_COUNT; i++)
    {
        if (ACTIVE_SIGNALS & (1 << i))
            count++;
    }
    return count;
}

static void application_task(void)
{
    volatile uint8_t active_count = count_active_waveforms();
    volatile bool infinite_mode = (active_count == 1);

    do
    {
        for (size_t signal_num = 0; signal_num < WAVEFORM_COUNT; signal_num++)
        {
            if (!(ACTIVE_SIGNALS & (1 << signal_num)))
                continue; // Skip disabled waveforms

            for (size_t i = 0; i < sample_size; ++i)
            {
                plot_wave_signal((1 << signal_num), i);
            }

            if (!infinite_mode)
                Delay_ms(1000); // Short pause between waveform types
        }

    } while (infinite_mode); // Run indefinitely if only one waveform is active
}

static void generate_waveform(WaveformType type, uint8_t *buffer,
                              size_t size, int start_high, double duty_cycle)
{
    if (!buffer || size == 0)
        return;

    switch (type)
    {
        case WAVEFORM_SINE:
        {
            double phase = start_high ? -PI / 2.0 : PI / 2.0;
            for (size_t i = 0; i < size; ++i)
            {
                double angle = (2.0 * PI * i) / size;
                double value = sin(angle + phase);
                buffer[i] = (uint8_t)((value + 1.0) * 127.5);
            }
        }
        break;

        case WAVEFORM_TRIANGLE:
        {
            for (size_t i = 0; i < size; ++i)
            {
                double t = (double)i / (double)size;
                double val = (t < 0.5) ? t * 2.0 : 2.0 - (t * 2.0);
                if (!start_high)
                    val = 1.0 - val;
                buffer[i] = (uint8_t)(val * 255.0);
            }
        }
        break;

        case WAVEFORM_SQUARE:
        {
            size_t half = size / 2;
            for (size_t i = 0; i < size; ++i)
                buffer[i] = (start_high ? (i < half) : (i >= half)) ? 255 : 0;
        }
        break;

        case WAVEFORM_SAWTOOTH:
        {
            for (size_t i = 0; i < size; ++i)
            {
                double t = (double)i / (double)(size - 1);
                double val = start_high ? t : (1.0 - t);
                buffer[i] = (uint8_t)(val * 255.0);
            }
        }
        break;

        case WAVEFORM_PULSE:
        {
            if (duty_cycle < 0.0) duty_cycle = 0.0;
            if (duty_cycle > 1.0) duty_cycle = 1.0;

            size_t high_samples = (size_t)(size * duty_cycle);
            for (size_t i = 0; i < size; ++i)
            {
                if (start_high)
                    buffer[i] = (i < high_samples) ? 255 : 0;
                else
                    buffer[i] = (i < high_samples) ? 0 : 255;
            }
        }
        break;

        case WAVEFORM_RANDOM:
        {
            for (size_t i = 0; i < size; ++i)
                buffer[i] = (uint8_t)(random_simple(i) % 256);
        }
        break;

        default:
            break;
    }
}

static void plot_wave_signal(WaveformType type, size_t step)
{
    switch (type)
    {
        case WAVEFORM_SINE:
            plot("Sine Wave Positive Phase", wave_down[step]);
            plot("Sine Wave Negative Phase", wave_up[step]);
            break;

        case WAVEFORM_TRIANGLE:
            plot("Triangle Wave Positive Phase", tri_down[step]);
            plot("Triangle Wave Negative Phase", tri_up[step]);
            break;

        case WAVEFORM_SQUARE:
            plot("Square Wave Positive Phase", square_down[step]);
            plot("Square Wave Negative Phase", square_up[step]);
            break;

        case WAVEFORM_SAWTOOTH:
            plot("Sawtooth Wave Positive Phase", saw_down[step]);
            plot("Sawtooth Wave Negative Phase", saw_up[step]);
            break;

        case WAVEFORM_PULSE:
            plot("Pulse Wave Positive Phase", pulse_25[step]);
            plot("Pulse Wave Negative Phase", pulse_75[step]);
            break;

        case WAVEFORM_RANDOM:
            plot("Random Wave 1", rand_wave1[step]);
            plot("Random Wave 2", rand_wave2[step]);
            break;

        default:
            break;
    }
}

static uint32_t random_simple(uint32_t max) {
    static uint32_t seed = 1; // You can change this initial seed if needed
    seed = seed * 1664525UL + 1013904223UL;  // LCG formula
    return (seed >> 8) % (max + 1);
}

