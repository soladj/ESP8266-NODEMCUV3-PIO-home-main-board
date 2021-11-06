#include <Arduino.h>
#include <string.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "conf.h"
#include "code.h"
#include "memory.h"

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
char wifi_ssid[EEPROM_WIFI_SSID_LEN];
char wifi_pass[EEPROM_WIFI_PASS_LEN];

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

boolean connection_status;
boolean prev_connection_status;

int wifi_status;
int wifi_status_list[] = {WL_CONNECTED, WL_NO_SSID_AVAIL};
uint8_t wifi_status_len = 2;
int array_find(int arr[], int len, int seek)
{
    for (int i = 0; i < len; ++i)
    {
        if (arr[i] == seek) return i;
    }
    return -1;
}

WiFiServer server(8080);
bool ESP_SERVER = false;
bool alreadyConnected = 0;

unsigned hello_time;

void setup_WIFI(){
  Serial.begin(115200);

  Serial.print("Trying to connect to ssid: ");
  Serial.println(wifi_ssid);
  Serial.print("Pass: ");
  Serial.println(wifi_pass);
  WiFi.begin(wifi_ssid, wifi_pass);

  Serial.print("Conectando");
  do
  {
    delay(500);
    Serial.print(".");
    wifi_status = WiFi.status();
    Serial.print(wifi_status);
  } while (array_find(wifi_status_list, wifi_status_len, wifi_status) == -1);
  Serial.println();

  if (WiFi.status() == WL_NO_SSID_AVAIL)
  {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(INIT_SSID, INIT_PASSWORD);
    Serial.print("Set AP mode\n");
    ESP_SERVER=true;
    server.begin();
    IPAddress myIP = WiFi.softAPIP(); //Get IP address
    Serial.print("Get IP: ");
    Serial.println(myIP);
  }
  else
  {
    Serial.print("Conectado, dirección IP: ");
    Serial.println(WiFi.localIP());
  }
}

bool string_in_float_topic_array(char* value) {
  bool ret = false;
  for (int i=0;i<MQTT_FLOAT_TOPIC_NUMBER;i++)
  {
    if (strcmp(value, mqtt_float_topics[i])==0)
    {
      ret = true;
    }
  }
  return ret;
}

void pub_topics_refresh() {
  char num_char[6];
  for(uint8_t i=0;i<MQTT_PUB_TOPIC_NUMBER;i++) {
    if (string_in_float_topic_array((char*)mqtt_pub_topics[i]))
    {
      sprintf(num_char, "%.2f", ((double)*mqtt_pub_var[i])/100);
    }
    else 
    {
      sprintf(num_char, "%d", (int)*mqtt_pub_var[i]);
    }
    
    mqtt_client.publish(mqtt_pub_topics[i], num_char);
    Serial.print("Message published [");
    Serial.print(mqtt_pub_topics[i]);
    Serial.print("] ");
    Serial.print(num_char);
    Serial.println();
  }
}

void main_topic_generator()
{
  Serial.print("CODE: ");
  char str_var[CODE_LENGTH + 1];
  main_topic = get_code(str_var, timeClient.getEpochTime(), CODE_LENGTH);
  Serial.println(str_var);
}

void pub_topics_hello() {
    hello_time = timeClient.getEpochTime();
    mqtt_client.publish(MQTT_HELLO_TOPIC, hello_time);
    Serial.println("Send hello topic");
  }
}

void pub_topics_hello_response() {
    own_topic = false;
    mqtt_client.publish(MQTT_HELLO_RESPONSE_TOPIC, hello_time);
    Serial.println("Send hello topic response");
  }
}

void engine_hello_response(unsigned ht) {
    if (ht != hello_time)
    {
      Serial.print("CONGRATULATIONS. YOUR NEW MAIN TOPIC IS ");
      Serial.println(main_topic);
    }
    else if (own_topic==false) own_topic=true;
    else
    {
      Serial.println("MAIN TOPIC REPEATED. CREATE ANOTHER ONE.");
      main_topic_generator();
    }
  }
}

void set_alarm_var() {
  for(uint8_t i=0;i<MQTT_ALARM_VAR_NUMBER;i++) {
    *mqtt_alarm_var[i] = (uint16_t) mqtt_alarm_val[i];
    
    Serial.print("Set alarm value [");
    Serial.print(mqtt_alarm_name[i]);
    Serial.print("] ");
    Serial.print(*mqtt_alarm_var[i]);
    Serial.println();
  }
}

