#include "Margay.h"
#include <Wire.h>

enum Sensor {
	RH = 0,
	Pressue = 1
};

#define CTRL1 0x00
#define TEMP_PRES 0x02
#define TEMP_RH 0x04
#define PRES_REG 0x06
#define RH_REG 0x09

uint8_t ADR = 0x40; //Global address

Margay Logger(Model_2v0);

void setup() 
{
	Logger.PowerAux(OFF); 
	delay(500); //Cycle power 
	Logger.PowerAux(ON);  //Turn on external power

	Wire.begin();
	Serial.begin(9600); //Initalze serial for output

  Wire.beginTransmission(0x40);
  Wire.write(0x00);
  Wire.write(0x01);
  Serial.println(Wire.endTransmission());
}

void loop() 
{
  Wire.beginTransmission(0x40);
  Wire.write(0x00);
  Wire.write(0x01);
  Serial.println(Wire.endTransmission());
  delay(5000);
	//Print data
	Serial.println("Begin Data...");
	Serial.print("Temp, RH = "); Serial.print(GetTemp(RH)); Serial.println("°C");
	Serial.print("Temp, Pres = "); Serial.print(GetTemp(Pressue)); Serial.println("°C");
	Serial.print("Pressure = "); Serial.print(GetPressure()); Serial.println("mBar");
	Serial.print("Humidity = "); Serial.print(GetRH()); Serial.println("%");
	Serial.println("... End Data");
	delay(1000); //Wait a bit
}

float GetPressure()   //Returns pressure [mBar]
{
	uint32_t Val = 0; //Val for getting/calculating pressure value
	// uint8_t Data[3] = {0}; //Storage for byte values
//	Wire.beginTransmission(ADR);
//	Wire.write(PRES_REG);
//	Wire.endTransmission();
//	Wire.requestFrom(ADR, 3);
//	while(Wire.available() < 3); //Wait for data to be ready //FIX!!!
  uint32_t Temp = 0; //DEBUG
	for(int i = 0; i < 3; i++) {
		// Data[i] = Wire.read(); //Read in 24 bit value
//		Val = (Val << 8) | Wire.read();
    Wire.beginTransmission(ADR);
    Wire.write(PRES_REG + i);
    Wire.endTransmission();
    Wire.requestFrom(ADR, 1);
    Temp = Wire.read(); //DEBUG!
    Val = (Temp << 8*i) | Val; //DEBUG!
//    Serial.println(Val, HEX); //DEBUG!
	}
	// Val = (Data[2] << 16) | (Data[1] << 8) | Data[0]; //Concatonate values 

//	if (Val & 0x800000){  //Sign extend if required 
//      Val = (0xFF000000 | Val);
//    }

  	return (Val / 4096.0);
}

float GetRH()  //Returns humidity [%]
{
	float Val = 0; //Val for getting/calculating RH value
	Val = GetWord(RH_REG);
	Val = (100.0*Val)/65535.0;  //Convert to RH
	return Val;
}

float GetTemp(Sensor Device)  //Returns temp [C], pass which sensor is desired 
{
	float Val = 0; //Val for getting/calculating temp value
	if(Device == Pressue) {
		Val = GetWord(TEMP_PRES);
		Val = Val/100.0; //Convert to C
	}

	else if (Device == RH) {
		Val = GetWord(TEMP_RH);
		Val = ((Val*175.0)/65535.0) - 45; //Convert to C
	}
	return Val;
}

uint16_t GetWord(uint8_t Reg)  //Returns word, read from Reg position
{
	uint16_t Val = 0; //Val to be read from device
	Wire.beginTransmission(ADR);
	Wire.write(Reg);
	Wire.endTransmission();

	Wire.requestFrom(ADR, 1);  //Request word
//	while(Wire.available() < 2); //Wait //FIX! Add timeout
	Val = Wire.read();
//  Serial.println(Val, HEX);

  Wire.beginTransmission(ADR);
  Wire.write(Reg + 1);
  Wire.endTransmission();
  Wire.requestFrom(ADR, 1);  //Request word
	Val = Val | (Wire.read() << 8);  //Concatonate 16 bits 
//  Val = Wire.read() | (Val << 8);  //Concatonate 16 bits //DEBUG!
	return Val; 
}
