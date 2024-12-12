# Digital Input Output DEMO

This example demonstrates digital input and digital output functionalities.
Turn on switches for PORT LEDS and BUTTONS in the BOARD SETUP section of your board,
follow the steps and see how the LEDs on port A respond to pushing buttons.

---

- **Authors**     : [MIKROE](https://github.com/MikroElektronika)
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware Support

- Put PORT LED switches in ON position.
  - switch A for MCUs with 8-bit port
  - switches A and B for MCUs with 16-bit port
  - switches A, B, C and D for MCUs with 32-bit port
- Put BUTTON switches in ON position.
  - switch B for MCUs with 8-bit port
  - switches C and D for MCUs with 16-bit and 32-bit port

![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demodigitalinputoutput/demo_digital_input_output_switches.png)

- Put switches for buttons on the left side of the board in PULL DOWN position
  - in case of 8-bit port:
    - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demodigitalinputoutput/demo_digital_input_output_8bit.png)
  - in case of 16-bit port:
    - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demodigitalinputoutput/demo_digital_input_output_16bit.png)

---

## Software Support

We provide demo code for testing digital input and output functionalities. To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate setup and the project will open.
5. Program it by clicking on **Flash**.
6. Buttons 0 to 4 correspond to buttons PB0 to PB4, respectively. Depending on the pushed button, the following will happen:
   - BUTTON_0 -> LEDs PA0 to PA4 will light up
   - BUTTON_1 -> LEDs PA0 to PA4 will turn off
   - BUTTON_2 -> LEDs PA0 to PA4 will toggle
   - BUTTON_3 -> LED PA3 will light up
   - BUTTON_4 -> LED PA4 will turn off
