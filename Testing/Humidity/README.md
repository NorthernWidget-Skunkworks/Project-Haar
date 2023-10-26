# Testing - Humidity

A step response of humidity performance was evaluated with the use of a ultrasonic humidifier and a general purpose test chamber. 

### Procedure 
A pair of Haar sensors (`v0.1` hardware) were placed in a large (5 gallon) sealed chamber - see [Figure 1](images/Fig1.jpg). This chamber was left open and allowed to reach atmospheric equilibrium. Once the humidity was stabilized, an ultrasonic humidifier was used to inject a small amount of extremely humid air (>90%) into the test chamber, increasing the ambient humidity in the box by a small amount (<10%). Once this injection was completed, the inlet ports to the chamber were sealed - isolating the chamber. An ultrasonic humidifier was chosen as it can produce extremely humid air without appreciably affecting the temperature of the environment, which would add undesired variability to the test. This simulates an extreme example of a humidity due to a shift in weather system. Haar data was recorded until such time as both units had reached equilibrium.

Haar data was logged to a computer using the generic Haar [DataRecorder](../DataRecorder/DataRecorder.ino) script with `period` set to 100ms.

![Figure 1](images/Fig1.jpg)

### Analysis

The sensor with an open enclosure has an almost immediate response, as expected. This is used to establish a start time of the test. We then identify the equilibrium value (max value measured by the sensor in the vented enclosure). The time constant of the system (&tau;) is defined as the time in which is takes the system to reach FinalVal x (1 - 1/e) which is equal to 0.6321 x FinalVal. 

This is evaluated for the data collected and found to be **approximately 54 minutes**. 
### Data

<!-- [File](StepChangeData.txt) -->

Sensor A - Haar sensor with solid 3D printed enclosure and Gore-Tex vent

Sensor B - Haar sensor with open 3D printed enclosure 