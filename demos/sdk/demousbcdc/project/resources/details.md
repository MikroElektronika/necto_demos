# USB CDC MSC DEMO

This example demonstrates the use of USB CDC MassStorage Class demo.
It will allow a Device to be recognized as a MSC storage device when connected to PC via USB.

- **Authors**     : [MikroE](https://github.com/MikroElektronika)
                  , [Hathach](https://github.com/hathach/tinyusb)
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

## Hardware Support

Before starting, put switches PSW and ID on your board to ON position like this:

![image](https://download.mikroe.com/images/click_for_ide/board-uni-ds-v8-usb.png)

Connect USB-C to USB connector on the right side of the board.

## Software Support

We provide demo code for testing USB CDC MSC functionality.
For example to work, proper clock setup is needed. To create a setup with the adequate clock scheme, follow these steps:

1. Click on Setups and select New.
2. Select GCC ARM or RISC-V compiler and click Next.
3. Select Board you are using.
4. Select your MCU and click on Advanced.
5. Select Config scheme from drop down menu, it will have MCU_NAME_USB name.
6. If you dont see MCU_NAME_USB scheme for your MCU then USB module is not supported.
7. Click Save, then Next.
8. Select desired display, and choose programmer/debugger tool.
9. Click Finish.
10. Program the MCU.
11. TinyUSB MSC storage device will apear on your screen as in image below.

![example](https://download.mikroe.com/images/click_for_ide/demo-usb-cdc.png)

---
