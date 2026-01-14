# Bare Metal Motor Demo

This is a simple example of motor application using only Bare Metal code with [Brushless 17 Click Board](https://www.mikroe.com/brushless-17-click?srsltid=AfmBOopg-0DSnyf8LlcVOCHdo3DOmrCkdNPWK2iiMt7stItPYlmLa2se) and [BLDC Motor with Hall sensor](https://www.mikroe.com/motor-bldc-42blf01-with-hall-sensor). It rotates the motor in 2 directions - clockwise and counter-clockwise. User specifies the sweep time and duty cycle goes from 40% to 100% and back during 2 sweep periods.

---

- **Authors**     : [MIKROE](https://github.com/MikroElektronika)
- **Version**     : 3.1.0
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware support

### Wire the motor and Click board
   **Motor Wires**
   - **Yellow** → 1
   - **Green** → 2
   - **Blue** → 3

   **Motor Base Wires**
   - **Blue** → H1
   - **Green** → H2
   - **Yellow** → H3
   - **Red** → +5V
   - **Black** → GND

   **External Power Supply**
   - **Ground** → GND
   - **Output** → +12V

> **Analog Pin Note:**
>> The **analog pin** is used as a break trigger when grounded. It should be wired to **5V** using the **Terminal Click** and a **jumper**, since some MCUs don't allow configuring `AN` pin as digital high with enough voltage.

## Software Support

This demo provides full code for the following boards:
   - [RA4M1 Clicker](https://www.mikroe.com/ra4m1-clicker?srsltid=AfmBOorwH7gUaApWiWyyXVQRQNEHzeMCsVPUxxFcD2GihE-hPBD5dmTt)

For other boards and MCUs you will have to follow `TODO` comments and adjust the code manually.

To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate bare-metal setup and the project will open.
5. Program the example by clicking on **Flash**.
6. You should be able to see the motor working.

---
