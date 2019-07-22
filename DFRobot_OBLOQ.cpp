/*

  MIT License

  Copyright (C) <2019> <@DFRobot zhixin.liu>


  Permission is hereby granted, free of charge, to any person obtaining a copy of this

  software and associated documentation files (the "Software"), to deal in the Software

  without restriction, including without limitation the rights to use, copy, modify,

  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to

  permit persons to whom the Software is furnished to do so.



  The above copyright notice and this permission notice shall be included in all copies or

  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,

  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR

  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE

  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,

  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */
 
 
#include "DFRobot_OBLOQ.h"
#include <Arduino.h>
#include <Wire.h>

DFRobot_OBLOQ::DFRobot_OBLOQ()
{
  this->_addr = 0x16;               //OBLOQ Device number fixed to 0x16
  this->_addr = 0x16;               //OBLOQ Device number fixed to 0x16
}

DFRobot_OBLOQ::~DFRobot_OBLOQ()
{
}

/* Join the I2C bus (master address fixed) */
bool DFRobot_OBLOQ::begin()
{
  Wire.begin();                     // connecting the i2c bus 
  Wire.beginTransmission(_addr);
  if(Wire.endTransmission() == 0)
    return true;
  return false;
}

/* WiFi initialization ,Set the wifi password and connect to the wifi */
uint8_t DFRobot_OBLOQ::wifiBegin(uint8_t *Name, uint8_t *pwd)
{
  if(_wifiName != Name) {
    _wifiName = Name;
    manageFunction(Name, OB_SET_COMMAND ,SET_WIFI_NAME);
    /* Commands are written to registers with data */
    manageFunction(pwd, OB_SET_COMMAND, SET_WIFI_PAD);
    connectWifi();
  }
  return readWifiState();
}

/* Set up the server, port, id,pwd of mqtt and connect to mqtt */
uint8_t DFRobot_OBLOQ::mqttBegin(uint8_t *Server, uint8_t *port ,uint8_t *id, uint8_t *pwd)
{
  if(_mqttID != id) {
    _mqttID = id;
    manageFunction(Server, OB_SET_COMMAND, SET_MQTT_SERVER);
    manageFunction(port, OB_SET_COMMAND, SET_MQTT_PORT);
    manageFunction(id, OB_SET_COMMAND, SET_MQTT_ID);
    manageFunction(pwd, OB_SET_COMMAND, SET_MQTT_PWD);
  
    connectMqtt();
  }
  return readMqttState();
}

/* set the port and address for http */
bool DFRobot_OBLOQ::httpBegin(uint8_t *ip, uint8_t *port)
{
  if((!ip) || (!port))
    return false;
  manageFunction(ip, OB_SET_COMMAND, SET_HTTP_ID);
  manageFunction(port, OB_SET_COMMAND, SET_HTTP_PORT);
  
  return true;
}

/* Send ping command */
void DFRobot_OBLOQ::sendPing()
{
  manageFunctionShort(OB_RUN_COMMAND ,SEND_PING);
}

/* connect wifi */
void DFRobot_OBLOQ::connectWifi()
{
  manageFunctionShort(OB_RUN_COMMAND ,CONNECT_WIFI);
}

/* reconnect wifi */
void DFRobot_OBLOQ::reConnectWifi()
{
  manageFunctionShort(OB_RUN_COMMAND ,RECONNECT_WIFI);
  while(1) {
    if(readWifiState() == 1)
      break;
  }
  _wifiState = MQTT_SUCCESS_CON;
}

  /* Disconnect wifi connection */
bool DFRobot_OBLOQ::disConnectWifi()
{
  manageFunctionShort(OB_RUN_COMMAND ,BREAK_WIFI_CON);
  while(1){
    if(readWifiState() == 1)
      break;
  }
  _wifiState = WIFI_NO_CON;
  return true;
}

/* connect Mqtt */
void DFRobot_OBLOQ::connectMqtt()
{
  manageFunctionShort(OB_RUN_COMMAND ,CONNECT_MQTT);
}


