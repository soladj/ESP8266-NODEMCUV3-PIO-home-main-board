#include <Arduino.h>

void setup_comm();
void loop_comm();

void setup()
{
  Serial.begin(115200);
  Serial.println();

  setup_comm();
}

void loop() {
  loop_comm();
}