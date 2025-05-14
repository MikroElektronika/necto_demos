# Bare Metal Motor Demo

This is a simple example of motor application using only Bare Metal code with Brushless 17 Click Board and [BLDC Motor with Hall sensor](https://www.mikroe.com/motor-bldc-42blf01-with-hall-sensor).

---

- **Authors**     : [MIKROE](https://github.com/MikroElektronika)
- **Version**     : 3.0.0
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

   **Analog Pin Note**
   - The **analog pin** should be wired to **3.3V (3v3)** using the **Terminal Click** and a **jumper**, since some MCUs don't allow to configure AN pin as digital high.

## Software Support

We provide code for demonstrating the usage of functions of Bare Metal files. To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate bare-metal setup and the project will open.
5. Program the example by clicking on **Flash**.
6. You should be able to see the motor working.
7. NOTE: Demo Supports only RA4M1 Clicker Board for now.

---
