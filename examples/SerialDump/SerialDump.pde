#include "Beomote.h"

int irPin = 10;

void setup() {
  Serial.begin(9600);
  
  Beo.initialize(irPin);
}

void loop() {
  BeoCommand cmd;
  
  if (Beo.receive(cmd)) {  
    Serial.print(cmd.link, HEX);
    Serial.print(cmd.address, HEX);
    Serial.println(cmd.command, HEX);
  }
}