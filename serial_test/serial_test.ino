#include <SoftwareSerial.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial1.println("hello world serial write.");
    Serial.println("hello world serial println");
    delay(1100);
}
