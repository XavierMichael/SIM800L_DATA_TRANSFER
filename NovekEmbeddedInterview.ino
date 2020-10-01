#include <max6675.h>

#define MAX6675_SO          8       // Slave out in. Transmits data from the max6675 to the arduino
#define MAX6675_CS          9       // Chip Select. Used by arduino to select the max6675 module
#define MAX6675_SCK         10      // Serial Clock. Transmits clock signal from arduino to max6675

/**
 * @brief: Declare a max6675 clss object and initialize it with the pins used for
 *         clock, chip select and slave out on the arduino board
*/
MAX6675 max6675Temp(MAX6675_SCK, MAX6675_CS, MAX6675_SO);

// Function Prototypes
void sim800Init();
float max6675TempRead();
void sim800SendData();
void showSerialData();

void  setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

/**
 * @brief Reads the temperature from the max6675
 * @retVal Float temperature value if a thermocouple is attached to the max6675
 *         Otherwise returns NAN
*/
float max6675TempRead()
{
    // Declare the variable as static so that it lasts beyond the lifetime of the function
    static float tempCelsius = max6675Temp.readCelsius();
    // Print out the temperature
    Serial.print("The temperature in Celsius Degrees is: "); Serial.println(tempCelsius);
    return tempCelsius;    
}
