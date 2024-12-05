# CAN Transmit

Code developed on [UNI-DS v8](https://www.mikroe.com/uni-ds-v8) and it can be used on other MIKROE development boards as well. Code was developed with purpose of easier understanding functions for Controller Area Network Interface (CAN). CAN Transmit Demo is meant to work with CAN Receive Demo, meaning that one should be programmed on one MCU and the other on another one and boards should be connected properly. Follow the steps to make the example work as it should.

---

- **Authors**     : MikroE https://github.com/MikroElektronika
- **Version**     : 2.0.0.4
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Code description

This is a simple example that utilizes mikroSDK 2.10.0 CAN driver library.
In this demo we send three bytes of data, of which the first and the third one are
the same every time and they're used to make sure transmit and receive are not stuck.
The second one is an 8-bit value that gets incremented in each transmit.

---

## Hardware Support

This code was developed on *UNI-DS v8* board. For this hardware to work according to program, you will need the following:

1. Two boards and two MCUs
2. [Twisted wire](https://www.mikroe.com/twisted-wire-2m) - around 30cm of it
3. MCU cards with CAN connectors (or boards with on-board CAN connectors, such as EasyPIC v8)
    - ![easypic_v8_can_connector](https://download.mikroe.com/images/mikrosdk/v2/demos/democan/demo-can-pins-mcu-card.png)
    - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/democan/demo-can-pins-easypic-board.png)
4. Alternatively, instead of soldering additional connectors to your existing MCU cards, you could use [CAN-1 Boards](https://www.mikroe.com/can-1-board)

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
5. Program it by clicking on **Flash**.
6. Once **CAN Receive Demo** is programmed on the other board and the two boards are connected properly, data should be visible on it on the defined port. Use a logic analyzer to see the signals.

---
