#define EEPROM_WIFI_SSID_DIR 0
#define EEPROM_WIFI_SSID_LEN 32
#define EEPROM_WIFI_PASS_DIR EEPROM_WIFI_SSID_DIR+EEPROM_WIFI_SSID_LEN
#define EEPROM_WIFI_PASS_LEN 32
#define EEPROM_MAIN_TOPIC_DIR EEPROM_WIFI_PASS_DIR+EEPROM_WIFI_PASS_LEN
#define EEPROM_MAIN_TOPIC_LEN 25

void eeprom_init(int n_var);
void read_eeprom_ssid(char*);
void update_eeprom_ssid(char* data);
void read_eeprom_pass(char*);
void update_eeprom_pass(char* data);
