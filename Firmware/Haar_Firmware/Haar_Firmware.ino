//Haar_Firmware_Test.ino

#include "SlowSoftI2CMaster.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include "WireS.h"


#define SHT31_ADDR    0x45
#define LPS35HW_ADDR 	0x5D ///< LPS35HW default i2c address

#define SHT31_MEAS_HIGHREP_STRETCH 0x2C06
#define SHT31_MEAS_MEDREP_STRETCH  0x2C0D
#define SHT31_MEAS_LOWREP_STRETCH  0x2C10
#define SHT31_MEAS_HIGHREP         0x2400
#define SHT31_MEAS_MEDREP          0x240B
#define SHT31_MEAS_LOWREP          0x2416
#define SHT31_READSTATUS           0xF32D
#define SHT31_CLEARSTATUS          0x3041
#define SHT31_SOFTRESET            0x30A2
#define SHT31_HEATEREN             0x306D
#define SHT31_HEATERDIS            0x3066

#define LPS35HW_CTRL_REG1     0x10 ///< Control register 1
#define LPS35HW_CTRL_REG2     0x11 ///< Control register 2
#define LPS35HW_CTRL_REG3     0x12 ///< Control register 3
#define LPS35HW_PRESS_OUT_XL  0x28 ///< Pressure low byte
#define LPS35HW_PRESS_OUT_L   0x29 ///< Pressure mid byte
#define LPS35HW_PRESS_OUT_H   0x2A ///< Pressure high byte
#define LPS35HW_TEMP_OUT_L    0x2B ///< Temperature low byte
#define LPS35HW_TEMP_OUT_H    0x2C ///< Temperature high byte

#define LPS35HW_CTRL_REG2_DEFAULT 0x10

#define READ 0x01
#define WRITE 0x00

unsigned long ReadTimeout = 100; //Wait at most 100ms for new read

uint8_t Config = 0; //Global config value

uint8_t Reg[10] = {0}; //Initialize registers
bool Sample = true; //Flag used to start a new converstion
bool Sleep = false; //Used to put the device into deep sleep //ADD
bool Startup = false; 

uint16_t ST, SRH; //Global values for RH sensor (FIX!!!!)

volatile uint8_t ADR = 0x40; //Use arbitraty address, change using generall call??

SlowSoftI2CMaster si = SlowSoftI2CMaster(PIN_C4, PIN_C5, true);  //Initialize software I2C

volatile bool StopFlag = false; //Used to indicate a stop condition 
volatile uint8_t RegID = 0; //Used to denote which register will be read from
volatile bool RepeatedStart = false; //Used to show if the start was repeated or not

void setup() {
	pinMode(15, OUTPUT); //DEBUG!
	digitalWrite(15, HIGH); //DEBUG!
	Reg[0] = 0x00; //Set Config to POR value
	Wire.begin(ADR);  //Begin slave I2C

	Wire.onAddrReceive(addressEvent); // register event
	Wire.onRequest(requestEvent);     // register event
	Wire.onReceive(receiveEvent);
	Wire.onStop(stopEvent);

  	si.i2c_init(); //Begin I2C master
  	// Serial.begin(4800); //DEBUG!
  	RHreset(); //Reset RH sensor on startup 
  	PresReset(); //Reset pressure sensor on startup

  	WriteByte(LPS35HW_ADDR, LPS35HW_CTRL_REG3, 0x40);; //Setup 50Hz data rate //DEBUG!

}