/* Subscribe to topic ,Up to 5 topic */
uint8_t DFRobot_OBLOQ::subscribe(uint8_t *Topic)
{
  static uint8_t num = 0;
  if(num > 5)
    return 6;

  if(_Topic != Topic) {
    _Topic = Topic;
    for(int i = 0; i < 5; i++) {
      if(_topicName[i] == Topic)
        return 7;
    }
    _topicName[num] = Topic;

    switch(num) {
      case 0:
        manageFunction(Topic, OB_RUN_COMMAND, SUBSCRIBE_TOPIC0);
        num++;
        break;
      case 1:
        manageFunction(Topic, OB_RUN_COMMAND, SUBSCRIBE_TOPIC1);
        num++;
        break;
      case 2:
        manageFunction(Topic, OB_RUN_COMMAND, SUBSCRIBE_TOPIC2);
        num++;
        break;
      case 3:
        manageFunction(Topic, OB_RUN_COMMAND, SUBSCRIBE_TOPIC3);
        num++;
        break;
      case 4:
        manageFunction(Topic, OB_RUN_COMMAND, SUBSCRIBE_TOPIC4);
        num++;
        break;
      default:
        return RETURN_NONE;
    }
  }
  return readTakeState();
}

/* Send message to topic ,Up to 5 topic can be sent at the same time */
bool DFRobot_OBLOQ::publish(String Data, String Topic)
{
  /* Determine whether the user has not passed a value */
  if((!Data) || (!Topic))
    return false;
  uint8_t num = 254;
  for(int i = 0; i < 5; i++) {
    if(_topicName[i] == Topic){
      num = i;
      break;
    }
  }
  switch(num){
    case 0:
      manageFunction((uint8_t* )Data.c_str(), OB_RUN_COMMAND, SEND_TOPIC0);
      break;
    case 1:
      manageFunction((uint8_t* )Data.c_str(), OB_RUN_COMMAND, SEND_TOPIC1);
      break;
    case 2:
      manageFunction((uint8_t* )Data.c_str(), OB_RUN_COMMAND, SEND_TOPIC2);
      break;
    case 3:
      manageFunction((uint8_t* )Data.c_str(), OB_RUN_COMMAND, SEND_TOPIC3);
      break;
    case 4:
      manageFunction((uint8_t* )Data.c_str(), OB_RUN_COMMAND, SEND_TOPIC4);
      break;
    default:
      return false;
  }
  return true;

}

/* http get url */
String  DFRobot_OBLOQ::httpGet(uint8_t *GetUrl)
{
  char recvHttpData[25] = {0x00};
  manageFunction(GetUrl, OB_RUN_COMMAND, HTTP_GET_URL);
  httpReturn(&recvHttpData[0]);
  String data = recvHttpData;
  return data;
}

/* http post url */
String  DFRobot_OBLOQ::httpPost(String PostUrl, String Data)
{
  char recvHttpData[25] = {0x00};
  String pData;
  pData  = PostUrl;
  pData += ',';
  pData += Data;
  manageFunction((uint8_t*)pData.c_str(), OB_RUN_COMMAND, HTTP_POST_URL_CON);
  
  httpReturn(&recvHttpData[0]);
  String data = recvHttpData;
  return data;
}

/* http put url */
String  DFRobot_OBLOQ::httpPut(uint8_t *PutUrl)
{
  char recvHttpData[25] = {0x00};
  manageFunction(PutUrl, OB_RUN_COMMAND, HTTP_PUT_URL_CON);
  httpReturn(&recvHttpData[0]);
  String data = recvHttpData;
  return data;

}

/* Read ping state */
uint8_t DFRobot_OBLOQ::pingState()
{
  uint8_t len = 2;
  uint8_t buffer[len] = {0x00};
  readRegister(OB_COMMAND_REGISTER, &buffer[0], len);

  if(buffer[0] == PING_STATUE){
    if(buffer[1] == PING_SUCCESS){
      return 0;
    }else{
      return 1;
    }
  }else{
    return 5;
  }
}

/* Read wifi connect state */
uint8_t DFRobot_OBLOQ::readWifiState()
{
  uint8_t len = 2;
  uint8_t buffer[len] = {0x00};
  readRegister(OB_COMMAND_REGISTER, &buffer[0], len);

  if(buffer[0] == WIFI_STATUE_CON){
    if(buffer[1] == WIFI_SUCCESS_CON){      //wifi connect 
      readWifiIp();
      _wifiState = WIFI_SUCCESS_CON;
      return 0;
    }else if(buffer[1] == WIFI_NO_CON){     //wifi connect false
      _wifiState = WIFI_NO_CON;
      return 1;
    }else if(buffer[1] == WIFI_RUN_CON){    //Connecting to the WiFi
      _wifiState = WIFI_RUN_CON;
      return 2;
    }
  }else 
    return 5;
}

