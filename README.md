# SIM800L_DATA_TRANSFER

The following project reads temperature data and sends it to a webserver using an API.
The temperature sensor used is a thermocouple(Type K given the tye of amplifier used).
The thermocouple is connected to the MAX6675. The ic has an in-built amplifier, a cold junction compensator and an
ADC. The amplifier amplifies the signal from the thermocouple, the CJC is used to compensate for the ambient 
temperature while the ADC converts the analog signal to a 12 bit digital data packet.
This data is then stored in a JSON format and sent to a webserver/web address using an API through the sim800l modem.
NB: Hardware Serial is used to communicate between the sim800l modem and the arduino mega2560(Serial1).
   Therefore:
            SIM800L_TX -> Arduino Mega RX1(Pin 19)
            SIM800L_RX -> Arduino Mega TX1(Pin 18)
   Safaricom APN's is used. Change if another operator is used. This has been set in the sim800Init() function.

