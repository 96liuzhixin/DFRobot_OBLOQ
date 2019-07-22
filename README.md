# OBLOQ-Arduino
DFRobot_OBLOQ is a Internet of things connection module, which can send and receive data from MQTT and HTTP after connecting to wifi.

## DFRobot_OBLOQ Library for Arduino
---------------------------------------------------------
Provides a Arduino library for reading and sending OBLOQ data through I2C. <br>
OBLOQ module has two basic functions: sending data to the Internet of things and receiving Internet of things data.


## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

<snippet>
<content>

## Installation

To use this library download the zip file, uncompress it to a folder named DFRobot_OBLOQ. 
Download the zip file first to use this library and uncompress it to a folder named DFRobot_OBLOQ. 

## Methods

```C++
class DFRobot_OBLOQ{

 /*
  * @brief begin OBLOQ device
  *
  * @return ture  : Initialization success
  *         false : Initialization failure
  */
  bool begin();


 /*
  * @brief  WiFi initialization ,Set the wifi password and connect to the wifi
  *
  * @param Name is wifi name
  * 
  * @param pwd is wifi password
  * 
  * @return 0     : WiFi connect success
  *         1     : WiFi connect error
  *         2     : WiFi running connect
  *         5     : other state
  */
  uint8_t wifiBegin(uint8_t *Name = NULL, uint8_t *pwd = NULL);


 /*
  * @brief  Set up the server, port, id,pwd of mqtt and connect to mqtt
  *
  * @param server is Server Web site for DFRobot 
  * 
  * @param port is of DFRobot
  * 
  * @param id is DFRobot Iot_id(user)
  * 
  * @param pwd is DFRobot Iot_pwd(password)
  *
  * @return 0     : Mqtt connect success
  *         1     : Mqtt connect error
  *         2     : Mqtt Start
  *         5     : Other state
  */
  bool mqttBegin(uint8_t *server = NULL, uint8_t *port = NULL, uint8_t *id = NULL, uint8_t *pwd = NULL);


 /*
  * @brief  set the port and address for http 
  *
  * @param ip is Server ip address
  * 
  * @param port Server port
  *
  * @return ture  : param is normal
  *         false : param is empty
  */
  bool httpBegin(uint8_t *ip = NULL, uint8_t *port = NULL);


 /*
  * @brief send ping
  */
  void sendPing();


 /*
  * @brief reconnect wifi
  */
  void reConnectWifi();


 /*
  * @brief break connect wifi
  *
  * @return ture  : Wifi disconnected successfully 
  */
  bool disConnectWifi();


 /*
  * @brief Subscribe to topic ,Up to 5 topic
  *
  * @param Topic is The device name in DFRobot
  * 
  * @return 0     : Subscribe success
  *         1     : Subscribe error
  *         2     : Subscribe take upper limit
  *         3     : Subscribe take start
  *         5     : other state
  */
  uint8_t subscribe(uint8_t *Topic = NULL);


 /*
  * @brief Send message to topic ,Up to 5 topic can be sent at the same time
  *
  * @param Data is Data sent to topic
  * 
  * @param Topic is Send to topic name, And it's a topic that has been subscribed to.
  * 
  * @return ture  : param is normal
  *         false : param is empty or Num out of range
  */
  bool publish(String Data, String Topic);


 /*
  * @brief http get url
  *
  * @param GetUrl is URL sent
  *
  * @return RecvHttpData : Data returned by http 
  *                        data or Error code 
  */
  String httpGet(uint8_t *GetUrl = NULL);


 /*
  * @brief http post url
  *
  * @param PostUrl is sent URL
  * 
  * @param Data is sent data
  *
  * @return RecvHttpData : Data returned by http 
  *                        data or Error code 
  */
  String httpPost(String PostUrl, String Data);


 /*
  * @brief http put url
  *
  * @param PutUrl is URL sent
  *
  * @return RecvHttpData : Data returned by http 
  *                        data or Error code 
  */
  String httpPut(uint8_t *PutUrl = NULL);


 /*
  * @brief ping state
  *
  * @return 0           : ping success
  *         1           : ping error
  *         5           : other state
  */
  uint8_t pingState();


 /*
  * @brief Get hardware version 
  *
  * @return version        : hardware version
  */
  String getVersion();


 /*
  * @brief Get the IP address of the wifi
  *
  * @return WifiIp         : Wifi ip address 
  */
  String getWifiIp();


 /*
  * @brief Get Wifi State
  *
  * @return WifiState      : Wifi state 
  *         WIFI_NO_CON       is      wifi is disconnected
  *         WIFI_RUN_CON      is      Wifi is connecting 
  *         WIFI_SUCCESS_CON  is      The wifi connection was successful
  */
  uint8_t getWifiState();


 /*
  * @brief Set callback function
  *
  * @param *call is function name
  * 
  * @param (char *, uint8_t * ,uint8_t)  is The parameters of the callback function 
  */
  void setCallback(void (*call)(char *, uint8_t * ,uint8_t));


 /*
  * @brief Loop Read State
  *        Receiving the information sent by the topic is called back to the topic print function and prints the data 
  */
  void loop();
  
```
## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
FireBeetle-ESP32   |      √       |              |             | 
Arduino uno        |      √       |              |             | 

## History

- July 5, 2019 - Version 0.1 released.


## Credits

Written by Frank(zhixin.liu@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))