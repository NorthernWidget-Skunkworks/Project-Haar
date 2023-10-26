#include <Haar.h>

Haar sensorA; //Instantiate Haar sensor A
Haar sensorB; //Instantiate Haar sensor B
unsigned long period = 100; //Number of ms to wait between sensor readings

void setup() {
	Serial.begin(115200); //Begin serial conunication at high speed 
	while (!Serial) {
		; // wait for serial port to connect
	}
	sensorA.begin(0x41); //Initialize Haar sensor A
	sensorB.begin(0x42); //Initialize Haar sensor B
	Serial.println("Time [ms],TemperatureA [°C],PressureA [mBar],HumidityA [%],TemperatureB [°C],PressureB [mBar],HumidityA [%]"); //Print out column headers for CSV output
}

void loop() {
	unsigned long startTime = millis(); //Record start of each reading to get timing precise 
	sensorA.updateMeasurements(true); //Update data for A and B, block until process is complete
	sensorB.updateMeasurements(true); 

	//Grab data from sensor A
	float humidityA = sensorA.getHumidity(); //Grab new humidity values [%]
	float pressureA = sensorA.getPressure(); //Grab new pressure values [mBar]
	float tempA = sensorA.getTemperature(); //Grab new temp values [°C]

	//Grab data from sensor B
	float humidityB = sensorB.getHumidity(); //Grab new humidity values [%]
	float pressureB = sensorB.getPressure(); //Grab new pressure values [mBar]
	float tempB = sensorB.getTemperature(); //Grab new temp values [°C]

	//Print out formatted data from A and B
	Serial.print(millis()); Serial.print(",");
	Serial.print(tempA, 4); Serial.print(","); //Print temp to 4 decimal places
	Serial.print(pressureA, 2); Serial.print(","); //Print pressure to 2 decimal places
	Serial.print(humidityA, 2); Serial.print(","); //Print humidity to 2 decimal places
	Serial.print(tempB, 4); Serial.print(","); //Print temp to 4 decimal places
	Serial.print(pressureB, 2); Serial.print(","); //Print pressure to 2 decimal places
	Serial.print(humidityB, 2); Serial.println(); //Print humidity to 2 decimal places
	while((millis() - startTime) < period); //Wait until logging interval has been reached 
}