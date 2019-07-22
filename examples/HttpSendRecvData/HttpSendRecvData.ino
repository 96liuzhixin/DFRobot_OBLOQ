/*!
 * HttpSendRecvData.ino
 *
 * Download this demo to test config to OBLOQ, connect sensor through IIC interface
 * Data will print on your serial monitor
 *
 * This is an example of Http sending and receiving data.
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  05/07/2019
 */
#include "DFRobot_OBLOQ.h"

uint8_t * WIFI_NAME    = "hitest";
uint8_t * WIFI_PWD     = "12345678";
uint8_t * HTTP_ADDRESS = "192.168.1.102";
uint8_t * HTTP_PORT    = "8080";

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
  
  /* set the port and address for http 
   *     HTTP_ADDRESS is http address
   *     HTTP_PORT    is http port
   */
  while(!OBLOQ.httpBegin(HTTP_ADDRESS, HTTP_PORT)) {
    Serial.println("Parameter is empty!");
    delay(1000);
  }
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

  /* Http sends data as Get */
  String GetData =OBLOQ.httpGet("input?id=1&val=hello");
  Serial.println(GetData);
  delay(200);
  
  /* Http sends data as Post */
  String PostData = OBLOQ.httpPost("input?id=1&val=22", "{\"data\":\"123456789\"}|\r");
  Serial.println(PostData);
}