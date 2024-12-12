# USB MIDI

Code developed on [UNI-DS v8](https://www.mikroe.com/uni-ds-v8) and it can be used on other MIKROE development boards as well. Code was developed with purpose of easier understanding functions for USB Musical Instrument Digital Interface (MIDI).

---

- **Authors**     : [MikroE](https://github.com/MikroElektronika)
                  , [Hathach](https://github.com/hathach/tinyusb)
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

---

## Code description

Depending on the numbers in the **note_sequence** array, different melodies will play. By default there is a "Happy Birthday" melody playing.

---

## Hardware Support

![image](https://download.mikroe.com/images/click_for_ide/board-uni-ds-v8-usb.png)

This code was developed on *UNI-DS v8* board. For this hardware to work according to program, it is necessary to:

- Pull *USB/UART* switches that turn on **PSW** and **ID** lines (in the *BOARD SETUP* part, on picture -> bigger red square on the right);
- Connect USB-C to USB connector (in the *USB* part, on picture -> smaller red square on the right);

### Functions Description

All functions used in main project are located in *MikroSDK.USB.Tiny* and *MikroSDK.USB.HW*.

Functions in main file:

- ```bool tusb_init(void)```
  - initializes device/host stack;
- ```uint32_t tud_midi_available(void)```
  - gets the number of bytes available for reading;
- ```bool tud_midi_packet_read(uint8_t packet[4])```
  - reads event packet;
- ```uint32_t tud_midi_stream_write(uint8_t cable_num, uint8_t const* buffer, uint32_t bufsize)```
  - writes byte stream;

## Software Support

We provide demo code for testing all MIDI functions contained in libraries *MikroSDK.USB.Tiny* and *MikroSDK.USB.HW*, available in [mikroSDK 2.0](https://github.com/MikroElektronika/mikrosdk_v2).

In order to make this example work, proper clock setup is needed. To create a setup with the adequate clock scheme, follow these steps:

1. Click on **Setups** and select **New**.
2. Choose **GCC ARM** or **RISC-V GNU** compiler.
3. Choose the board you are using.
4. Choose the MCU that has USB functionality and click **Advanced** to enter clock configuration window.
5. In the **Config scheme** field choose the option with *_USB* in the end.
6. If there is no such option, then either USB for this MCU is not supported in current version of SDK, or this MCU doesn't have USB functionality.
7. Complete the setup.
8. Program the MCU with created setup.
9. Download [MIDI-OX](http://www.midiox.com/) application if using windows OS.
10. Open MIDI-OX application and you will be able to hear the melody that you programmed.
11. If you have an error **No MIDI devices** when you're opening MIDI-OX, reset the MCU.
12. If you see the MIDI notes on the screen, but don't hear the melody, go to **Options** -> **MIDI Devices** and choose **Microsoft GS Wavetable Synth** in the *MIDI Outputs* field.

---