void loop() {
	Sample = BitRead(Reg[0], 0);

	if(Sample == true || Startup == false) {  //FIX!!! Make first conversion cleaner 
		WriteByte(LPS35HW_ADDR, LPS35HW_CTRL_REG2, LPS35HW_CTRL_REG2_DEFAULT | 0x01); //Set ONE_SHOT bit in order to trigger new conversion for pressure
		readRH(); //Get new temp/RH values
		SplitAndLoad(0x04, ST); //Load temp from RH sensor
		SplitAndLoad(0x09, SRH); //Load RH 
		ReadPres(); //FIX!!! Make non-blocking/parellel conversion 

		Reg[0] = Reg[0] & 0xFE; //Clear sample bit in register
		Sample = false; //Clear sample bit
		Startup = true; //Set after first conversion 
	}

	//Sleep after loading registers 
	// ADCSRA &= ~(1<<ADEN); //Disable ADC
	// SPCR   &= ~_BV(SPE); //Disable SPI
	//    PRR = 0xFF;
  	digitalWrite(15, LOW); //DEBUG!

	if(Sleep) set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
	else set_sleep_mode(SLEEP_MODE_STANDBY);
	sleep_enable();
	sleep_mode(); //Waits here while in sleep mode

	sleep_disable(); //Wake up
	TWSCRA = bit(TWEN);  //Re-enable I2C
	Wire.begin(ADR);
	si.i2c_init(); //Begin I2C master


	delay(10); //DEBUG!
}

uint16_t readStatusRH(void) {
  writeCommand(SHT31_READSTATUS);
  si.i2c_start((SHT31_ADDR << 1) | READ);
  // Wire.requestFrom(SHT31_ADDR, (uint8_t)3);
  uint16_t stat = si.i2c_read(false);;
  stat <<= 8;
  stat |= si.i2c_read(false);;
  //Serial.println(stat, HEX);
  return stat;
}

void RHreset(void) {
  writeCommand(SHT31_SOFTRESET);
  delay(10);
}

void PresReset(void) {
	Serial.println(WriteByte(LPS35HW_ADDR, LPS35HW_CTRL_REG2, 0x04)); //Set bit 2 in CtrlReg2 to force a software reset //DEBUG!
}

void heater(boolean h) {
  if (h)
    writeCommand(SHT31_HEATEREN);
  else
    writeCommand(SHT31_HEATERDIS);
}



// int16_t readTemperatureRH(void) {
//   if (! readTempHum()) return NAN;

//   return ST;
// }
  

// uint16_t readHumidity(void) {
//   if (! readTempHum()) return NAN;

//   return SRH;
// }
bool ReadPres(void) {
	// WriteByte(LPS35HW_ADDR, LPS35HW_CTRL_REG2, LPS35HW_CTRL_REG2_DEFAULT | 0x01); //Set ONE_SHOT bit in order to trigger new conversion
	unsigned long Timeout = millis();
	bool Done = false; //Flag to wait for new data
	while((millis() - Timeout) < ReadTimeout && !Done) {
		if(BitRead(ReadByte(LPS35HW_ADDR, LPS35HW_CTRL_REG2), 0) == 0) { //Wait for ONE_SHOT to be cleared 
			Done = true; //Set flag 
		}
	}
	if(Done) {  //If read succesfully 
		Reg[0x06] = ReadByte(LPS35HW_ADDR, LPS35HW_PRESS_OUT_XL); //Read out LSB
		Reg[0x07] = ReadByte(LPS35HW_ADDR, LPS35HW_PRESS_OUT_L); //Read out Mid byte
		Reg[0x08] = ReadByte(LPS35HW_ADDR, LPS35HW_PRESS_OUT_H); //Read out MSB

		Reg[0x02] = ReadByte(LPS35HW_ADDR, LPS35HW_TEMP_OUT_L); //Read out LSB 
		// Reg[0x02] = 10;
		Reg[0x03] = ReadByte(LPS35HW_ADDR, LPS35HW_TEMP_OUT_H); //Read out MSB 
	}
	return Done; //Return valid status
}

boolean readRH(void) {
  uint8_t readbuffer[6];

  writeCommand(SHT31_MEAS_HIGHREP);
  
  delay(30);  //FIX! Read byte and test for NACK value
  // Wire.requestFrom(SHT31_ADDR, (uint8_t)6);
  si.i2c_start((SHT31_ADDR << 1) | READ);
  // if (Wire.available() != 6) //FIX??
    // return false;
  for (uint8_t i=0; i<6; i++) {
    readbuffer[i] = si.i2c_read(false);
  //  Serial.print("0x"); Serial.println(readbuffer[i], HEX);
  }
  si.i2c_stop();
  // uint16_t ST, SRH;
  ST = readbuffer[0];
  ST <<= 8;
  ST |= readbuffer[1];

  if (readbuffer[2] != crc8(readbuffer, 2)) return false;

  SRH = readbuffer[3];
  SRH <<= 8;
  SRH |= readbuffer[4];

  if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;

//  // Serial.print("ST = "); Serial.println(ST);
//   double stemp = ST;
//   stemp *= 175;
//   stemp /= 0xffff;
//   stemp = -45 + stemp;
//   temp = stemp;
  
// //  Serial.print("SRH = "); Serial.println(SRH);
//   double shum = SRH;
//   shum *= 100;
//   shum /= 0xFFFF;
  
//   humidity = shum;
  
  return true;
}