/* Read the IP address of the wifi. */
void DFRobot_OBLOQ::readWifiIp()
{
  char ipdata[16] = {0x00};
  if(readRegisterData(&ipdata[0],IP_ADDRESS))   //Normal return wifi ip
    _wifiIP = ipdata;
}

/* Get the IP address of the wifi. */
String DFRobot_OBLOQ::getWifiIp()
{
  return _wifiIP;
}

/* Get hardware version */
String DFRobot_OBLOQ::getVersion()
{
  char getVersionData[5] = {0x00};
  manageFunctionShort(OB_RUN_COMMAND ,QUERY_VERSION);
  delay(10);
  readVersion(&getVersionData[0]);
  delay(10);
  String Version = getVersionData;
  return Version;
}

/* Get Wifi State */
uint8_t DFRobot_OBLOQ::getWifiState()
{
  return _wifiState;
}

/* Read wifi connect state */
uint8_t DFRobot_OBLOQ::readMqttState()
{
  uint8_t len = 2;
  uint8_t buffer[len] = {0x00};
  readRegister(OB_COMMAND_REGISTER, &buffer[0], len);
  
  if(buffer[0] == MQTT_STATUE_CON){
    if(buffer[1] == MQTT_SUCCESS_CON){
      return 0;
    }else if(buffer[1] == MQTT_ERROR_CON){
      return 1;
    }else if(buffer[1] == MQTT_START){
      return 2;
    }
  }else if((buffer[0] == NONE)){            //Normal state
    return RETURN_NONE;
  }else{
    return 5;                               //other state
  }
}

/* read Topic Subscription Status */
uint8_t DFRobot_OBLOQ::readTakeState()
{
  uint8_t len = 2;
  uint8_t buffer[len] = {0x00};
  readRegister(OB_COMMAND_REGISTER, &buffer[0], len);

  if(buffer[0] == SUBSCRIBE_STATUE){
    /* Subscription Status */
    if(buffer[1] == TAKE_SUCCESS){
      return 0;
    }else if(buffer[1] == TAKE_ERROR){
      return 1;
    }else if(buffer[1] == TAKE_UPPER_LIMIT){
      return 2;
    }else if(buffer[1] == TAKE_START){
      return 3;
    }
  }else if((buffer[0] == NONE)){
    return RETURN_NONE;
  }else{
    return 5;
  }
}

/* Http return data */
void DFRobot_OBLOQ::httpReturn(char * Data)
{
  readRegisterData(&Data[0], HTTP_NORMAL_RETURN, HTTP_ERROR_RETURN);
}

/* Read the hardware version */
void DFRobot_OBLOQ::readVersion(char * getVersionData)
{
  readRegisterData(&getVersionData[0], GET_VERSION);
}

/* Command write register */
void DFRobot_OBLOQ::manageFunctionShort(uint8_t config, uint8_t command)
{
  uint8_t buf[1];
  buf[0] = config;
  buf[1] = command;
  I2CWriteData(OB_COMMAND_REGISTER, &buf[0], 2);
}

/* Commands are written to registers with data */
void DFRobot_OBLOQ::manageFunction(uint8_t *data, uint8_t command, uint8_t config)
{
  uint8_t i = 0, j = 0;
  while(data[i])                        //Get data length
    i++;
  uint8_t datalen = i + 3;              //overall length
  uint8_t buf[datalen] = {0x00};
  buf[0]  = command;
  buf[1]  = config;
  buf[2]  = datalen;
  for(i = 3; i < datalen;)
    buf[i++] = data[j++];
  I2CWriteData(OB_COMMAND_REGISTER, &buf[0], datalen);
}

/* Write data to an I2C register */
void DFRobot_OBLOQ::I2CWriteData(uint8_t Reg, uint8_t *pdata, uint8_t datalen)
{
  Wire.beginTransmission(_addr);       // transmit to device #8
  Wire.write(Reg);                     // sends one byte
    for (uint8_t i = 0; i < datalen; i++) 
      Wire.write(pdata[i]);
  Wire.endTransmission();              // stop transmitting
}

/* Normal reading of I2c register data */
void DFRobot_OBLOQ::readRegister(uint8_t registaddress, uint8_t* buf, uint8_t len)
{
  uint8_t i = 0;

  Wire.beginTransmission(_addr);       // transmit to device Address
  Wire.write(registaddress);           // sends one byte
  Wire.write(OB_READ_COMMAND);         
  Wire.write(READ_COMMAND);         
  Wire.endTransmission();              // stop transmitting
  Wire.requestFrom(_addr, len);
 
  while (Wire.available())             // slave may send less than requested
    buf[i++] = Wire.read();

}

