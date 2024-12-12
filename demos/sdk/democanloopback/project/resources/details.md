# CAN Loopback

Code developed on [UNI-DS v8](https://www.mikroe.com/uni-ds-v8) and it can be used on other MIKROE development boards as well. Code was developed with purpose of easier understanding functions for Controller Area Network Interface (CAN) Loopback Mode.

---

- **Authors**     : [MikroE](https://github.com/MikroElektronika)
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Code description

This is a simple example that utilizes mikroSDK 2.10 CAN driver library.
In this demo we use the internal CAN transceiver module. Sent and received data
are compared at the end of cycle transmission-receiving.

---

## Hardware Support

![image](https://download.mikroe.com/images/mikrosdk/v2/demos/democan/demo-can-pins-mcu-card.png)
![image](https://download.mikroe.com/images/mikrosdk/v2/demos/democan/demo-can-pins-easypic-board.png)

This code was developed on *UNI-DS v8* board. For this hardware to work according to program, it is necessary to:

- For NXP, PIC32 and RISC-V MCUs provide CAN signals with the same baud rate values to CAN H and CAN L pins on MCU card or board.

## Software Support

We provide code for demonstrating the usage of functions contained in CAN driver library. To run this example, follow these steps:

In order to make this example work, proper clock setup is needed. To create a setup with the adequate clock scheme, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate setup and clock configuration and the project will open.
5. Define test pins using TEST_PIN_x macros according to your hardware
6. Program it by clicking on **Flash**.
7. If transfer completes successfully, **TEST_PIN_SUCCESS** pin on the Board/Tester click will light up.

---