void callback(char* topic, unsigned char* payload, unsigned int length) {
  char data[length+1];
  // sprintf(data, "%s", payload);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    data[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  };
  data[length] = 0;
  Serial.println();

  if (strcmp(topic, MQTT_REFRESH_TOPIC)==0){
    pub_topics_refresh();
  }
  else if (strcmp(topic, MQTT_HELLO_TOPIC)==0){
    pub_topics_hello_response();
  }
  else if (strcmp(topic, MQTT_HELLO_RESPONSE_TOPIC)==0){
    engine_hello_response();
  }
  else{
    for(uint8_t i=0; i<MQTT_SUB_TOPIC_NUMBER;i++) {
      if (strcmp(topic, mqtt_sub_topics[i])==0) {
        if (string_in_float_topic_array(topic))
        {
          *mqtt_sub_var[i] = (uint16_t) (atof(data)*100);
          Serial.println(data);
          Serial.println(atof(data));
          Serial.println(atof(data)*100);
          Serial.println((uint16_t) (atof(data)*100));
        }
        else 
        {
          *mqtt_sub_var[i] = atoi(data);
        }
        switch(mqtt_sub_topics_int[i]) {
          case MQTT_CALEFACTOR_TSEL_TOPIC_ENUM:
            // *mqtt_sub_var[i] = update_eeprom_temperature_sel(*mqtt_sub_var[i]);
            break;
        }
      }
    }
  }
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.print(" clientID=");
    Serial.print(clientId);
    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish(mqtt_topic, "hello world");
      // ... and resubscribe
      mqtt_client.subscribe("asshome/refresh");
        Serial.print("Subscribe to topic: ");
        Serial.println("asshome/refresh");
      for(uint8_t i=0;i<(sizeof(mqtt_sub_topics)/sizeof(*mqtt_sub_topics));i++) {
        mqtt_client.subscribe(mqtt_sub_topics[i]);
        Serial.print("Subscribe to topic: ");
        Serial.println(mqtt_sub_topics[i]);
      }
      
    } else {
      Serial.print(" failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup_comm()
{
  eeprom_init(EEPROM_WIFI_PASS_LEN + EEPROM_WIFI_SSID_LEN);
  read_eeprom_ssid(wifi_ssid);
  read_eeprom_pass(wifi_pass);
  delay(1000);

  setup_WIFI();

  if (ESP_SERVER){}
  else
  {
    // Initialize a NTPClient to get time
    timeClient.begin();
    timeClient.setTimeOffset(0);
    timeClient.update();
    Serial.print("EPOCH_TIME: ");
    Serial.println(timeClient.getEpochTime());
    Serial.print("CODE: ");
    char str_var[CODE_LENGTH + 1];
    get_code(str_var, timeClient.getEpochTime(), CODE_LENGTH);
    Serial.println(str_var);

    // Serial.printf("CODE_GENERATE: %s ", get_code(timeClient.getEpochTime()));

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    mqtt_client.setCallback(callback);
    mqtt_reconnect();

    pub_topics_refresh();
    connection_status = prev_connection_status = true;
  }
}

void set_variable(char* var, char* varv)
{
  enum variableEnum {null, ssid, pass};
  variableEnum var_index=null;
  if (!strcmp(var, "ssid")) var_index=ssid;
  else if (!strcmp(var, "pass")) var_index=pass;
  switch (var_index)
  {
    case null:
      break;
    case ssid:
      Serial.print("SSID parameter value change to ");
      Serial.println(varv);
      update_eeprom_ssid(varv);
      break;
    case pass:
      Serial.print("PASS parameter value change to ");
      Serial.println(varv);
      update_eeprom_pass(varv);
      break;
  }
}

void loop_comm() {
  bool ret = false;

  if (ESP_SERVER)
  {
    espClient = server.available();
    if (espClient) {
      delay(500);
      Serial.println("We have a new client");
      espClient.println("Hello, client!");
      while (espClient.status()) 
      {
        int client_available = espClient.available();
        if (client_available != 0)
        {
          char input_data;
          char input_buffer[128];
          char command[3];
          char variable[16];
          char variable_value[64];
          char *pibuffer = input_buffer;
          for (int i=0; i<client_available; i++)
          {
            input_data = (char) espClient.read();
            // *(pibuffer++) = input_data;
            if (input_data==';' or input_data=='\r' or input_data=='\n')
              {
                Serial.print("Received command: ");
                Serial.println(input_buffer);
                *pibuffer=0;
                if (input_buffer[0]=='X')
                  {
                    sscanf(input_buffer, "X %s %s %s", command, variable, variable_value);
                  }
                  if (!strcmp(command, "cf")) set_variable(variable, variable_value);
                pibuffer=input_buffer;
              }
            else {*(pibuffer++)=input_data;}
          }
        }
      }
      Serial.println("Client disconnected.");
    }
  }
  else
  {
    connection_status = mqtt_client.connected();
    if (connection_status) {
      if (prev_connection_status != connection_status) {
        pub_topics_refresh();
      }
      // ret |= loop_tmpsensor(&tmp, &tsel, &cstatus, DEVICE_ZONE);
      
      if (ret) 
      {
        pub_topics_refresh();
      }
      mqtt_client.loop();
    }
    else {
      if (prev_connection_status != connection_status) {
        set_alarm_var();
      }
      mqtt_reconnect();
    }
    prev_connection_status = connection_status;
  }
}