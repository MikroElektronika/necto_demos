# CycloneTCP HTTP Client DEMO

In this demo, the HTTP client running on MCU will connect to the HTTP server "www.httpbin.org", port number 80. Client will then send POST request to HTTP server whith header containing, Host: "www.httpbin.org", User-Agent: "Mozilla/5.0" ,Content-Type: "text/plain", and Transfer-Encoding: "chunked", and parameters/aruments, param1 and param2, having respective values of "value1" and "value2". Body of the message is plain text "Hello World!". Response from the HTTP server will be read, echoing back sent data in json format, and will be written over UART/Application output.

- **Authors**     : [MikroE](https://github.com/MikroElektronika)
                  , [ORYX EMBEDDED](https://github.com/Oryx-Embedded/CycloneTCP)
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

## Hardware Support

- Connect Ethernet cable to ETH connector on the right side of the board.
- Connect USB-C cable to USB-UART connector on the left side of the board.

![image](https://download.mikroe.com/images/click_for_ide/board-uni-ds-v8-eth.png)

## Software Support

We provide demo code for testing HTTP client functionality.
For example to work, proper clock setup is needed. To create a setup with the adequate clock scheme, follow these steps:

1. Click on Setups and select New.
2. Select GCC ARM or RISC-V compiler and click on Advanced.
3. From Redirect standard output to: drop down menu, select UART, and then click Save.
4. Select Board you are using.
5. Select your MCU and click on Advanced.
6. Select Config scheme from drop down menu, it will have MCU_NAME_ETHERNET name.
7. If you dont see MCU_NAME_ETHERNET scheme for your MCU then ETHERNET module is not supported.
8. Click Save, then Next.
9. Select desired display, and choose programmer/debugger tool.
10. Click Finish.
11. Program the MCU.
12. In case of a setup with UART open Tools view (Ctrl + 7) and select UART Terminal. In case of setup with Application output, run debug and go straight to step 17
13. Click on Options and change Port to the one you are using.
14. Furthemore, change Baud rate to 115200 bps.
15. Press Configure
16. Click on Connect.
17. When prompted, press the appropriate button, and on UART/Application output you can observe the process of connecting to HTTP server, sedning POST request to, and receiving the response from the HTTP server.

![example](https://download.mikroe.com/images/click_for_ide/demo-eth-http-client-1.png)

![example](https://download.mikroe.com/images/click_for_ide/demo-eth-http-client-2.png)

---
