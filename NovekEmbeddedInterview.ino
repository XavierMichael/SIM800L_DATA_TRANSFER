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
    Serial.begin(9600);
    // Check to see if thermocouple is connected to the max6675
    float tempC = max6675Temp.readCelsius();
    (tempC == NAN)? Serial.println("No thermocouple attached"): Serial.println("Thermocouple is attached");

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

/**
 * @brief: Initializes the sim800
*/
void sim800Init()
{
    Serial1.begin(9600); // Setting the baudrate of the sim800l to 9600
    /* Check if the sim800l is properly connected to the arduino mega
     * If not connected, the execution remain in this loop.
     */
    while(!Serial1.available())
    {
        Serial1.println("AT");
        delay(1000);
        Serial.println("Connecting...");
    }
    Serial.print("Connected");
    Serial1.println("AT+CMEE=2"); //Set the ME's result error code
    delay(2000);
    Serial1.println("AT+CPIN?"); //Checks for pin status. Can be used to check if sim is inserted(properly) or not.
    delay(2000);
    showSerialData();
    Serial1.println("AT+CSQ"); //Returns signal strength indication. Response= +CSQ: <rssi>,<ber>  
    delay(2000);
    showSerialData();
    Serial1.println("AT+CSCS?"); //Checks for terminal equipment's(TE) ch_set. Can be used to check if antennae is ok.
    delay(2000);  
    showSerialData();
    /**
     * @brief AT+SAPBR : Bearer settings for applications based on IP 
    */
    Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); // Sets the bearer internet connection type to GPRS
    delay(6000);
    showSerialData();
    /*
     * Set the APN, username and password
    */
    Serial1.println("AT+SAPBR=3,1,\"APN\",\"safaricom\""); // Set APN
    delay(6000);
    showSerialData();
    //NB: Comment out username and password settings when using Airtel
    Serial1.println("AT+SAPBR=3,1,\"USER\",\"saf\""); // Set Username
    delay(6000);
    showSerialData();
    Serial1.println("AT+SAPBR=3,1,\"PWD\",\"data\""); // Set Password
    delay(6000);
    showSerialData();    

    Serial1.println("AT+SAPBR=1,1"); // Open the bearer
    delay(6000);
    showSerialData();
    Serial1.println("AT+SAPBR=2,1"); // Query the bearer
    delay(6000);
    showSerialData();      
}
