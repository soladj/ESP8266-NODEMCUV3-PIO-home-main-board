#include <iostream>
#include "data.h"

#define DEVICE_ZONE 0 //0...15
#define DEVICE_ZONE_CHAR "0"

#define CODE_LENGTH 24

#define MQTT_SUB_TOPIC_NUMBER 3
#define MQTT_SUB_TOPIC_STORED 2
#define MQTT_PUB_TOPIC_NUMBER 3
#define MQTT_FLOAT_TOPIC_NUMBER 2
#define MQTT_ALARM_VAR_NUMBER 1

#define MQTT_MAIN_TOPIC "asshome"
#define MQTT_CALEFACTOR_TOPIC "calefactor"

#define MQTT_TMP_TOPIC "temperature"
#define MQTT_TSEL_TOPIC "temperaturesel"
#define MQTT_STATUS_TOPIC "status"
#define MQTT_REFRESH_TOPIC MQTT_MAIN_TOPIC "/" "refresh"
#define MQTT_HELLO_TOPIC MQTT_MAIN_TOPIC "/" "hello"
#define MQTT_HELLO_RESPONSE_TOPIC MQTT_MAIN_TOPIC "/" "hello_response"

#define MQTT_CALEFACTOR_TSEL_TOPIC MQTT_MAIN_TOPIC "/" MQTT_CALEFACTOR_TOPIC "/" MQTT_TSEL_TOPIC "/" DEVICE_ZONE_CHAR
#define MQTT_CALEFACTOR_TMP_TOPIC MQTT_MAIN_TOPIC "/" MQTT_CALEFACTOR_TOPIC "/" MQTT_TMP_TOPIC "/" DEVICE_ZONE_CHAR
#define MQTT_CALEFACTOR_STATUS_TOPIC MQTT_MAIN_TOPIC "/" MQTT_CALEFACTOR_TOPIC "/" MQTT_STATUS_TOPIC

#define MQTT_CALEFACTOR_TSEL_TOPIC_ENUM 1
#define MQTT_CALEFACTOR_TMP_TOPIC_ENUM 2
#define MQTT_CALEFACTOR_STATUS_TOPIC_ENUM 3

#define MQTT_CALEFACTOR_TSEL_VALUE_ALARM 2000
#define MQTT_CALEFACTOR_TMP_VALUE_ALARM 2000
#define MQTT_CALEFACTOR_POWER_VALUE_ALARM 1
#define MQTT_CALEFACTOR_STATUS_VALUE_ALARM 0

#define MQTT_CALEFACTOR_TSEL_NAME_ALARM MQTT_CALEFACTOR_TSEL_TOPIC
#define MQTT_CALEFACTOR_TMP_NAME_ALARM MQTT_CALEFACTOR_TMP_TOPIC
#define MQTT_CALEFACTOR_POWER_NAME_ALARM MQTT_CALEFACTOR_POWER_TOPIC
#define MQTT_CALEFACTOR_STATUS_NAME_ALARM MQTT_CALEFACTOR_STATUS_TOPIC

char main_topic[CODE_LENGTH+1];
uint8_t own_topic;

const char* mqtt_sub_topics[MQTT_SUB_TOPIC_NUMBER] = {
  MQTT_CALEFACTOR_TSEL_TOPIC,
  MQTT_CALEFACTOR_TMP_TOPIC,
  MQTT_CALEFACTOR_STATUS_TOPIC
};
int mqtt_sub_topics_int[MQTT_SUB_TOPIC_NUMBER] = {
  MQTT_CALEFACTOR_TSEL_TOPIC_ENUM,
  MQTT_CALEFACTOR_TMP_TOPIC_ENUM,
  MQTT_CALEFACTOR_STATUS_TOPIC_ENUM
};
const char* mqtt_pub_topics[MQTT_PUB_TOPIC_NUMBER] = {
  MQTT_CALEFACTOR_TSEL_TOPIC,
  MQTT_CALEFACTOR_TMP_TOPIC,
  MQTT_CALEFACTOR_STATUS_TOPIC
};
const char* mqtt_float_topics[MQTT_FLOAT_TOPIC_NUMBER] = {
  MQTT_CALEFACTOR_TSEL_TOPIC,
  MQTT_CALEFACTOR_TMP_TOPIC
};

uint16_t tsel;
uint16_t tmp = 2000;
uint16_t cpower;
uint16_t cstatus;

uint16_t *mqtt_sub_var[MQTT_SUB_TOPIC_NUMBER] = {
  &tsel,
  &tmp,
  &cstatus
};
const uint16_t *mqtt_pub_var[MQTT_PUB_TOPIC_NUMBER] = {
  &tsel,
  &tmp,
  &cstatus
};
uint16_t *mqtt_alarm_var[MQTT_ALARM_VAR_NUMBER] = {
  &cstatus
};
const uint16_t mqtt_alarm_val[MQTT_ALARM_VAR_NUMBER] = {
  MQTT_CALEFACTOR_STATUS_VALUE_ALARM
};
const char* mqtt_alarm_name[MQTT_ALARM_VAR_NUMBER] = {
  MQTT_CALEFACTOR_STATUS_NAME_ALARM
};


//Command config
#define MAX_COMMAND_COUNTER 7
#define MAX_COMMAND_LENGHT 16
#define MAX_VALUE_LENGHT 16

char commands[MAX_COMMAND_COUNTER][MAX_COMMAND_LENGHT];
char values[MAX_COMMAND_COUNTER][MAX_VALUE_LENGHT];
