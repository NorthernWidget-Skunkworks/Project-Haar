# Testing - Pressure

Two different tests were performed to evaluate the system performance of Haar as a pressure sensor - a step change and a steady state response. In both tests, an ExTech SD700 was used as a standard of comparison. 

## Step Response

### Procedure 
A pair of Haar sensors (`v0.1` hardware) were placed in a large (5 gallon) sealed chamber - see [Figure 1](images/Fig1.jpg). This chamber was placed at a slight over-pressure (about 25 mBar) above atmospheric pressure. Once this pressure was stabilized, a ball valve was opened - rapidly returning the environment back to atmospheric pressure. This simulates the most extreme example of a pressure change one might see from a cold front, or other atmospheric phenomenon. Baseline pressure before and after the pressure change was recorded. 

Haar data was logged to a computer using the generic Haar [DataRecorder](../DataRecorder/DataRecorder.ino) script with `period` set to 100ms.

![Figure 1](images/Fig1.jpg)

### Analysis

For both sensors the pressure before and after release is within sensor specs (4 mBar absolute error) and response time difference is insignificant between a Gore-Tex vent and an open housing. In both cases system fully responds well within 1 second

A basic plot of the response is created using [PresureAnalysis](PresureAnalysis.py) Python script - this script prints the resulting trimmed data and creates a simple plot

![Step Response](PressureResponse.png)

[Python](PressureAnalysis.py)

[Jupyter Notebook](PressureAnalysis.ipynb)

### Data

[File](StepChangeData.csv)

Measured pressure before over pressure - 976.7 mBar

Measured pressure before release - 1003.2 mBar 

Measure pressure after release - 976.6 mBar

Sensor A - Haar sensor with solid 3D printed enclosure and Gore-Tex vent

Sensor B - Haar sensor with open 3D printed enclosure 