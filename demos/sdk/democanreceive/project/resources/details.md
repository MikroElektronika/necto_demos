# CAN Receive

Code developed on [UNI-DS v8](https://www.mikroe.com/uni-ds-v8) and it can be used on other MIKROE development boards as well. Code was developed with purpose of easier understanding Controller Area Network Interface (CAN) receive functions.

---

- **Authors**     : MikroE https://github.com/MikroElektronika
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Code description

This is a simple example that utilizes mikroSDK 2.10 CAN driver library.
We recommend using this demo with [CAN Transmit Demo](https://libstock.mikroe.com/projects/view/5632/can-transmit-demo).
In this demo MCU receives data using Normal CAN Mode. Default configuration
for receiving data is set to receive data from another CAN node, configured
with CAN Transmit Demo. In this Demo it is expected to receive 3 data bytes
where the first and the third bytes are constant and the second byte is an 8-bit value
incremented with each reception and displayed on the port defined as TEST_PORT macro.

---

## Hardware Support

This code was developed on *UNI-DS* v8 board. For this hardware to work according to program, you will need the following:

1. Two boards and two MCUs.
2. [Twisted wire](https://www.mikroe.com/twisted-wire-2m) - around 30cm of it
3. MCU cards with CAN connectors (or boards with on-board CAN connectors, such as EasyPIC v8)
   1. See image below
4. Alternatively, instead of soldering additional connectors to your existing MCU cards, you could use [CAN-1 Boards](https://www.mikroe.com/can-1-board)

![easypic_v8_can_connector](https://download.mikroe.com/images/mikrosdk/v2/demos/democan/demo-can-pins-mcu-card.png)
![image](https://download.mikroe.com/images/mikrosdk/v2/demos/democan/demo-can-pins-easypic-board.png)

Note: You might need terminal resistence (aproximately 120 Ohms). This is hardware dependent.

To connect everything properly, follow these steps:

1. Connect CAN connectors with twisted wire. CANH and CANL on one board are connected to CANH and CANL on the other one respectively.
2. If terminal resistance is needed, put a through-hole resistor in one of the connectors (between CANH and CANL of the same connector).
3. If using [CAN-1 Boards](https://www.mikroe.com/can-1-board), place them on adequate headers on boards.

## Software Support

We provide code for demonstrating the usage of functions contained in CAN driver library. To run this example, follow these steps:

In order to make this example work, proper clock setup is needed. To create a setup with the adequate clock scheme, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate setup and clock configuration and the project will open.
5. Define test port using TEST_PORT macro and test pins using **TEST_PIN_x** macros according to your hardware.
6. Program it by clicking on **Flash**.
7. Once **CAN Receive Demo** is programmed and the two boards are connected properly, data should be visible on it on the defined port. Use a logic analyzer to see the signals.
8. If transfer completes successfully, **TEST_PORT** will display received value.
9. If nothing is happenning reset your transmitter board.

---
