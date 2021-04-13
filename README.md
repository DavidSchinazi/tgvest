TG Vest
================

TG Vest is the code currently running on TechnoGecko vests.
It is mostly based on DFSparks, its predecessor for the DiscoFish.
The target platform is ESP8266.

You'll want to flash this file onto your device:
tgvest/examples/Vest/Vest.ino

Using with Arduino IDE
----------------------------

To install Arduino and dfsparks code on Ubuntu:

    Download latest software from https://www.arduino.cc/en/Main/Softwarea
      https://www.arduino.cc/download_handler.php?f=/arduino-1.6.10-linux64.tar.xz
    cd ~
    mv Downloads/arduino-1.6.10-linux64.tar.xz ./
    tar -xf arduino-1.6.10-linux64.tar.xz
    mkdir -p ~/Arduino/libraries
    cd ~/Arduino/libraries
    git clone https://github.com/FastLED/FastLED
    ~/arduino-1.6.10/arduino &
    Open File/Preferences, and add to Additional Boards:
        http://arduino.esp8266.com/stable/package_esp8266com_index.json
    Open Tools/Boards/BoardsManager, search for esp8266, install
    Select Tools/Boards/NodeMCU1.0
    Add yourself to dialout group:
        sudo usermod -a -G dialout igor


To compile:

    ~/arduino-1.6.10/arduino &
    Click menu option: File/Examples/DFSparks/Vest
    Click on the checkmark button to verify
    Select Tools/Ports/ttyUSB0
    Click the Upload button

You can use extras/test-server.py to test how your device
synchronizes with the server.


Adding new effects
----------------------------

To implement new effect:

	1. Create a file that contains effect source, e.g. src/dfsparks/effects/coolstuff.h
	(you may keep it header-only or add a .cpp file as well)
	2. Use one of the existing files as an example. At a minimum, you need to extend
		the Effect class and implement 'doRender' method.
	3. doRender gets Pixels object representing a viewport mapped to device pixels.
		You can get viewport width and height from the pixels object. You can also find
		out viewport coordinates of each pixel by calling 'coords' method of the Pixels
		object. timeElapsed() method on the effect itself lets you query the time.
		All times are in milliseconds, stored as int32_t.
	4. Create the effect in src/dfsparks/playlist.cpp, see Repertoire constructor.
	5. Add new effect sources to CMakeLists.txt


Flashing updated firmware
-------------------------

To flash new firmware onto NodeMCU:

    Open https://github.com/nodemcu/nodemcu-firmware/releases
        download latest firmware
    Otherwise, you can build your own newer firmware:
        go to http://nodemcu-build.com/
        enter email address, leave all default, start build
        download using link from email, e.g.:
            wget http://nodemcu-build.com/builds/nodemcu-master-7-modules-2016-08-14-10-43-06-float.bin
    Connect the device using USB cable
        try different cable if first one does not work
    cd ~
    git clone https://github.com/themadinventor/esptool
    sudo python ./esptool/esptool.py --port /dev/ttyUSB0  write_flash 0x00000 \
        nodemcu-master-7-modules-2016-08-14-10-43-06-float.bin


Uploading sketches on macOS
-------------------------

(this is generic stuff not specific to DFSparks library)

To upload sketches to ESP8266 and ESP32 board from Arduino IDE on macOS:

1. Install ESP8266 and ESP32 boards
   - Go to Arduino->Preferences->Settings; Additional board manager URLs
   - Add https://arduino.esp8266.com/stable/package_esp8266com_index.json
   - Add https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   - Go to Tools->Board->Board Manager
   - Search for ESP8266, select latest version (tested with 2.7.4), install
   - Search for ESP32, select latest version (tested with 1.0.6), install
   - (Note that this was last tested with Arduino 1.8.13 and FastLED 3.4.0)

1. For ESP8266, install USB to UART drivers. Required drivers seem to be different for different
   board flavours.
   - On MacOS Mojave, this worked:
		https://drive.google.com/file/d/1lUYkw-mCOekJjY1MVA5YuEbERHNWNCZW/view
   - On macOS Sierra, for LoLin boards this worked:
		http://www.codenuke.net/2015/01/nodemcu-install-ch340-usb-to-serial-for-yosemite.html
   - On macOS Sierra, for noname board I got from Amazon this worked:
		https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx (/dev/cu.SLAB_USBtoUART)
   - On macOS Big Sur, the drivers above seemed to still work.
   - On macOS Big Sur, ESP32 seems to be supported out of the box.
1. Select board and port
   - For ESP8266, use "NodeMCU 1.0 (ESP-12E)" board and port corresponding to USB driver
   - For ESP32, use "ESP32 Pico Kit" board and port corresponding to USB driver
1. Set serial monitor to 115200 baud, 80MHz, 4M (3M SPIFFS).
