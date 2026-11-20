#include <Arduino.h>
#include <SCServo.h>

SMS_STS st;

#define S_RXD 18
#define S_TXD 19
#define IR_PIN 25

bool gateOpen = false;

void setup() {
  Serial.begin(115200);
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  st.pSerial = &Serial1;

  pinMode(IR_PIN, INPUT);
  
  // Start with gate closed (horizontal)
  st.WritePosEx(1, 2048, 1200, 50);
  delay(1000);
}

void loop() {
  // IR sensor outputs LOW when an object is detected
  if (digitalRead(IR_PIN) == LOW) {
    if (!gateOpen) {
      // Lift gate arm 90 degrees smoothly
      st.WritePosEx(1, 3072, 1200, 40);
      gateOpen = true;
    }
    delay(200);
  } else if (gateOpen) {
    // Wait 2 seconds after object clears before closing
    delay(2000);
    
    if (digitalRead(IR_PIN) == HIGH) {
      st.WritePosEx(1, 2048, 1000, 30);
      gateOpen = false;
    }
  }
}
