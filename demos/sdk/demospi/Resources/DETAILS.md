# SPI DEMO

This is a simple example that utilizes mikroSDK 2.0 SPI driver library. In this demo, we use SRAM click to write 20 bytes of data and read them back. Written and read data is printed out on the Standard output - Application output or UART, determined by the chosen setup.

---

- **Author**      : MIKROE
- **Version**     : 2.0.0.5
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware support

- Place [**SRAM click**](https://www.mikroe.com/sram-click) in mikroBUS 1
- Place [**Tester click**](https://www.mikroe.com/tester-click) in mikroBUS 2

![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demospi/demo_spi_clicks.png)

- Note: You can also use a logic analyzer to see signals and data.

## Software Support

We provide code for demonstrating the usage of functions contained in SPI driver library. To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate setup and the project will open.
5. Program it by clicking on **Flash**.
6. If transfer completes successfully, **INT** pin on the Tester click will light up.

Transferred data can also be seen on the Application output or UART terminal, depending on the chosen setup.

![example](https://download.mikroe.com/images/mikrosdk/v2/demos/demospi/demo_spi_output.png)

---