void writeCommand(uint16_t cmd) {
  // Wire.beginTransmission(SHT31_ADDR);
  si.i2c_start((SHT31_ADDR << 1) | WRITE);
  si.i2c_write(cmd >> 8);
  si.i2c_write(cmd & 0xFF);
  // Wire.endTransmission();  
  si.i2c_stop();
}

uint8_t crc8(const uint8_t *data, int len)
{
/*
*
 * CRC-8 formula from page 14 of SHT spec pdf
 *
 * Test data 0xBE, 0xEF should yield 0x92
 *
 * Initialization data 0xFF
 * Polynomial 0x31 (x8 + x5 +x4 +1)
 * Final XOR 0x00
 */

  const uint8_t POLYNOMIAL(0x31);
  uint8_t crc(0xFF);
  
  for ( int j = len; j; --j ) {
      crc ^= *data++;

      for ( int i = 8; i; --i ) {
	crc = ( crc & 0x80 )
	  ? (crc << 1) ^ POLYNOMIAL
	  : (crc << 1);
      }
  }
  return crc;
}


/////////// Utility Functions///////////////
bool BitRead(uint8_t Val, uint8_t Pos) //Read the bit value at the specified position
{
	return (Val >> Pos) & 0x01;
}

uint8_t SendCommand(uint8_t Adr, uint8_t Command)
{
    si.i2c_start((Adr << 1) | WRITE);
    bool Error = si.i2c_write(Command);
    // si.i2c_stop(); //DEBUG!
    return 1; //DEBUG!
}

uint8_t WriteWord(uint8_t Adr, uint8_t Command, unsigned int Data)  //Writes value to 16 bit register
{
	si.i2c_start((Adr << 1) | WRITE);
	si.i2c_write(Command); //Write Command value
	si.i2c_write(Data & 0xFF); //Write LSB
	uint8_t Error = si.i2c_write((Data >> 8) & 0xFF); //Write MSB
	si.i2c_stop();
	return Error;  //Invert error so that it will return 0 is works
}

uint8_t WriteByte(uint8_t Adr, uint8_t Command, uint8_t Data)  //Writes value to 16 bit register
{
	si.i2c_start((Adr << 1) | WRITE);
	si.i2c_write(Command); //Write Command value
	uint8_t Error = si.i2c_write((Data) & 0xFF); //Write MSB
	si.i2c_stop();
	return Error;  //Invert error so that it will return 0 is works
}

uint8_t WriteWord_LE(uint8_t Adr, uint8_t Command, unsigned int Data)  //Writes value to 16 bit register
{
	si.i2c_start((Adr << 1) | WRITE);
	si.i2c_write(Command); //Write Command value
	si.i2c_write((Data >> 8) & 0xFF); //Write MSB
	si.i2c_write(Data & 0xFF); //Write LSB
	si.i2c_stop();
	// return Error;  //Invert error so that it will return 0 is works
}

// uint8_t WriteConfig(uint8_t Adr, uint8_t NewConfig)
// {
// 	si.i2c_start((Adr << 1) | WRITE);
// 	si.i2c_write(CONF_CMD);  //Write command code to Config register
// 	uint8_t Error = si.i2c_write(NewConfig);
// 	si.i2c_stop();
// 	if(Error == true) {
// 		Config = NewConfig; //Set global config if write was sucessful 
// 		return 0;
// 	}
// 	else return -1; //If write failed, return failure condition
// }

