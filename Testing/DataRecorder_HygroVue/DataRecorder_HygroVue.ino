#include <Arduino.h>

//Interface using SDI-12 Symbiont v0.0 (https://github.com/RTGS-Lab/Symbiont-SDI12)
const uint8_t TX = 1;
const uint8_t RX = 0;
const uint8_t FOut = 12;
const uint8_t Dir = 11;

#define MARKING_PERIOD 9 //>8.33ms for standard marking period
const unsigned long TimeoutStandard = 380; //Standard timeout period for most commands is 380ms 

char ReadArray[25] = {0};

const unsigned long period = 1000; //Wait 1 second between logs

void setup() {
	pinMode(TX, OUTPUT);
	pinMode(FOut, OUTPUT);
	pinMode(Dir, OUTPUT);

	digitalWrite(Dir, HIGH); //Set to transmit 
	Serial1.begin(1200, SERIAL_7E1);
	Serial.begin(115200);
	while(!Serial) { //Wait until serial interface is available 
		;
	} 
	Serial.println("HygroVue10");
	Serial.println("Time[s]\tTemp[°C]\tRH[%]");
}

void loop() {
	SendCommand("0M!");
	delay(100);
	String data = SendCommand("0D0!");
	data = data.substring(1, data.length()); //Trim address val
	float temp, rh;
	temp = DataToFloat(data.substring(0, NextIndexOfSep(data))); //Grab first float from data
	data = data.substring(NextIndexOfSep(data) + 1, data.length()); //Trim first chunk off of data
	rh = DataToFloat(data.substring(0, NextIndexOfSep(data))); //Grab other float from data
	static unsigned long printTime = millis();
	if(floor(millis()/1000.0) != millis()/1000.0) { //If data out of sync, perform sync
		while(floor(millis()/1000.0) != millis()/1000.0); //Wait for seconds to match
	}
	while((millis() - printTime) < 1000.0); //Wait until time is up
	printTime = millis();
	Serial.print(millis()/1000.0);
	Serial.print('\t');
	Serial.print(temp);
	Serial.print('\t');
	Serial.println(rh);
}

uint8_t IndexOfSep(String val) //Find the first index of the value seperator 
{
	uint8_t pos = val.indexOf('+');
	uint8_t neg = val.indexOf('-');
	if(pos == -1 || neg == -1) return max(pos, neg); //If one of them is not found, return the other one
	else return min(pos, neg); //If both are valid, then return the first
}

uint8_t NextIndexOfSep(String val) //Find the NEXT index of the value seperator 
{
	uint8_t pos = val.indexOf('+');
	uint8_t neg = val.indexOf('-');
	if(pos == -1 || neg == -1) return max(pos, neg); //If one of them is not found, return the other one
	else if (pos == 0 || neg == 0) return IndexOfSep(val.substring(1, val.length())); //If the first value is a seperator, call normal IndexOfSep, but give is 1:n as an inpus
	else return max(pos, neg); //If both are valid, AND the first element is not a seperator, then return the LATER one
}

float DataToFloat(String val)
{
	char sign = val.charAt(0); //Grab first char to test for signage
	val = val.substring(1, val.length()); //Trim sign char
	float data = val.toFloat();
	data = (sign == '+') ? data : -1.0*data; //Adjust sign of data if needed
	return data;
}

char GetAddress()
{
	SendBreak(); //Send break to start message
	Mark(MARKING_PERIOD); //Mark for standard period before command
	Serial1.begin(1200, SERIAL_7E1);
	Serial1.print("0I!");
	Serial1.flush(); //Make sure data is transmitted before releasing the bus
	delay(1);
	ReleaseBus(); //Switch bus to recieve 

	unsigned long LocalTime = millis();
	while(Serial1.available() < 3 && (millis() - LocalTime) <  TimeoutStandard);
	String Val = Serial1.readStringUntil('\r');
	Serial.println(Val);
	return 0; //DEBUG!
}

void SendBreak()
{
	pinMode(Dir, OUTPUT); //Make sure direction is set to output
	pinMode(FOut, OUTPUT); //Make sure the force out pin is in output mode
	digitalWrite(Dir, HIGH); //Set direction to output
	digitalWrite(FOut, LOW); //Set low to force output high
	delay(13); //Wait for bus to acnowledge action
	digitalWrite(FOut, HIGH); //Stop forcing output
}

void Mark(unsigned long Period)
{
	pinMode(Dir, OUTPUT); //Make sure direction control pin is set as an output
	pinMode(TX, OUTPUT); //Make sure transmit pin is set as output
	digitalWrite(Dir, HIGH); //Set direction to output
	digitalWrite(TX, 1); //Begin marking condition
	delay(Period); //Wait for a given marking period
	digitalWrite(TX, 0); //Stop marking  
}

void Space(unsigned long Period)
{
	pinMode(Dir, OUTPUT); //Make sure direction control pin is set as an output
	pinMode(TX, OUTPUT); //Make sure transmit pin is set as output
	digitalWrite(Dir, HIGH); //Set direction to output
	digitalWrite(TX, 0); //Begin spacing condition
	delay(Period); //Wait for a given marking period
	digitalWrite(TX, 1); //Stop spacing
}

void ReleaseBus() 
{
	pinMode(Dir, OUTPUT); //Make sure direction pin is set as an output
	digitalWrite(Dir, LOW); //Set direction to input
}

String SendCommand(String Command) 
{
	SendBreak(); //Send break to start message
	Mark(MARKING_PERIOD); //Mark for standard period before command
	Serial1.begin(1200, SERIAL_7E1);
	Serial1.print(Command);
	Serial1.flush(); //Make sure data is transmitted before releasing the bus
	delay(1);
	ReleaseBus(); //Switch bus to recieve 

	unsigned long LocalTime = millis();
	char Data[100] = {0}; //Make data array for storage FIX! Change length to not be arbitrary
	bool GotData = false; //Used to keep track of terminating character has been recieved 
	int Pos = 0; //Keep track of position in data array
	while(!GotData && (millis() - LocalTime) <  TimeoutStandard) {
		if(Serial1.available() > 0) {
			Data[Pos] = Serial1.read(); //If byte is available, read it in
			Pos++; //Increment position
		}
		if(Data[Pos] == '\n') GotData = true; //Set flag if end character is read in
	}
	String Val = String(Data); //Convert to String
	Val.trim(); //Get rid of any trailing characters 
	return Val; 
}