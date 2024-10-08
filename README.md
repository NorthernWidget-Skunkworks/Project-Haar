[![DOI](https://zenodo.org/badge/177828982.svg)](https://zenodo.org/badge/latestdoi/177828982)

# Project-Haar

***Mechanical, electrical, and firmware files for a rugged and submersible temperature, pressure, and relative-humidity sensor***

## Namesake and history

![Haar in Ben Bhraggie Woods, Sutherland](https://s2.geograph.org.uk/geophotos/05/74/72/5747242_d2ec0f07_1024x1024.jpg)
***Haar in Ben Bhraggie Woods, Sutherland, Scotland.*** *Photo by Andrew Tryon.*

Project Haar takes its codename from the [North Sea fogs that roll across Britain](https://en.wikipedia.org/wiki/Haar_(fog)).
[Bobby Schulz](https://github.com/bschulz1701) designed it after the consistently-near-0C temperatures and 100% relative humidity of [Chimborazo Volcano](https://en.wikipedia.org/wiki/Chimborazo), Ecuador, claimed the lives of many brave but misguided [BME-280](https://www.adafruit.com/product/2652) units, who then catastrophically bricked their I2C buses and those of their associated data loggers, thus taking down a large fraction of our hydromet network.

Determined to avoid future issues with persistently condensing humidity, we worked to design a unit that would survive in this environment, and indeed, withstand full submersion.

## Technical specifications
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="Documentation/images/Haar_v010_top_annotated_20200427_LIGHT.png">
  <!-- <source media="(prefers-color-scheme: light)" srcset="Documentation/images/Haar_v010_top_annotated_20200427.png"> -->
  <img alt="Haar bare board" src="Documentation/images/Haar_v010_top_annotated_20200427.png">
</picture>
<!-- ![Haar bare board](Documentation/images/Haar_v010_top_annotated_20200427.png) -->

### Electronic Hardware

* Microcontroller (computer) core
  * ATTiny1634
  * [Firmware](Firmware/) written in Arduino-compatible C++
  * Firmware upload via a [0.05" (1.27 mm) pitch 2x3 ICSP header](https://www.digikey.com/product-detail/en/samtec-inc/FTS-103-01-L-D/SAM1156-03-ND/1109979)
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
  * [4-pin "HardMount" plug](https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/1838893-2/A97650-ND/1764165) ([3D drawing](http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=1838893-2&DocType=Customer+View+Model&DocLang=English))
  * I2C
* Power
  * Voltage limits: 3.3V ~ 5.5V
  * Power consumption: 
    * Peak = 5.21mA @ 3.3V
    * Sleep = 0.524 mA @ 3.3V
    * Mean (5 Hz sample rate) = 0.583 mA @ 3.3V
    * Mean (1 Hz sample rate) = 0.780 mA @ 3.3V
* Submersible with its Gore-Tex sealed housing
* Entirely IP67 or greater rated components
* Response Time
  * Temperature: &tau; = 377s
  * Humidity: &tau; = 505s
  * Pressure: &tau; &lt;1s

### Electronic Software and Firmware

* Programmable using the Arduino IDE https://www.arduino.cc/en/main/software
* [Firmware](Firmware/) available in this repository. [`Haar_Firmware.ino`](Firmware/Haar_Firmware/Haar_Firmware.ino) is the standard firmware to be used. 
* [Software](https://github.com/NorthernWidget-Skunkworks/Haar_Library) to use this sensor with Arduino-compatible devices
* Open-source licensing via GNU GPL 3.0

### Weatherproofing and enclosure
* Low-VOC conformal coating as recommended on Page 3 of the [humidity and temperature sensor data sheet](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Application_Note/Sensirion_Humidity_Sensors_Handling_Instructions.pdf). We used [Dow Corning 1-2577 Low VOC Conformal Coating](https://www.digikey.com/product-detail/en/1-2577+CONFORMAL+CTG+453G/2275-1-2577CONFORMALCTG453G-ND/11494980/?itemSeq=337863596).
* Fully sealed housing with removable [breathable-membrane (IP68) vent](https://www.digikey.com/product-detail/en/amphenol-ltw/VENT-PS1NGY-O8002/1754-1221-ND/7898276)

> [!WARNING]
> If applying conformal coating, ensure none comes in contact with the sensor orifices themselves, see Senserion Handling Instructions [example](Documentation/images/ConformalApplication.png)

![Haar: in housing](Documentation/images/Haar_v010_in_housing_noRuler_20200228.png)

## Assembly

### Sensor circuit board

[Bill of Materials](Production/Haar_HardMount_BoM_0v1.csv)

[PCB Gerber Files](Production/Gerbers_2020-02-20.zip)

Assembling this sensor is possible by hand with sufficient skill and the following tools:
* Temperature-controlled soldering iron
* Hot-air rework station
* Equipment for stenciling with solder paste
* ESD-safe tweezers and workstation
* Solder wick

Many components on this board are fine pitch and/or have difficult to access pads. However, there are not very many components. We would suggest hand assembly using tweezers, stenciled solder paste, and a hot-air rework station. A soldering iron will be helpful -- if not outright essential -- for attaching the "Hard Mount" plug and (if desired) the 2x3 pin ICSP header to upload the firmware.
<!-- >> THOUGHT: I have basic instructions for assembly on the [Project Walrus](https://github.com/NorthernWidget-Skunkworks/Project-TPDH-Walrus) README; I will not reproduce these here just yet, as we might want to set up a separate assembly guide, mostly with good links to existing resources, since our writing something of this nature would be to reinvent the wheel. -->

After finishing the main board assembly, solder the HardMount plug onto the four SMD pads with the notch (for plug orientation) at the bottom side of the board.

### Housing and cabling

![Haar: exploded view](Documentation/images/Haar_v010_and_housing_exploded_20200228.png)

Two different housings have been designed to be used in different situations depending on how much robustness is required by the sensor. 
**Closed:**
For maximal robustness a [sealed IP67 rated housing](Mechanical/ClosedBody/ClosedBody.stl) was designed. This housing can be constructed using FDM printing with ABS plastic and acetone smoothing - an [FDM optimized version](Mechanical/ClosedBody_FDM/ClosedBody_FDM.stl) of the design was made featuring an eyelet to hang the enclosure from during the smoothing process. However, results will be significantly better with PA12 SLS nylon - readily available from many print houses.

![Render of Closed Enclosure](Documentation/images/ClosedEnclosureRender.png)

**Open:**
For maximal responsiveness an [open structure enclosure](Mechanical/OpenBody/OpenBody.stl) was designed to allow airflow while still protecting the sensor from mechanical damage. This enclosure has been tested with FDM and SLA printing, methods will depend on your desired material properties.

![Render of Closed Enclosure](Documentation/images/OpenEnclosureRender.png)


#### Parts required

* Sealing plug with cable such as this [-40 degree PVC-jacketed one](https://www.digikey.com/product-detail/en/alpha-wire/AR0400105-SL357/AR0400105SL357-ND/6555497) or [this set of Digi-Key search results for all those that are 3 meters or less in both -40 degree rated PVC and in ](https://www.digikey.com/products/en/cable-assemblies/circular-cable-assemblies/448?k=&pkeyword=&sv=0&pv2331=312745&sf=0&FV=-1%7C216%2C2344%7C346572%2C2346%7C369969%2C2352%7C387800%2C2353%7C421432%2C2380%7C203167%2C-8%7C448%2C77%7C172800%2C77%7C298043%2C77%7C62398&quantity=&ColumnSort=0&page=1&pageSize=25); these come in [a variety of lengths from 0.6 to 20 meters](http://www.alphawire.com/en/Products/Connectivity/AlphaConnect/Cordsets/AR0400105) ([Digi-Key search results](https://www.digikey.com/products/en/cable-assemblies/circular-cable-assemblies/448?FV=2331%7C312745%2C2344%7C346572%2C2350%7C349292%2C2380%7C203167%2C-8%7C448%2C2345%7C1%2C2352%7C387800&quantity=0&ColumnSort=77&page=1&stock=1&rohs=1&nstock=1&k=M12+Alpha+Wire&pageSize=25&pkeyword=M12+Alpha+Wire)). You may also [attach a plug to your own cable](http://www.alphawire.com/en/Products/Connectivity/AlphaConnect/FieldAttachable/200ARS7). Note that I2C communications become iffy for any cable longer than 3 meters.
* [3D-printed housing barrel](Mechanical/ClosedBody_FDM), tapped (see below). We recommend light-colored plastic to reduce radiative heating.
* [4-pin HardMount plug](https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/1838893-2/A97650-ND/1764165)
* [Amphenol breathable vent](https://www.digikey.com/product-detail/en/amphenol-ltw/VENT-PS1NGY-O8002/1754-1221-ND/7898276)

#### Soldering the HardMount attachment to your board

If it is not already attached, you will need to solder the [4-pin HardMount plug](https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/1838893-2/A97650-ND/1764165) onto the four SMD pads at the end of the board far from the isolated humidity sensor. You should do so with the notch (for plug orientation) at the bottom side of the board.

Our recommendation for by-hand assembly:
* Add small pillows of solder to each of the four pads, enough to create contact with the HardMount solder cups but not so much that it is not possible to slide the HardMount plug over them.
* Slide the HardMount plug on; it will take a bit of finesse to keep it from being pushed off of the solder pillows.
* Using some Blu Tack (or equivalent), fix the board and HardMount plug in place
* Double-check that the notch in the HardMount plug is on the back side of the board
* Solder on the first connection
* Check whether the board is straight; reflow the solder as needed until the connection is secure
* Solder the second connection
* Flip the board, ensuring that it is straight (if not, make changes)
* Solder the other two solder-cup connections

> [!TIP]
> Sometimes the 3D printed threads are not sufficiently fine to work smoothly. In this case, you can 'repair' the threads using a metal tap. 
>
> The cable side thread is an [M16 x 1.5](https://www.mcmaster.com/26015A263/), and the vent side is a [M12 x 1.5](https://www.mcmaster.com/26015A613/) - More economical taps can be found, these placed here for referance.



#### Cable

Here we assume that you are using [standard Alpha Wire cables with waterproof attachments]([Alpha Wire 5004C](https://www.digikey.com/product-detail/en/alpha-wire/5004C-SL001/5004CSL001-ND/484976). Because of the wire insulation colors available for these cables and the locations of their attachment points to the [4-pin "HardMount" plug](https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/1838893-2/A97650-ND/1764165) ([3D drawing](http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=1838893-2&DocType=Customer+View+Model&DocLang=English)), **our HardMount devices do NOT match standard wire-color definitions (or any kind of standard)**. Wire-color definitions are:

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="Documentation/images/Pinout_LIGHT.png">
  <img alt="Haar Connector Pinout" src="Documentation/images/Pinout.png">
</picture>

| **Color** | **Connection** |
|-----------|----------------|
| Brown     | V+ (3.3-5.5 V) |
| White     | GND            |
| Black     | SDA            |
| Blue      | SCL            |

For reference, these are the definitions of the four pads to which the [HardMount connector](https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/1838893-2/A97650-ND/1764165) attaches:
* Top left: SDA
* Top right: SCL
* Bottom left: VCC (3.3V to 5.5V)
* Bottom right: GND


## Firmware

### Downloading and installing the Arduino IDE

Go to https://www.arduino.cc/en/main/software. Choose the proper IDE version for your computer. For Windows, we suggest the non-app version to have more control over Arduino; this might change in the future. You will have to add custom libraries, so the web version will not work (at least, as of the time of writing). Download and install the Arduino IDE. Open it to begin the next steps.

### AVR ISP

To install firmware on the Haar board, you use the [2x3-pin 6-pin ICSP (also called ISP) header](https://www.digikey.com/product-detail/en/samtec-inc/FTS-103-01-L-D/SAM1156-03-ND/1109979) with a special device called an "in-circuit system programmer" (or just "in-system programmer; yup, that's what the acronym stands for).

Many devices exist to upload firmware, including:
* The official [AVR ISP mkII](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42093-AVR-ISP-mkII_UserGuide.pdf) (no longer produced but available used)
* Using an [Arduino as an ISP](https://www.arduino.cc/en/tutorial/arduinoISP)
* The versatile [Olimex AVR-ISP-MK2](https://www.olimex.com/Products/AVR/Programmers/AVR-ISP-MK2/open-source-hardware) (recommended)
* The [Adafruit USBtinyISP](https://www.adafruit.com/product/46)

> [!IMPORTANT] 
> The Haar has a [half-sized (0.05" pitch) ICSP header](https://www.digikey.com/product-detail/en/samtec-inc/FTS-103-01-L-D/SAM1156-03-ND/1109979). Therefore, you will need to have the appropriate 2x3-pin header to form a connection - see detailed [assmbly instructions](Hardware/ISP_Cable_Assembly.md).

<!-- ***Note: Be sure to download and/or update drivers for your ISP.*** -->

> [!NOTE]
> Frequently on Windows machines the driver configuration of the ISP programmer can become problematic. See discussion of how to get drivers and the use of Zadig to configure them in this [Sparkfun Guide](https://learn.sparkfun.com/tutorials/pocket-avr-programmer-hookup-guide/installing-drivers)

### Uploading the firmware

Using this ISP, upload (as of the time of writing): [the Arduino sketch in this folder](Firmware/Haar_Firmware/). To do so, follow these steps:

1. Open the Arduino IDE.
2. Follow [these instructions](https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md) to install the [ATTinyCore board definitions](https://github.com/SpenceKonde/ATTinyCore)
3. Select [ATTiny1634](https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/extras/ATtiny_1634.md) **(No bootloader)**

> [!CAUTION]
> Check your ISP target voltage before you connect! Make sure the target voltage is set to 3.3v, 5v can damage the sensor irreparably 

4. Plug your ISP of choice into your computer (via a USB cable) and onto the 6-pin header. There are two ways to place it on; the header is aligned such that the ribbon cable should be facing away from the board while programming. If this fails without being able to upload, try flipping the header around. This should both power the board and provide communications.
5. Go to Tools --> Programmer and select the appropriate programmer based on what you are using.
6. Go to Tools --> Burn Bootloader. Yes, we know that you just selected "ATTiny1634 (No bootloader)", but this step sets the fuses, which configure their internal oscillator and brown-out detection.
7. Go to Sketch --> Upload Using Programmer. After several seconds, you learn whether you succeeded or failed. Hopefully it worked!




![Upload using programmer](https://media.githubusercontent.com/media/NorthernWidget-Skunkworks/Project-Symbiont-LiDAR/master/Documentation/images/UploadUsingProgrammer.png)

***Uploading using the in-system programmer.***

> [!NOTE] 
> **Linux Users:** You must supply permissions to the Arduino IDE for it to be able to use the ICSP, or you will have to run it using `sudo`. The former option is better; the latter is easier in the moment.


## Writing a program to connect to the Haar sensor

Once the  is bootloaded and connected with a LiDAR Lite sensor, you should be able to use any standard Arduino device to connect to it and read its data.

### Very simple Arduino code

This code is intended for any generic Arduino system.

```c++
// Include the Haar library
#include "Haar.h"

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
// Include the libraries
#include "Margay.h"
#include "Haar.h"

// Declare variables -- just as strings
String header;
String data;

// Instantiate classes
Haar myTPRH;
Margay Logger; // Margay v2.2; UPDATE CODE TO INDICATE THIS

// Empty header to start; will include sensor labels and information
String Header = "";

// I2CVals for Haar
uint8_t I2CVals[] = {0x42}; // DEFAULT

//Number of seconds between readings
uint32_t updateRate = 60;

void setup(){
    Header = Header + myTPRH.getHeader();
    Logger.begin(I2CVals, sizeof(I2CVals), Header);
    initialize();
}

void loop(){
    initialize();
    Logger.run(update, updateRate);
}

String update() {
    initialize();
    return myTPRH.getString();
}

void initialize(){
    myTPRH.begin();
}
```

### Northern Widget Okapi code

>> Currently nearly identical to Margay code, by design. Add telemetry with Particle Boron.

```c++
// Include the libraries
#include "Okapi.h"
#include "Haar.h"

// Declare variables -- just as strings
String header;
String data;

// Instantiate classes
Haar myTPRH;
Okapi Logger;

// Empty header to start; will include sensor labels and information
String Header = "";

// I2CVals for Haar
uint8_t I2CVals[] = {0x42}; // DEFAULT

//Number of seconds between readings
uint32_t updateRate = 60;

void setup(){
    Header = Header + myTPRH.getHeader();
    Logger.begin(I2CVals, sizeof(I2CVals), Header);
    initialize();
}

void loop(){
    initialize();
    Logger.run(update, updateRate);
}

String update() {
    initialize();
    return myTPRH.getString();
}

void initialize(){
    myTPRH.begin();
}
```

## Characterization

### Response time


<!-- ![Lab fridge test](Documentation/images/Haar_T9602_vent_test_lab_fridge.png)

***Response-time testing.*** *Our initial tests demonstrate that, with the vent detached, the Haar exhibits sensitivity and response time similar to the [T9602 temperature and relative humidity sensor](https://www.amphenol-sensors.com/images/stories/moisture-humidity/main-T9602-Mod-4.png). With the vent cap on, this response is dampened significantly.* -->
This sensor was tested for using an impulse for each variable (temperature, pressure, humidity) and for each housing type (closed, open). The details of each of these evaluations can be found in the testing documentation: [Temperature](Testing/Temperature/README.md), [Pressure](Testing/Pressure/README.md), [Humidity](Testing/Humidity/README.md). A summary of these results, along with visuals of the step response, is presented here.

Approximate time to equilibrium for each variable and sensor type
| **Sensor** | Temperature | Pressure | Humidity | 
|---:|:---:|:---:|:---:|
| Control | 105s | &lt;1s | 18 min |
| Haar, Closed | 75s | &lt;1s | 31.75 min |
| Haar, Open | 12.5 min | &lt;1s | 10.25 min |

![Temperature Step Response Graph](Testing/Temperature/TempResponse.png)

![Humidity Step Response Graph](Testing/Humidity/HumidityResponse.png)


This response time information should serve as a guide to the choice of housing. In situations where very fast response time is required *and* the sensor will not be subjected to environmental extremes (e.g. in a radiation shield on a meteorological station) an open housing can be used. However, in cases where the hazards of the environment are a priority (e.g. a sensor in the flood plane of a river to calibrate a depth sensor) a closed housing should be used.

<!-- >> @bschulz1701: Application note about when to use the cap? Notes about the IP ratings of the devices and practical ratings as well? And I've been looking for a M12 vent without any membrane so it can still physically protect the sensor (or act as a roof for rain) but have not yet found one. Thoughts on this?
>> @awickert: The cap would desire to be used when there is the potential for the device to become fully submerged and slow response time is not a problem (for getting system averages instead of response time or quick variance). All elements of the device (uncapped) are rated to >IP67, however, the conformal coating of the board is not specifically rated for any pressure. It would likely survive breif immersion but not for extended periods of time. As for the a cap with no membrane: since be 3D print the body, we would just design a body with a "roof" instead of a thread at the top, and holes or slots in the side, whichever we desire. Then instead of replacing the end cap, you just replace the entire body (which just screws on to the bottom). This would actually make it a bit easier to make, since it would be one less thread that would need to be made by hand. -->



## Acknowledgments

Support for this project provided by:

<img src="Documentation/logos/UMN_ESCI.png" alt="UMN ESCI" width="240px">
<br/>
<br/>
<img src="Documentation/logos/NSF.png" alt="NSF" width="240px">

<img src="Documentation/logos/WMO.png" alt="WMO" width="240px">

<br/>
<br/>
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
