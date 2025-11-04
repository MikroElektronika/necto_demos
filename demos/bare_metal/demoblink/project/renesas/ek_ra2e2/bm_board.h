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
    { R_PORT0, 1 << 15 },   // AN
    { R_PORT0, 1 << 14 },   // RST
    { R_PORT1, 1 << 3 },    // CS
    { R_PORT1, 1 << 2 },    // SCK
    { R_PORT1, 1 << 0 },    // CIPO
    { R_PORT1, 1 << 1 },    // COPI
    { R_PORT1, 1 << 9 },    // PWM
    { R_PORT1, 1 << 10 },   // INT
    { R_PORT1, 1 << 11 },   // RX
    { R_PORT1, 1 << 12 },   // TX
    { R_PORT4, 1 << 0 },    // SCL
    { R_PORT4, 1 << 1 },    // SDA
};

/**
 * @brief On-board LED pin definitions.
 *
 * @details Each LED pin is defined as a @ref Pin structure specifying the port
 * and pin mask for that LED. Modify these entries to match the LEDs
 * available on the target board.
 */
static const Pin board_leds[] = {
    { R_PORT1, 1 << 12 },   // LD1
    { R_PORT9, 1 << 14 },   // LD2
    { R_PORT0, 1 << 15 },   // LD3
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