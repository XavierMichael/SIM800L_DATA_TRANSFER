#include <max6675.h>

#define MAX6675_SO          8       // Slave out in. Transmits data from the max6675 to the arduino
#define MAX6675_CS          9       // Chip Select. Used by arduino to select the max6675 module
#define MAX6675_SCK         10      // Serial Clock. Transmits clock signal from arduino to max6675

/**
 * @brief: Declare a max6675 clss object and initialize it with the pins used for
 *         clock, chip select and slave out on the arduino board
*/
MAX6675 max6675Temp(MAX6675_SCK, MAX6675_CS, MAX6675_SO);

void  setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
