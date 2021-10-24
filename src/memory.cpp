#include <EEPROM.h>

#define EEPROM_WIFI_SSID_DIR 0
#define EEPROM_WIFI_SSID_LEN 32
#define EEPROM_WIFI_PASS_DIR EEPROM_WIFI_SSID_DIR+EEPROM_WIFI_SSID_LEN
#define EEPROM_WIFI_PASS_LEN 32

void eeprom_init(int n_var) {
    EEPROM.begin(sizeof(uint16_t)*n_var);
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

char* read_memory_string(int dir, int len)
{
  char res[len];
  char *pres=res;
  int cdir=dir;
  for (int i=0; i<len; i++)
  {
    *(pres++) = (char) read_memory_byte(cdir++);
  }
  return res;
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
    uint8_t value_tmp = value;
    EEPROM.write(dir, value_tmp);
    EEPROM.commit();
  }
}

void update_memory_string(int dir, char* data, int len)
{
  char *pdata=data;
  int cdir=dir;
  for (int i=0; i<len; i++)
  {
    update_memory_byte(cdir++, (uint8_t) *(data++));
  }
}

char* read_eeprom_ssid(void) {
  return read_memory_string(EEPROM_WIFI_SSID_DIR, EEPROM_WIFI_SSID_LEN);
}

void update_eeprom_ssid(char* data) {
  update_memory_string(EEPROM_WIFI_PASS_DIR, data, EEPROM_WIFI_PASS_LEN);
}
