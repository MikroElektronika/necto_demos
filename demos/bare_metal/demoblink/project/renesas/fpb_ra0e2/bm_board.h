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
    { R_PORT0, 1 << 15 },   // MIKROBUS_1_AN (A0)
    { R_PORT0, 1 << 12 },   // MIKROBUS_1_RST (A3)
    { R_PORT1, 1 << 4 },    // MIKROBUS_1_CS (D10)
    { R_PORT5, 1 << 0 },    // MIKROBUS_1_SCK (D13)
    { R_PORT5, 1 << 2 },    // MIKROBUS_1_CIPO (D12)
    { R_PORT5, 1 << 1 },    // MIKROBUS_1_COPI (D11)
    { R_PORT3, 1 << 1 },    // MIKROBUS_1_PWM (D6)
    { R_PORT4, 1 << 11 },   // MIKROBUS_1_INT (D2)
    { R_PORT1, 1 << 10 },   // MIKROBUS_1_RX (D0)
    { R_PORT1, 1 << 9 },    // MIKROBUS_1_TX (D1)
    { R_PORT0, 1 << 8 },    // MIKROBUS_1_SCL (A5)
    { R_PORT0, 1 << 9 },    // MIKROBUS_1_SDA (A4)
    { R_PORT0, 1 << 14 },   // MIKROBUS_2_AN (A1)
    { R_PORT0, 1 << 13 },   // MIKROBUS_2_RST (A2)
    { R_PORT1, 1 << 5 },    // MIKROBUS_2_CS (D9)
    { R_PORT5, 1 << 0 },    // MIKROBUS_2_SCK (D13)
    { R_PORT5, 1 << 2 },    // MIKROBUS_2_CIPO (D12)
    { R_PORT5, 1 << 1 },    // MIKROBUS_2_COPI (D11)
    { R_PORT2, 1 << 1 },    // MIKROBUS_2_PWM (D5)
    { R_PORT4, 1 << 9 },    // MIKROBUS_2_INT (D3)
    { R_PORT1, 1 << 9 },    // MIKROBUS_2_RX (D0)
    { R_PORT1, 1 << 10 },   // MIKROBUS_2_TX (D1)
    { R_PORT0, 1 << 8 },    // MIKROBUS_2_SCL (A5)
    { R_PORT0, 1 << 9 },    // MIKROBUS_2_SDA (A4)
};

/**
 * @brief On-board LED pin definitions.
 *
 * @details Each LED pin is defined as a @ref Pin structure specifying the port
 * and pin mask for that LED. Modify these entries to match the LEDs
 * available on the target board.
 */
static const Pin board_leds[] = {
    { R_PORT1, 1 << 3 },   // LD1
    { R_PORT1, 1 << 2 },   // LD2
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