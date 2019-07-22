/*!
 * MqttSendRecvData.ino
 *
 * Download this demo to test config to OBLOQ, connect sensor through IIC interface
 * Data will print on your serial monitor
 *
 * This is an example of MQTT sending and receiving data.
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  05/07/2019
 */
#include "DFRobot_OBLOQ.h"

uint8_t * WIFI_NAME        = "hitest";
uint8_t * WIFI_PWD         = "12345678";
uint8_t * MQTT_SERVER      = "iot.dfrobot.com.cn";
uint8_t * MQTT_PORT        = "1883";
uint8_t * MQTT_ID          = "4hsxCcSWR";
uint8_t * MQTT_PWD         = "V2sxjcSZRz";
uint8_t * SUBSCRIBE_TOPIC  = "tS-kktSZg";
uint8_t * PUBLISH_TOPIC    = "tS-kktSZg";

DFRobot_OBLOQ OBLOQ;

void callback(char * topic , uint8_t * payload, uint8_t len)
{
  Serial.print("Receive [ ");
  Serial.print(topic);
  Serial.print(" ]");
  for(int i = 0; i < len; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

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
  
  /* Set the server port id and password for MQTT
   *         Server   is  Address of the Internet of things server
   *         port     is  Port of the Internet of Things server. 
   *         id       is  DFRobot Iot_id(user)
   *         password is  DFRobot Iot_pwd(password)
   */
  while(OBLOQ.mqttBegin(MQTT_SERVER, MQTT_PORT, MQTT_ID, MQTT_PWD) != 0) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Mqtt Connect SUCCESS");

  /* Subscribe to topic ,SUBSCRIBE_TOPIC is The device name in DFRobot */
  while(OBLOQ.subscribe(SUBSCRIBE_TOPIC)!= 0) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Subscribe Topic SUCCESS");
  
  /* Set callback function*/
  OBLOQ.setCallback(callback);
}

void loop() { 

 /* If the wifi is disconnected, reconnect the wifi. 
  *     WIFI_NO_CON       is      wifi is disconnected
  *     WIFI_RUN_CON      is      Wifi is connecting 
  *     WIFI_SUCCESS_CON  is      The wifi connection was successful 
  */
  if(WIFI_NO_CON == OBLOQ.getWifiState())
    OBLOQ.reConnectWifi();                       // Reconnect the wifi 

  /* Send data to topic ,PUBLISH_TOPIC is The device name in DFRobot. And it's a topic that has been subscribed to. */
  OBLOQ.publish("hello_world",PUBLISH_TOPIC); 
  OBLOQ.loop();                                  // Cyclic read state 
  delay(500);
}