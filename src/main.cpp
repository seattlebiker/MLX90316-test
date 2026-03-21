// Bare bones code for testing the MLX90316 angle sensor
//
// Idea from: https://www.instructables.com/id/Accurate-Wireless-Weather-Vane/
// 
// From <http://interface.khm.de/index.php/lab/interfaces-advanced/rotary-positionsensor-mlx90316/> 
//
// * MLX90316 Rotary Position Sensor
// * KHM 2010 /  Martin Nawrath
// * Kunsthochschule fuer Medien Koeln
// * Academy of Media Arts Cologne
// */
// The MLX90316BDG is a rotary position sensor which measures the direction of a 
// magnetic field with an SPI interface. It is a substitution for potentiometers or 
// shaft encoders. The resolution is about 1/10 of a degree with an update rate 
// of 2 mS. A little magnet* (sic) placed on a shaft or a joint where the sensor is placed 
// next to it is the typical configuration. To obtain the best accuracy the 
// rotation axis of the magnet should be placed over the center of the chip. 
// The MLX90316 library helps to read the chip data.
//
// *Meaning a diametrically-magnetized Neobidinium magnet.
//
//===============================================================================
//
// The below is from the author of Metro, which is a "cron-like" llibrary for the
// Arduino/ESP8266: 
// https://github.com/thomasfredericks/Metro-Arduino-Wiring
// 
// Metro works well, but this is the new version : https://github.com/SofaPirate/Chrono
//
// Metro library for Arduino or Wiring that facilitates the implementation of recurring 
//timed events by Thomas Ouellet Fredericks with contributions from: Paul Bouchier and 
// Benjamin.soelberg
//
//#include "Metro.h"     //Include Metro library
//#include "MLX90316.h"  // Include MLX90316 library
//int pinSS = 5;
//int pinSCK = 3;
//int pinMOSI = 4;
//int ii;
//Metro mlxMetro = Metro(5);
//MLX90316 mlx_1  = MLX90316();
//void setup(){
//  Serial.begin(115200);
//  mlx_1.attach(pinSS,pinSCK, pinMOSI );
//  Serial.println(" MLX90316 Rotary Position Sensor");
//}
//void loop() {
//  if (mlxMetro.check() == 1) {
//    ii = mlx_1.readAngle();
//    Serial.print(ii);
//    Serial.println("");
//  }
//}

#include <Chrono.h> 
#include "MLX90316.h"     // Include MLX90316 library
#include <Wire.h>   
   
#define ONE_SECOND_IN_MILLIS            1000        // milliseconds
#define ONE_MINUTE_IN_MILLIS            1000 * 60   // One minute in milliseconds

#define MLXpinSCLK         D6      // GPIO12 to pin 4 on MLX angle sensor
#define MLXpinMOSI         D7      // GPIO13 to pin 5 on MLX angle sensor
#define MLXpinSS           D8      // GPIO15 to pin 3 on MLX angle sensor

int angle;
Chrono mlxChrono(Chrono::SECONDS);       //5
MLX90316 mlx_1  = MLX90316();

//
//------------------------------- MetOne 034B Windvane-------------------------------------------
//
float wind_direction_avg;    // average wind direction
int vane_value;              // raw analog value from wind vane
int direction;               // translated 0 - 360 direction
int CalDirection;            // converted value with offset applied
#define Offset 0;
//char heading[10];            // Place to to put heading obtained by getHeading()

int  getWindDirection(void);
char * getHeading(int);

void setup() {
  Serial.begin(9600);
  
  Serial.println(" MLX90316 Rotary Position Sensor Test");
 
  //Initializes the SPI bus by setting SCK, MOSI, and SS to 
  // outputs, pulling SCK and MOSI low, and SS high.
  mlx_1.attach(MLXpinSS, MLXpinSCLK, MLXpinMOSI );  
}


void loop() {

  if ( mlxChrono.hasPassed(2) ) {   // returns true if it passed 2 seconds since it was started
    mlxChrono.restart();            // restart the crono so that it triggers again later
    angle = mlx_1.readAngle();
    //angle = i/1;     //readAngle gives 10 * degrees, thus 3600 = is 360.0ยบ
    /* if i = -1 then no SPI signal
     * if i = -2 then signal too strong
     * if i = -3 then signal too weak
     * "angle" will read 0 if signal is lost
    */   
    //angle /= 10;
    if(angle > 0) {     
      angle /= 10;
      Serial.print("angle = ");
      Serial.println(angle);
    } else {      
      if (angle == -1) {
        Serial.println("No SPI signal");
      }
      if (angle == -2) {
        Serial.println("Signal too strong");
      }
      if (angle == -3) {
        Serial.println("Magnetic field too weak");
      }
    }      
  }

  //Serial.println("Pause 2 seconds...");
  delay(2000);                           
}


