//PID thermal box controller
//Gate of power N-MOS connected to pin 11
//T9602 used as input sensor

#include <PID_v1.h>
#include <T9602.h>

#define PIN_OUTPUT 11

T9602 tempSense;

double Setpoint, Input, Output;

double Kp=600, Ki=3, Kd=15000; 
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  //initialize the variables we're linked to
  Serial.begin(9600);
  tempSense.begin();

  Setpoint = 25.0;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  tempSense.updateMeasurements();
  Input = tempSense.getTemperature();
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
  Serial.flush();
  Serial.print("Time:\t");
  Serial.print(millis()/1000.0);
  Serial.print("\tTemp:\t");
  Serial.print(Input);
  Serial.print("\tOutput:\t");
  Serial.println(Output);
}


