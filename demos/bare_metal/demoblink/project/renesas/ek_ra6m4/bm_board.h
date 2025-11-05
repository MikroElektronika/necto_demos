#include "mcu.h"

/**
 * @brief Structure representing a GPIO pin abstraction.
 *
 * @details The Pin structure encapsulates a port reference and
 * the corresponding bit mask for a specific pin.
 */
typedef struct {
    R_PORT0_Type *port;   // base address of the port
    uint16_t pin_mask;    // bit mask of the pin
} Pin;

/**
 * @brief MikroBUS pin definitions for the current board.
 *
 * @details Each element in this array defines the corresponding MikroBUS signal
 * mapped to a specific MCU port and pin mask.
 */
static const Pin mikrobus_pins[] = {
    { R_PORT, 1 << 0 },     // AN
    { R_PORT, 1 << 15 },    // RST
    { R_PORT, 1 << 5 },     // CS
    { R_PORT, 1 << 4 },     // SCK
    { R_PORT, 1 << 2 },     // CIPO
    { R_PORT, 1 << 3 },     // COPI
    { R_PORT, 1 << 8 },     // PWM
    { R_PORT, 1 << 9 },     // INT
    { R_PORT, 1 << 14 },    // RX
    { R_PORT, 1 << 13 },    // TX
    { R_PORT, 1 << 12 },    // SCL
    { R_PORT, 1 << 11 },    // SDA
};

/**
 * @brief On-board LED pin definitions.
 *
 * @details Each LED pin is defined as a @ref Pin structure specifying the port
 * and pin mask for that LED. Modify these entries to match the LEDs
 * available on the target board.
 */
static const Pin board_leds[] = {
    { R_PORT4, 1 << 15 },  // LD1
    { R_PORT4, 1 << 4 },   // LD2
    { R_PORT4, 1 << 0 },   // LD3
};

/**
 * @brief Bare Metal Pin Configuration routine.
 * @details This function sets pins as outputs and drives them to low state.
 * @return None
 */
void initilize_pins( void );

/**
 * @brief Bare Metal Pin Toggling routine.
 * @details This function toggles defined pins every second.
 * @return None
 */
void toggle_pins( void );