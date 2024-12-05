# Analog Input DEMO

This is a simple example of analog-digital converter that utilizes mikroSDK 2.0 ADC driver library. Conversion results are represented on port A (and port B in case of 8-bit MCU) LEDs and can also be seen on the Standard output - Application output or UART, determined by the chosen setup.

---

- **Author**      : MIKROE
- **Version**     : 2.0.0.22
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware support

- Analog input voltage needs to be in range from 0 to VREF volts on the MIKROBUS_1_AN pin. Place a click with potentiometer in mikroBUS 1, such as [**POT click**](https://www.mikroe.com/pot-click).
  - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demoanalogin/demo_analog_input_pot_click.png)
- Turn on PORT LEDS
  - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demo_port_leds_switch.png)

## Software Support

We provide code for demonstrating the usage of functions contained in Analog In driver library. To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate setup and the project will open.
5. Program the example by clicking on **Flash**.
6. You should be able to see the digital representation of the voltage value on port A.
7. NOTE: ADC values can also be visible in Application output through debug or on UART terminal if setup with that selection is chosen.

![example](https://download.mikroe.com/images/mikrosdk/v2/demos/demoanalogin/demo_analog_input_output.png)

---
