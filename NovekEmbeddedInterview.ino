/***********************************************************************************************************************
 * Embedded Engineer Interview Test
 * 
 * Copyright 2020 Xavier Michael <xaviermike92@gmail.com>
 * Based on previous work by:
 *       Mr. Alam <https://how2electronics.com/send-gsm-sim800-900-gprs-data-thingspeak-arduino/>
 *       John Main <https://www.best-microcontroller-projects.com/max6675.html#L3870>
 *       
 *       
 * The following project reads temperature data and sends it to a webserver using an API.
 * The temperature sensor used is a thermocouple(Type K given the tye of amplifier used).
 * The thermocouple is connected to the MAX6675. The ic has an in-built amplifier, a cold junction compensator and an
 * ADC. The amplifier amplifies the signal from the thermocouple, the CJC is used to compensate for the ambient 
 * temperature while the ADC converts the analog signal to a 12 bit digital data packet.
 * This data is then stored in a JSON format and sent to a webserver/web address using an API through the sim800l modem.
 * NB: Hardware Serial is used to communicate between the sim800l modem and the arduino mega2560(Serial1).
 *     Therefore:
 *              SIM800L_TX -> Arduino Mega RX1(Pin 19)
 *              SIM800L_RX -> Arduino Mega TX1(Pin 18)
 *     Safaricom APN's is used. Change if another operator is used. This has been set in the sim800Init() function.
 ***********************************************************************************************************************/

#include <max6675.h>
#include <ArduinoJson.h>

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
    sim800Init();
}

void loop() {
  // put your main code here, to run repeatedly:
    sim800SendData();
    delay(30000);
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

/**
 * @brief: Sends the temperature data to the webserver/ web address using an API
*/
void sim800SendData()
{
    Serial1.println("AT+HTTPINIT"); // Initialize the HTTP Service
    delay(6000);
    showSerialData();   
    Serial1.println("AT+HTTPPARA=\"CID\",1"); // Sets the Bearer profile identifier to Bearer is connected
    delay(6000);
    showSerialData();

    /*  Parse the temperature data into JSON format */
    String temperature = (String)max6675TempRead();
    
    // Allocate the json buffer memory in the stack since the data is 12 bytes.
    StaticJsonDocument<100> jsonBuffer; // allocates a little more than a 200 bytes in the stack memory.
    jsonBuffer["t"] = temperature; // Initialize the JSONDocument array with an element with the key "t" and value of temperature
    String sendToServer; // string buffer to store the json format data
    serializeJson(jsonBuffer, sendToServer); // Serialize the JSON Document

    // Set the web address
    Serial1.println("AT+HTTPPARA=\"URL\",\"http://178.62.249.218:5000/temperature\""); 
    delay(4000);
    showSerialData();

    // Set the content type to json format
    Serial1.println("AT+HTTPPARA=\"CONTENT\",\"application/json\""); 
    delay(4000);
    showSerialData();

    // Set the size of the data in bytes and the maximum time in milliseconds to input the data
    Serial1.println("AT+HTTPDATA=" + String(sendToServer.length()) + ",100000"); 
    Serial.println(sendToServer);
    delay(6000);
    showSerialData();

    // Send the data
    Serial1.println(sendToServer);
    delay(6000);
    showSerialData;
    
    // Set HTTP Action Method as POST. Expected response: 200, OK
    Serial1.println("AT+HTTPACTION=1");
    delay(6000);
    showSerialData();

    // Reads the HTTP Server Response
    Serial1.println("AT+HTTPREAD");
    delay(6000);
    showSerialData();

    // Terminates the HTTP Service
    Serial1.println("AT+HTTPTERM");
    delay(10000);
    showSerialData;
}
/**
 * @brief: Reads  data from the sim800l modem and prints it to the Serial Monitor
*/
void showSerialData()
{
    while(Serial1.available() !=0) // As long as there is data being transmitted from the sim800l modem
    {
        Serial.write(Serial1.read()); // Read data from the sim800l and print it out to the serial monitor
        delay(1000);
    }
}
