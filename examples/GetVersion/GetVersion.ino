/*!
 * GetVersion.ino
 *
 * Download this demo to test config to OBLOQ, connect sensor through IIC interface
 * Data will print on your serial monitor
 *
 * This example is the version information for obtaining the OBLOQ.
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  05/07/2019
 */
#include "DFRobot_OBLOQ.h"

DFRobot_OBLOQ OBLOQ;

void setup() {

  Serial.begin(9600);
 
  /* Initialize i2c */
  while(!OBLOQ.begin()) {
    Serial.println("I2c Device Number ERROR");
    delay(100);
  }
  Serial.println("I2c Connect SUCCESS");

  Serial.print("The Version is ");
  Serial.println(OBLOQ.getVersion());            // Get the hardware version number.
}

void loop() {
  delay(1000);
}