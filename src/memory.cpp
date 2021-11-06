#include <Arduino.h>
#include <EEPROM.h>
#include "memory.h"

void eeprom_init(int n_var) {
    EEPROM.begin(sizeof(uint8_t)*n_var);
}

uint16_t read_memory(int dir)
{
  uint16_t value;
  value = EEPROM.read(dir*2)&0xFF;
  value <<= 8;
  value |= EEPROM.read(dir*2+1)&0xFF;
  return value;
}

uint8_t read_memory_byte(int dir)
{
  return EEPROM.read(dir);
}

void read_memory_string(char* res, int dir, int len)
{
  char *pres=res;
  int cdir=dir;
  // Serial.print("Reading memory: ");
  for (int i=0; i<len; i++)
  {
    *(pres++) = (char) read_memory_byte(cdir++);
    // Serial.print(read_memory_byte(cdir-1));
  }
  // Serial.println("");
}

void update_memory(int dir, uint16_t value)
{
  if (read_memory(dir) != value) {
    uint16_t value_tmp = value;
    EEPROM.write(dir*2, (uint8_t) (value_tmp>>8)&0xFF);
    EEPROM.write(dir*2+1, (uint8_t) value_tmp&0xFF);
    EEPROM.commit();
  }
}

void update_memory_byte(int dir, uint8_t value)
{
  if (read_memory_byte(dir) != value) {
    // Serial.print(value);
    EEPROM.write(dir, value);
    EEPROM.commit();
  }
}

void update_memory_string(int dir, char* data, int len)
{
  int cdir=dir;
  // Serial.print("Update memory: ");
  for (int i=0; i<len; i++)
  {
    update_memory_byte(cdir++, (uint8_t) *(data++));
  }
  // Serial.println("");
}

void read_eeprom_ssid(char* res) {
  read_memory_string(res, EEPROM_WIFI_SSID_DIR, EEPROM_WIFI_SSID_LEN);
  if (res[0]==0 || res[0]==255) strcpy(res, "SSID");
}

void update_eeprom_ssid(char* data) {
  update_memory_string(EEPROM_WIFI_SSID_DIR, data, EEPROM_WIFI_SSID_LEN);
}

void read_eeprom_pass(char* res) {
  read_memory_string(res, EEPROM_WIFI_PASS_DIR, EEPROM_WIFI_PASS_LEN);
  if (res[0]==0 || res[0]==255) strcpy(res, "PASS");
}

void update_eeprom_pass(char* data) {
  update_memory_string(EEPROM_WIFI_PASS_DIR, data, EEPROM_WIFI_PASS_LEN);
}

void read_eeprom_main_topic(char* res) {
  read_memory_string(res, EEPROM_MAIN_TOPIC_DIR, EEPROM_MAIN_TOPIC_LEN);
  if (res[0]==0 || res[0]==255) strcpy(res, "PASS");
}

void update_eeprom_main_topic(char* data) {
  update_memory_string(EEPROM_MAIN_TOPIC_DIR, data, EEPROM_MAIN_TOPIC_LEN);
}