int ReadByte(uint8_t Adr, uint8_t Command, uint8_t Pos) //Send command value, and high/low byte to read, returns desired byte
{
	bool Error = SendCommand(Adr, Command);
	si.i2c_rep_start((Adr << 1) | READ);
	uint8_t ValLow = si.i2c_read(false);
	uint8_t ValHigh = si.i2c_read(false);
	si.i2c_stop();
	Error = true; //DEBUG!
	if(Error == true) {
		if(Pos == 0) return ValLow;
		if(Pos == 1) return ValHigh;
	}
	else return -1; //Return error if read failed

}

int ReadByte(uint8_t Adr, uint8_t Command) //Send command value, and high/low byte to read, returns desired byte
{
	bool Error = SendCommand(Adr, Command);
	si.i2c_rep_start((Adr << 1) | READ);
	// si.i2c_start((Adr << 1) | READ);
	uint8_t Val = si.i2c_read(true);
	si.i2c_stop();
	Error = true; //DEBUG!
	if(Error == true) {
		return Val;
	}
	else return -1; //Return error if read failed

}

int ReadWord(uint8_t Adr, uint8_t Command)  //Send command value, returns entire 16 bit word
{
	bool Error = SendCommand(Adr, Command);
	Serial.print("Error = "); Serial.println(Error); //DEBUG!
	si.i2c_rep_start((Adr << 1) | READ);
	uint8_t ByteLow = si.i2c_read(false);  //Read in high and low bytes (big endian)
	uint8_t ByteHigh = si.i2c_read(false);
	si.i2c_stop();
	// if(Error == true) return ((ByteHigh << 8) | ByteLow); //If read succeeded, return concatonated value
	// else return -1; //Return error if read failed
	return ((ByteHigh << 8) | ByteLow); //DEBUG!
}

int ReadWord_LE(uint8_t Adr, uint8_t Command)  //Send command value, returns entire 16 bit word
{
	bool Error = SendCommand(Adr, Command);
	si.i2c_stop();
	si.i2c_start((Adr << 1) | READ);
	uint8_t ByteHigh = si.i2c_read(false);  //Read in high and low bytes (big endian)
	uint8_t ByteLow = si.i2c_read(false);
	si.i2c_stop();
	// if(Error == true) return ((ByteHigh << 8) | ByteLow); //If read succeeded, return concatonated value
	// else return -1; //Return error if read failed
	return ((ByteHigh << 8) | ByteLow); //DEBUG!
}

void SplitAndLoad(uint8_t Pos, unsigned int Val) //Write 16 bits
{
	uint8_t Len = sizeof(Val);
	for(int i = Pos; i < Pos + Len; i++) {
		Reg[i] = (Val >> (i - Pos)*8) & 0xFF; //Pullout the next byte
	}
}

void SplitAndLoad(uint8_t Pos, long Val)  //Write 32 bits
{
	uint8_t Len = sizeof(Val);
	for(int i = Pos; i < Pos + Len; i++) {
		Reg[i] = (Val >> (i - Pos)*8) & 0xFF; //Pullout the next byte
	}
}

boolean addressEvent(uint16_t address, uint8_t count)
{
	RepeatedStart = (count > 0 ? true : false);
	return true; // send ACK to master
}

void requestEvent()
{	
	//Allow for repeated start condition 
	if(RepeatedStart) {
		for(int i = 0; i < 2; i++) {
			Wire.write(Reg[RegID + i]);
		}
	}
	else {
		Wire.write(Reg[RegID]);
	}
}

void receiveEvent(int DataLen) 
{
    //Write data to appropriate location
    if(DataLen == 2){
	    //Remove while loop?? 
	    while(Wire.available() < 2); //Only option for writing would be register address, and single 8 bit value
	    uint8_t Pos = Wire.read();
	    uint8_t Val = Wire.read();
	    //Check for validity of write??
	    Reg[Pos] = Val; //Set register value
	}

	if(DataLen == 1){
		RegID = Wire.read(); //Read in the register ID to be used for subsequent read
	}
}

void stopEvent() 
{
	StopFlag = true;
	//End comunication
}