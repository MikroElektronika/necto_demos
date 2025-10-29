# PLOT WAVEFORM DEMO

This is a waveform visualization example that utilizes the **MikroSDK.Plot** library to generate and display multiple signal types such as **Sine**, **Triangle**, **Square**, **Sawtooth**, **Pulse**, and **Random** waves.

---

- **Authors**     : [MIKROE](https://github.com/MikroElektronika)
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Hardware Support

This example runs on all development boards supported by **mikroSDK 2.0**, as it uses only internal MCU resources and the **MikroSDK.Plot** library for visualization.
No external hardware components are required.

- Simply **connect your board via USB**, open the IDE’s **Plot window**, and observe the generated waveform signals in real time.

![plot_demo](https://download.mikroe.com/images/mikrosdk/v2/demos/demoplot/demo_plot_preview.png)

---

## Software Support

This demo shows how to use the **MikroSDK.Plot** interface to visualize numerical data generated in real time.
You can select any waveform type, frequency, or duty cycle directly in code.

### Supported waveform types

- **Sine Wave**
- **Triangle Wave**
- **Square Wave**
- **Sawtooth Wave**
- **Pulse Wave (with duty cycle control)**
- **Random (noise-like) Wave**

---

## How to Run the Example

1. Install the package from the **Package Manager**.
2. Open the example project `plot_waveform_demo`.
3. Choose or create an adequate **Setup** for your hardware.
4. In the source file `main.c`, set the waveform type macros at the top:

   ```c
   #define WAVE_SINE      (1)
   #define WAVE_SQUARE    (0)
   #define WAVE_TRIANGLE  (0)
   #define WAVE_SAWTOOTH  (0)
   #define WAVE_PULSE     (0)
   #define WAVE_RANDOM    (0)
   ```

5. Build and flash the example to your board or run simulator if applicable.
6. Open **Tools → Plot** in NECTO Studio.
7. Press **Run** to visualize waveform output.
8. You’ll see dynamically plotted signals updating in real time.
9. For the random wave, values regenerate automatically after each full cycle.
10. Optionally, call `plot_mikroe_text();` to display the text **MIKROE** in the plot.

---

## Example Preview

| Waveform | Preview |
|-----------|----------|
| **Sine Wave** | ![sine_wave](https://download.mikroe.com/images/mikrosdk/v2/demos/demoplot/demo_sine.png) |
| **Square Wave** | ![square_wave](https://download.mikroe.com/images/mikrosdk/v2/demos/demoplot/demo_square.png) |
| **Triangle Wave** | ![triangle_wave](https://download.mikroe.com/images/mikrosdk/v2/demos/demoplot/demo_triangle.png) |
| **Sawtooth Wave** | ![sawtooth_wave](https://download.mikroe.com/images/mikrosdk/v2/demos/demoplot/demo_sawtooth.png) |
| **Pulse Wave** | ![pulse_wave](https://download.mikroe.com/images/mikrosdk/v2/demos/demoplot/demo_pulse.png) |
| **Random Wave** | ![random_wave](https://download.mikroe.com/images/mikrosdk/v2/demos/demoplot/demo_random.png) |

---

## Project Structure

```txt
demoplot/project
├── src/
│   └── main.c                     # Main example source file
├── README.md                      # This documentation file
└── CMakeLists.txt                 # Project build configuration
```

---

## Features Demonstrated

- Generation of multiple **analog-style waveforms** using mathematical functions.
- Real-time **plotting and visualization** using **MikroSDK.Plot**.
- Adjustable **frequency multiplier** and **duty cycle** parameters.
- Unified waveform generator function for easy extensibility.
- Optional **MIKROE text plotting** for visual branding.
- Full **Doxygen-style documentation** for educational clarity.

---

## Additional Notes

- Modify `frequency_multiplier` in `main.c` to increase or decrease the number of waveform cycles per buffer.
- Adjust `Delay_ms()` timing to control waveform scroll speed in the Plot window.
- The demo works entirely in software and does not require external analog hardware.
- The example is ideal for demonstrating **data visualization**, **signal generation**, and **plot interface usage** within MikroSDK-based projects.

---

![mikroe_logo](http://www.mikroe.com/img/designs/beta/logo_small.png)

---
