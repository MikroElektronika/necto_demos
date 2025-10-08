# Bare Metal Motor Demo

This is a simple example of LED toggling application using only Bare Metal code with [STM32 Nucleo-144 development board with STM32H755ZI MCU](https://www.st.com/en/evaluation-tools/nucleo-h755zi-q.html). This example toggles user LEDs on NUCLEO-144 Board with Dual Core STM32H755ZI MCU using primary Cortex-M7 Core to control the toggling of LD1 and LD3 LEDs and secondary Cortex-M4 Core to control the toggling of LD2 LED. Different speed of GPIO toggling is achieved by adjusting DELAY_COUNTER macro value in source files for both cores.

---

- **Authors**     : [MIKROE](https://github.com/MikroElektronika)
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware support

### Connect CODEGRIP to external programming pins on the board and power the board

## Software Support

This demo provides full code for the following boards:
   - [STM32 Nucleo-144 development board with STM32H755ZI MCU](https://www.st.com/en/evaluation-tools/nucleo-h755zi-q.html)

To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one with appropriate dual core MCU.
4. Choose the created dual core bare-metal setup and the project will open.
5. Program the example by clicking on **Debug**.
6. Control the run of each core by switching between them in Debug control panel.
7. You should be able to see different LEDs blinking while running the code on different cores.

---
