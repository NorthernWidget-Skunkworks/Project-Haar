# Project-Haar

***Mechanical, electrical, and firmware files for a rugged and submersible temperature, pressure, and relative-humidity sensor***

## Namesake and history

![Haar in Ben Bhraggie Woods, Sutherland](https://s2.geograph.org.uk/geophotos/05/74/72/5747242_d2ec0f07_1024x1024.jpg)
***Haar in Ben Bhraggie Woods, Sutherland, Scotland.*** *Photo by Andrew Tryon.*

Project Haar takes its codename from the [North Sea fogs that roll across Britain](https://en.wikipedia.org/wiki/Haar_(fog)).
[Bobby Schulz](https://github.com/bschulz1701) designed it after the consistently-near-0C temperatures and 100% relative humidity of [Chimborazo Volcano](https://en.wikipedia.org/wiki/Chimborazo), Ecuador, claimed the lives of many brave but misguided [BME-280](https://www.adafruit.com/product/2652) units, who then catistrophically bricked their I2C buses and those of their associated data loggers, thus taking down a large fraction of our hydromet network.

Determined to avoid future issues with persistently condensing humidity, we worked to design a unit that would survive in this environment, and indeed, withstand full submersion.

## Technical specifications
![Haar bare board](Documentation/images/Haar_v010_top_annotated_20200427.png)

### Electronic Hardware

* Microcontroller (computer) core
  * ATTiny1634
  * [Firmware](Firmware) written in Arduino-compatible C++
  * Firmware upload via a 0.05" (1.27 mm) pitch 2x3 ICSP header
* Sensors
  * Relative humidity and temperature sensor: [SHT35-DIS-F2.5KS](https://www.digikey.com/product-detail/en/sensirion-ag/SHT35-DIS-F2.5KS/1649-1067-1-ND/6212138) with [PTFE membrane](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Datasheets/Sensirion_Humidity_Sensors_SHT3x_Datasheet_Filter_Membrane.pdf) for water and dust protection
    * Humidity
      * Range: 0--100%
      * Accuracy: ±1.5%
      * Resolution: 0.01%
      * Long-term drift: <0.25%/yr
    * Temperature
      * Range: -40--125 °C
      * Accuracy: ±0.2 °C
      * Resolution: 0.01 °C
      * Long-term drift: <0.01 °C/yr
  * Atmospheric pressure and temperature sensor: [LPS35HW](https://www.st.com/en/mems-and-sensors/lps35hw.html)
    * Pressure
      * Range: 260 to 1260 mbar
      * Accuracy, calibrated: ±1 mbar
      * Accuracy, uncalibrated: ±4 mbar
      * Resolution: 1/4096 mbar (approx. 0.00024 mbar)
    * Temperature
      * Range, full accuracy: 0 to 60 °C
      * Accuracy: ±1.5 °C
      * Resolution: 0.01 °C
* Connections and communications protocols
  * [4-pin hard mount plug](https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/1838893-2/A97650-ND/1764165)
  * I2C
* Power
  * Voltage limits: **@bschulz1701?**
  * Power consumption: **@bschulz1701?**
* Indefinitely submersible with its Gore-Tex sealed housing
* Entirely IP67 or greater rated components

>> Response time @bschulz1701? Let's look at your tests.

### Electronic Software and Firmware

* Programmable using the Arduino IDE https://www.arduino.cc/en/main/software
* [Firmware](Haar_Firmware_Test) available in this repository. At the time of writing, "Haar_Firmware_Test" is the most up-to-date prototype firmware.
* [Software ](https://github.com/NorthernWidget-Skunkworks/Haar_Library) to use this sensor with Arduino-compatible devices
* Open-source licensing via GNU GPL 3.0

### Weatherproofing and enclosure
* Low-VOC conformal coating
* Fully sealed housing with removable Gore-Tex vent

![Haar: in housing](Documentation/images/Haar_v010_in_housing_20200228.png)

## Assembly

Assembling this sensor is possible by hand with sufficient skill and the following tools:
* Temperature-controlled soldering iron
* Hot-air rework station
* Equipment for stenciling with solder paste
* ESD-safe tweezers and workstation
* Solder wick

Many components on this board are fine pitch and/or have difficult to access pads. However, there are not very many components. We would suggest hand assembly using tweezers, stenciled solder paste, and a hot-air rework station. A soldering iron will be helpful -- if not outright essential -- for attaching the "Hard Mount" plug and (if desired) the 2x3 pin ICSP header to upload the firmware.
>> THOUGHT: I have basic instructions for assembly on the [Project Walrus](https://github.com/NorthernWidget-Skunkworks/Project-TPDH-Walrus) README; I will not reproduce these here just yet, as we might want to set up a separate assembly guide, mostly with good links to existing resources, since our writing something of this nature would be to reinvent the wheel.


## Uploading the firmware

### Downloading and installing the Arduino IDE

Go to https://www.arduino.cc/en/main/software. Choose the proper IDE version for your computer. For Windows, we suggest the non-app version to have more control over Arduino; this might change in the future. You will have to add custom libraries, so the web version will not work (at least, as of the time of writing). Download and install the Arduino IDE. Open it to begin the next steps.

### AVR ISP

To install firmware on the Haar board, you use the 2x3-pin 6-pin ICSP (also called ISP) header with a special device called an "in-circuit system programmer" (or just "in-system programmer; yup, that's what the acronym stands for).

Many devices exist to upload a bootloader including:
* The official [AVR ISP mkII](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42093-AVR-ISP-mkII_UserGuide.pdf) (no longer produced but available used)
* Using an [Arduino as an ISP](https://www.arduino.cc/en/tutorial/arduinoISP)
* The versatile [Olimex AVR-ISP-MK2](https://www.olimex.com/Products/AVR/Programmers/AVR-ISP-MK2/open-source-hardware)
* The [Adafruit USBtinyISP](https://www.adafruit.com/product/46)

**IMPORTANT:** The Haar has a half-sized (0.05" pitch) ICSP header. Therefore, you will need to have the appropriate 2x3-pin header to form a connection.

### Uploading the firmware

Using this ISP, upload (as of the time of writing): [the Arduino sketch in this folder](Firmware/LiDAR_InterfaceDemo/). To do so, follow these steps:

1. Open the Arduino IDE.
2. Follow [these instructions](https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md) to install the [ATTinyCore board definitions](https://github.com/SpenceKonde/ATTinyCore)
3. Select [ATTiny1634](https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/extras/ATtiny_1634.md) without bootloader
4. Plug your ISP of choice into your computer (via a USB cable) and onto the 6-pin header. There are two ways to place it on; the header is aligned such that the ribbon cable should be facing away from the board while programming. If this fails without being able to upload, try flipping the header around. This should both power the board and provide communications.
5. Go to Tools --> Programmer and select the appropriate programmer based on what you are using.
6. Go to Sketch --> Upload Using Programmer. After several seconds, you learn whether you succeeded or failed. Hopefully it worked!

![Upload using programmer](Documentation/images/UploadUsingProgrammer.png)

***Uploading using the in-system programmer.***

***Important note for Linux users:*** You must supply permissions to the Arduino IDE for it to be able to use the ICSP, or you will have to run it using `sudo`. The former option is better; the latter is easier in the moment.


## Writing a program to connect to the Haar sensor

Once the  is bootloaded and connected with a LiDAR Lite sensor, you should be able to use any standard Arduino device to connect to it and read its data.

### Very simple Arduino code

This code is intended for any generic Arduino system.

```c++
// Include the Haar library
#include "Haar"

// Declare variables -- just as strings
String header;
String data;

// Instantiate class
Haar myTPRH;

void setup(){
    // Begin Serial connection to computer at 38400 baud
    Serial.begin(38400);
    // Obtain the header just once
    header = myTPRH.getHeader();
    // Print the header to the serial monitor
    Serial.println(header);
}

void loop(){
    // Take one reading every (10 + time to take reading) seconds
    // and print it to the screen
    myTPRH.updateMeasurements();
    data = myTPRH.getString();
    Serial.println(data);
    delay(10000); // Wait 10 seconds before the next reading, inefficiently
}
```

### Northern Widget Margay code

The [Margay data logger](github.com/NorthernWidget-Skunkworks/Project-Margay) is the lightweight and low-power open-source data-logging option from Northern Widget. It saves data to a local SD card and includes on-board status measurements and a low-drift real-time clock. We have written [a library to interface with the Margay](github.com/NorthernWidget-Skunkworks/Margay_Library), which can in turn be used to link the Margay with sensors.

```c++
// Include the Symbiont library
#include "Margay.h"
#include "Haar.h"

// Declare variables -- just as strings
String header;
String data;

// Instantiate classes
Haar myTPRH;
Margay Logger(Model_2v0, Build_B); // Margay v2.2; UPDATE CODE TO INDICATE THIS

// Empty header to start; will include sensor labels and information
String Header = "";

// I2CVals for Symbiont
uint8_t I2CVals[] = {0x40}; // DEFAULT BUT CLASHES WITH Symbiont-LiDAR

//Number of seconds between readings
uint32_t updateRate = 60;

void setup(){
    Header = Header + myTPRH.GetHeader();
    Logger.begin(I2CVals, sizeof(I2CVals), Header);
    init();
}

void loop(){
    init();
    Logger.Run(update, updateRate);
}

String update() {
    init()
    return myTPRH.GetString();
}

void init(){
    myTPRH.begin();
}
```

### Northern Widget Resnik code

>> Currently nearly identical to Margay code, by design. Add telemetry with Particle Boron.

```c++
// Include the Symbiont library
#include "Resnik.h"
#include "Haar.h"

// Declare variables -- just as strings
String header;
String data;

// Instantiate classes
Haar myTPRH;
Resnik Logger();

// Empty header to start; will include sensor labels and information
String Header = "";

// I2CVals for Symbiont
uint8_t I2CVals[] = {0x40}; // DEFAULT BUT CLASHES WITH Symbiont-LiDAR

//Number of seconds between readings
uint32_t updateRate = 60;

void setup(){
    Header = Header + myTPRH.GetHeader();
    Logger.begin(I2CVals, sizeof(I2CVals), Header);
    init();
}

void loop(){
    init();
    Logger.Run(update, updateRate);
}

String update() {
    init()
    return myTPRH.GetString();
}

void init(){
    myTPRH.begin();
}
```

## Housing and cabling

![Haar: exploded view](Documentation/images/Haar_v010_and_housing_exploded_20200228.png)

### Parts required

* [Sealing plug with cable](https://www.digikey.com/product-detail/en/alpha-wire/AR0400105-SL357/AR0400105SL357-ND/6555497)
* [3D-printed housing barrel](3Dprint), tapped (see below). We recommmend light-colored plastic to reduce radiative heating.
* [4-pin hard mount plug](https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/1838893-2/A97650-ND/1764165)
* [Amphenol breathable vent](https://www.digikey.com/product-detail/en/amphenol-ltw/VENT-PS1NGY-O8002/1754-1221-ND/7898276)

### Tapping the housing

3D-printed housing barrel:
* Cable side: M16 x 1.5
* Vent side: M12 x 1.5

>> @bschulz1701: what pinout colors


<br>
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
