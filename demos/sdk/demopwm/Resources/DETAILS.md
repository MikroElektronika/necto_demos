# PWM DEMO

This is a simple example that utilizes mikroSDK 2.0 PWM driver library. In this demo, we demonstrate how to configure and enable PWM module and adjust the duty cycle using buttons. Increasing the duty cycle results in a brighter LED illumination, while decreasing it reduces the brightness.

- **Author**      : MIKROE
- **Version**     : 2.0.0.19
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware Support

- Put PORT LED switches in ON position.
  - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demopwm/demo_pwm_switches.png)

- Put switches for buttons on the left side of the board in PULL DOWN position
  - in case of 8-bit port:
    - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demopwm/demo_pwm_8bit.png)
  - in case of 16-bit port:
    - ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demopwm/demo_pwm_16bit.png)

## Software Support

We provide demo code for demonstrating the usage of functions contained in PWM driver library. To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate setup and the project will open.
5. Program it by clicking on **Flash**.
6. Press PA0 to increase duty ratio or PB0 to decrease it and see how it translates to the PWM pin LED.

---
