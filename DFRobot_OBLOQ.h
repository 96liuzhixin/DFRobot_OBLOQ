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
#include<Wire.h>
#include<Arduino.h>


#define    OB_COMMAND_REGISTER  0x1E             // command register 
/* command register */
#define    OB_RUN_COMMAND       0x02             // Obloq run  command
#define    OB_READ_COMMAND      0x00             // Obloq read command
#define    OB_SET_COMMAND       0x01             // Obloq set  command

#define    OB_TYPE_REGISTER     0x1F             // Parameter type register 
/* Set the type of parameter  0x1E---0x01*/
#define    SET_WIFI_NAME        0x01
#define    SET_WIFI_PAD         0x02
#define    SET_MQTT_SERVER      0x03
#define    SET_MQTT_PORT        0x04
#define    SET_MQTT_ID          0x05
#define    SET_MQTT_PWD         0x06
#define    SET_HTTP_ID          0x07
#define    SET_HTTP_PORT        0x08
/* Set the type of parameter  0x1E---0x02*/
#define    SEND_PING            0x01
#define    CONNECT_WIFI         0x02
#define    RECONNECT_WIFI       0x03
#define    BREAK_WIFI_CON       0x04
#define    CONNECT_MQTT         0x05
#define    SUBSCRIBE_TOPIC0     0x06
#define    SUBSCRIBE_TOPIC1     0x07
#define    SUBSCRIBE_TOPIC2     0x08
#define    SUBSCRIBE_TOPIC3     0x09
#define    SUBSCRIBE_TOPIC4     0x0A
#define    SEND_TOPIC0          0x0B
#define    SEND_TOPIC1          0x0C
#define    SEND_TOPIC2          0x0D
#define    SEND_TOPIC3          0x0E
#define    SEND_TOPIC4          0x0F
#define    HTTP_GET_URL         0x10
#define    HTTP_POST_URL_CON    0x11
#define    HTTP_PUT_URL_CON     0x12
#define    QUERY_VERSION        0x13
/* Set the type of parameter  0x1E---0x02*/
#define    READ_COMMAND         0x06             // Read command 


#define    OB_LEN_REGISTER      0x20             // Parameter length register 
/* The parameter type of the return value  */
#define    NONE                 0x00
#define    PING_STATUE          0x01
#define    WIFI_STATUE_CON      0x02
#define    IP_ADDRESS           0x03
#define    MQTT_STATUE_CON      0x04
#define    SUBSCRIBE_STATUE     0x05
#define    SUBSCRIBE_TOPIC0     0x06
#define    SUBSCRIBE_TOPIC1     0x07
#define    SUBSCRIBE_TOPIC2     0x08
#define    SUBSCRIBE_TOPIC3     0x09
#define    SUBSCRIBE_TOPIC4     0x0A
#define    HTTP_NORMAL_RETURN   0x10
#define    HTTP_ERROR_RETURN    0x11
#define    GET_VERSION          0x12


/* Data Register */
#define    READ_LEN_REGISTER    0x21
#define    READ_DATA_REGISTER   0x22

#define    PING_SUCCESS         0x01
#define    PING_ERROR           0x00

#define    WIFI_NO_CON          0x00
#define    WIFI_RUN_CON         0x02
#define    WIFI_SUCCESS_CON     0x03

#define    MQTT_START           0x00
#define    MQTT_SUCCESS_CON     0x01
#define    MQTT_ERROR_CON       0x02

#define    TAKE_START           0x00
#define    TAKE_SUCCESS         0x01
#define    TAKE_UPPER_LIMIT     0x02
#define    TAKE_ERROR           0x03


#define    RECV_TOPIC0          0x06
#define    RECV_TOPIC1          0x07
#define    RECV_TOPIC2          0x08
#define    RECV_TOPIC3          0x09
#define    RECV_TOPIC4          0x0A

#define    RETURN_NONE          0x0A



class DFRobot_OBLOQ{
  public:
    DFRobot_OBLOQ();
    ~DFRobot_OBLOQ();

    /* Initialize i2c */
    bool    begin();
  
    /* WiFi initialization ,Set the wifi password and connect to the wifi */
    uint8_t wifiBegin(uint8_t *Name = NULL, uint8_t *pwd = NULL);

    /* Set up the server, port, id,pwd of mqtt and connect to mqtt */
    uint8_t mqttBegin(uint8_t *Server = NULL, uint8_t* port = NULL, uint8_t *id = NULL, uint8_t *pwd = NULL);
  
    /* set the port and address for http */
    bool    httpBegin(uint8_t *ip = NULL, uint8_t *port = NULL);

    /* send ping */
    void    sendPing();

    /* reconnect wifi */
    void    reConnectWifi();

    /* Disconnect wifi connection */
    bool    disConnectWifi();

    /* Subscribe to topic ,Up to 5 topic */
    uint8_t subscribe(uint8_t *Topic = NULL);

    /* Send message to topic ,Up to 5 topic can be sent at the same time */
    bool    publish(String Data, String Topic);

    /* http get url */
    String  httpGet(uint8_t *GetUrl = NULL);

    /* http post url */
    String  httpPost(String PostUrl, String Data);

    /* http put url */
    String  httpPut(uint8_t *PutUrl = NULL);

    /* ping state */
    uint8_t pingState();

    /* Get the IP address of the wifi */
    String  getWifiIp();

    /* Get hardware version */
    String  getVersion();

    /* Get Wifi State */
    uint8_t getWifiState();

    /* Loop Read State */
    void    loop();

    /* Set callback function */
    void    setCallback(void (*call)(char *, uint8_t * ,uint8_t));

  private:

    uint8_t _addr;                           // I2c device number
    uint8_t _wifiState    = 254;             // Wifi State
    String  _topicName[5] = {"0"};           // Topic name
    String  _wifiIP       = "0";             // Wifi Ip Address
    String  _wifiName     = "0";             // Wifi Name
    String  _mqttID       = "0";             // Mqtt Id
    String  _Topic        = "0";             // Topic 
  
    /* Callback function */
    void    (* callback)(uint8_t * topic , uint8_t * payload , uint8_t len);

    /* Command write register */
    void    manageFunctionShort(uint8_t config, uint8_t command);

    /* Commands are written to registers with data */
    void    manageFunction(uint8_t *data, uint8_t command, uint8_t config);

    /* connect Mqtt */
    void    connectMqtt();

    /* connect wifi */
    void    connectWifi();

    /* Http return data */
    void    httpReturn(char * Data);

    /* Read wifi connect state */
    uint8_t readWifiState();

    /* Read Mqtt connect state */
    uint8_t readMqttState();

    /* Read Topic Subscription Status */
    uint8_t readTakeState();

    /* Write data to an I2C register */
    void    I2CWriteData(uint8_t Reg, uint8_t *pdata, uint8_t datalen);

    /* Read the data returned by topic */
    void    readData(uint8_t * data ,uint8_t len);

    /* Read the hardware version */
    void    readVersion(char * getVersionData);

    /* Read the IP address of the wifi. */
    void    readWifiIp();

    /* Normal reading of I2c register data */
    void    readRegister(uint8_t registaddress, uint8_t* buf, uint8_t len);

    /* Read twice, the first read length len, reads the len size data the second time */
    bool    readRegisterData(uint8_t * buf, uint8_t config , uint8_t config1 = 0xFF);  

};























