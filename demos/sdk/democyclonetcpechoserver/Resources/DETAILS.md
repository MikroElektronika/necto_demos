# CycloneTCP Echo Server DEMO

This example demonstrates echo server functionality. Observe UART/Application output and wait until server address is resolved, singnaling that echo server running on the MCU is ready to be used. Connect to the server with Tera Term or any other terminal of your choice. Set Host: to either "echo-server-demo" or the IP address from UART/Application output, and use Port: "7". Write something into the terminal, and observe it being echoed back by the echo server.

- **Authors**     : MikroE https://github.com/MikroElektronika
                  , ORYX EMBEDDED https://github.com/Oryx-Embedded/CycloneTCP
- **Version**     : 3.0.0
- **Date**        : ${COPYRIGHT_YEAR}.

## Hardware Support

- Connect Ethernet cable to ETH connector on the right side of the board.
- Connect USB-C cable to USB-UART connector on the left side of the board.

![image](https://download.mikroe.com/images/click_for_ide/board-uni-ds-v8-eth.png)

## Software Support

We provide demo code for testing echo server functionality.
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
12. In case of a setup with UART open Tools view (Ctrl + 7) and select UART Terminal. In case of setup with Application output, run debug and go straight to step 18.
13. Click on Options and change Port to the one you are using.
14. Furthemore, change Baud rate to 115200 bps.
15. Press Configure.
16. Click on Connect.
17. Observe UART/Application output, and wait until host address is resovolved, signaling that Echo server is ready to be used.
18. Open Tera Term or other terminal of you choice.
19. Select TCP/IP.
20. Set Host to "echo-server-demo", without quotes, or the IP address given in step 17.
21. For Service: select Telnet.
22. Set TCP port# to 7.
23. Click "OK".
24. Write "Hello World!" into the terminal and press Enter. Observe that the echo server running on the MCU will echo "Hello World!" back.

![example](https://download.mikroe.com/images/click_for_ide/demo-eth-echo-server-3.png)

![example](https://download.mikroe.com/images/click_for_ide/demo-eth-echo-server-1.png)

![example](https://download.mikroe.com/images/click_for_ide/demo-eth-echo-server-2.png)

---
