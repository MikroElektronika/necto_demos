# Led Blinking DEMO

This is a simple LED blinking example, utilizing mikroSDK 2.0 GPIO driver library.
The specified LED will be toggled with period of 1 second. Take into consideration that your hardware
might not have the specified pin.

---

- **Author**      : MIKROE
- **Version**     : 2.0.0.30
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware Support

- Put PORT LED switches in ON position.
- ![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demo_port_leds_switch.png)

---

## Software Support

We provide simple demo code for toggling one pin. To run this example, follow these steps:

1. Install the package from Package manager.
2. Open example.
3. It will take you to setups, so make one for your hardware.
4. Choose adequate setup and the project will open.
5. Program it by clicking on **Flash**.
6. Defined pin (**LED**) should start toggling every second.

**LED** pin is defined as MIKROBUS_1_CS in code, so make sure that pin is routed to port LEDs.
If it isn't, then change **LED** in code. Alternatively, place Tester click in mikroBUS 1.

![image](https://download.mikroe.com/images/mikrosdk/v2/demos/demoledblinking/demo_led_blinking_tester_click.png)
