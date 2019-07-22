/*!
 * GetWifiIp.ino
 *
 * Download this demo to test config to OBLOQ, connect sensor through IIC interface
 * Data will print on your serial monitor
 *
 * This example is to set the wifi name and password, and get the IP address after the connection is successful.
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  05/07/2019
 */
#include "DFRobot_OBLOQ.h"

uint8_t * WIFI_NAME = "hitest";
uint8_t * WIFI_PWD  = "12345678";

DFRobot_OBLOQ OBLOQ;

void setup() {
  Serial.begin(9600);
  /* Initialize i2c */
  while(!OBLOQ.begin()) {
    Serial.println("I2c Device Number ERROR");
    delay(1000);
  }
  Serial.println("I2c Connect SUCCESS");
  
  /* WiFi initialization ,Set the wifi password and connect to the wifi 
   *      WIFI_NAME is wifi name
   *      WIFI_PWD  is wifi password
   */
  while(OBLOQ.wifiBegin(WIFI_NAME, WIFI_PWD) != 0) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Wifi Connect SUCCESS");
  
  
/*
  while(OBLOQ.DisConnectWifi()) {                // disconnect Wifi
    Serial.print(".");
  }
  Serial.println("Wifi disconnected successfully! ");
*/
}
  
void loop() {

 /* If the wifi is disconnected, reconnect the wifi. 
  *     WIFI_NO_CON       is      wifi is disconnected
  *     WIFI_RUN_CON      is      Wifi is connecting 
  *     WIFI_SUCCESS_CON  is      The wifi connection was successful 
  */
  if(WIFI_NO_CON == OBLOQ.getWifiState())
    OBLOQ.reConnectWifi();                       // Reconnect the wifi 

  OBLOQ.loop();                                  // Cyclic read state 
  Serial.print("The Wifi Ip is ");
  Serial.println(OBLOQ.getWifiIp());             // Get the wifi ip address

  delay(2000);
}