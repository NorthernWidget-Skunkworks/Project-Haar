# Testing - Temperature

A test was conducted using a custom built temperature controlled box in order to evaluate the variance in response with an open housing verses a housing with a GoreTex vent  

### Procedure 
A pair of Haar sensors (`v0.1` hardware) were placed in open air and allowed to fully acclimate to stable room temperature (minimum of 60 minutes at a temperature of approximately 16°C). Once this is completed, the sensors are placed into a thermal regulated box held (construction discussed [here](#thermal-regulated-box-construction)) at a temperature of 25°C by a heater and [PID controller](../BoxPID/BoxPID.ino). Data from the sensors is recorded until after equilibrium is achieved. This procedure subjects the sensors to step response in order to determine the thermal characteristics. 

Haar data was logged to a computer using the generic Haar [DataRecorder](../DataRecorder/DataRecorder.ino) script with `period` set to 1000ms. In addition, ground truth data was taken using a [HygroVue 10](https://www.campbellsci.com/hygrovue10) sensor. An [SDI-12 Symbiont](https://github.com/RTGS-Lab/Symbiont-SDI12) was used to interface the SDI-12 sensor with the microcontroller board. This data was captured using the [DataRecorder_HygroVue](../DataRecorder_HygroVue/DataRecorder_HygroVue.ino) script. These two captured sets of data were then merged into the single data file presented below.   

### Analysis

The sensor with an open enclosure has an almost immediate response, as expected. This is used to establish a start time of the test. We then identify the equilibrium value (max value measured by the sensor in the vented enclosure). The time constant of the system (&tau;) is defined as the time in which is takes the system to reach FinalVal x (1 - 1/e) which is equal to 0.6321 x FinalVal. 

Response times are evaluated for all devices involved and compared in the table below

| **Sensor** | **Response Time (&tau;) [s]** | **Approx. Time to Equilibrium [min]** |
|---:|:---:|:---:|
| HygroVue 10 (Control) | 217 | 18 |
| Haar, Closed | 381 | 31.75 |
| Haar, Open | 124 | 10.25 |

The worst case response, as expected, is the closed Haar sensor. Despite this, we find a time to equilibrium of **just over 30 minutes**, which is perfectly reasonable in many situations. Additonally, the open Haar sensor greatly out performs the control sensor. 

![Step Response](TempResponse.png)

[Python](TemperatureAnalysis.py)

[Jupyter Notebook](TemperatureAnalysis.ipynb)


### Data

[File](Data.csv)

Sensor A - Haar sensor with open 3D printed enclosure

Sensor B - Haar sensor with solid 3D printed enclosure and Gore-Tex vent

#### Thermal Regulated Box Construction
An insulated box was constructed from 1.5 inch (3.81cm) thick Extruded Poly Styrene (XPS) insulation foam which has an R value (BTU / hr x ft^2 x &deg;F) of 7.5, creating a highly insulated test chamber. Overview of this box is shown in [Figure 1](images/Fig1.jpg) and measurments are shown in [Box Schematic](images/TempBoxDimensions.png). A series of holes are placed in the top of the box to allow entry for cables for test devices - surrounded by rubber stoppers. All edges of the box are sercured and sealed with foil tape to prevent heat loss through gaps.
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="images/TempBoxDimensions_LIGHT.png">
  <img alt="Haar bare board" src="images/TempBoxDimensions.png">
</picture>
<!-- ![Box Schematic](images/TempBoxDimensions.png) -->

| **Dimension** | **H** | **W** | **D** | **h'** | **w'** | **d'** |
|---:|:---:|:---:|:---:|:---:|:---:|:---:|
| English (inch) | 28 | 16 | 19 | 25 | 13 | 16 |
| Metric (cm) | 71 | 44 | 41 | 63 | 33 | 44 |

Inside the box a temperature sensor ([T9602](https://www.amphenol-sensors.com/en/telaire/humidity/527-humidity-sensors/3224-t9602)), a 12V DC computer fan, and a coil of NiChrome wire are placed - this will along with a micro-controller and power supply constitutes the temperature control system. This contents can be seen in [Figure 2](images/Fig2.jpg). The box is heated by supplying current to the NiChrome coil via a Pulse Width Modulation (PWM) controlled 12V line. The fan is left running at full speed constantly to provide circulation within the box and ensure accurate and consistent temperature measurement. Temperature is sensed using the T9602 sensor by the micro-controller, then the output to the NiChrome heater is varied based on a simple PID controller implemented on the micro-controller. Between the insulation and the active control system the box maintains an accurate and, most importantly, stable temperature.

![Figure 1](images/Fig1.jpg)
![Figure 2](images/Fig2.jpg)
![Figure 3](images/Fig3.jpg)