/* Read twice, the first read length len, reads the len size data the second time */
bool DFRobot_OBLOQ::readRegisterData(uint8_t * buf, uint8_t config, uint8_t config1)
{
  uint8_t i = 0, j = 0;
  uint8_t buffer[2] = {0x00};
  /* Get data length */
  Wire.beginTransmission(_addr);
  Wire.write(OB_COMMAND_REGISTER);
  Wire.write(OB_READ_COMMAND);
  Wire.write(READ_COMMAND);
  Wire.endTransmission();
  Wire.requestFrom(_addr, (uint8_t)2);
  while(Wire.available()){
    buffer[i++] = Wire.read();
  }
  /* Get the data length and re-get the data */
  if(buffer[0] == config){
    Wire.beginTransmission(_addr);
    Wire.write(READ_DATA_REGISTER);
    Wire.endTransmission();
    Wire.requestFrom(_addr, buffer[1]);
    while(Wire.available()){              //data
      buf[j++] = Wire.read();
    }
    buf[j] = '\0';
    return true;
  }else if (buffer[0] == config1){        //http error number
    buf[0] = buffer[1];
    return false;
  }else{
    return false;
  }
}

/* Read the data returned by topic */
void DFRobot_OBLOQ::readData(uint8_t * data ,uint8_t len)
{
  uint8_t j = 0;
    Wire.beginTransmission(_addr);
    Wire.write(READ_DATA_REGISTER);
    Wire.endTransmission();
    Wire.requestFrom(_addr, len);
    while(Wire.available())              //data
      data[j++] = Wire.read();
    data[j] = '\0';
}

/* Set callback function */
void DFRobot_OBLOQ::setCallback(void (*call)(char *, uint8_t*, uint8_t))
{
  this->callback = call;
}

/* Loop Read State */
void DFRobot_OBLOQ::loop()
{
  uint8_t i = 0;
  uint8_t buffer[2] = {0,0};
  /* Get data length */
  Wire.beginTransmission(_addr);
  Wire.write(OB_COMMAND_REGISTER);
  Wire.write(OB_READ_COMMAND);
  Wire.write(READ_COMMAND);
  Wire.endTransmission();
  Wire.requestFrom(_addr, (uint8_t)2);
  while(Wire.available()){
    buffer[i++] = Wire.read();
  }

  /* state */
  if(buffer[0] == NONE){
  }else if(buffer[0] == PING_STATUE){
  }else if(buffer[0] == WIFI_STATUE_CON){
    _wifiState = buffer[1];
  }else if(buffer[0] == IP_ADDRESS){
  }else if(buffer[0] == MQTT_STATUE_CON){
  }else if(buffer[0] == SUBSCRIBE_STATUE){
  }else if(buffer[0] == RECV_TOPIC0){
    uint8_t data[buffer[1]];
    readData(&data[0], buffer[1]);
    if(callback)
      callback((char *)(_topicName[0].c_str()),&data[0],buffer[1]);
  }else if(buffer[0] == RECV_TOPIC1){
    uint8_t data[buffer[1]];
    readData(&data[0], buffer[1]);
    if(callback)
      callback((char *)(_topicName[1].c_str()),&data[0],buffer[1]);
  }else if(buffer[0] == RECV_TOPIC2){
    uint8_t data[buffer[1]];
    readData(&data[0], buffer[1]);
    if(callback)
      callback((char *)(_topicName[2].c_str()),&data[0],buffer[1]);
  }else if(buffer[0] == RECV_TOPIC3){
    uint8_t data[buffer[1]];
    readData(&data[0], buffer[1]);
    if(callback)
      callback((char *)(_topicName[3].c_str()),&data[0],buffer[1]);
  }else if(buffer[0] == RECV_TOPIC4){
    uint8_t data[buffer[1]];
    readData(&data[0], buffer[1]);
    if(callback)
      callback((char *)(_topicName[4].c_str()),&data[0],buffer[1]);
  }else if(buffer[0] == HTTP_NORMAL_RETURN){
  }else if(buffer[0] == HTTP_ERROR_RETURN){
  }else if(buffer[0] == GET_VERSION){
  }else{
    return;
  }
 
}









