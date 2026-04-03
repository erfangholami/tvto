#include <Wire.h>

  bool LED[8];
  byte motorSpeed[2];
  int adc[4];
  bool key[4];
  byte gwez[12];

void setup() {
  pinMode(21 , OUTPUT);
  pinMode(22 , OUTPUT);
  pinMode(23 , OUTPUT);
  pinMode(24 , OUTPUT);
  pinMode(25 , OUTPUT);
  pinMode(26 , OUTPUT);
  pinMode(27 , OUTPUT);
  pinMode(28 , OUTPUT);
  
  pinMode(31 , INPUT);
  pinMode(33 , INPUT);
  pinMode(35 , INPUT);
  pinMode(37 , INPUT);

  Serial.begin(9600);
  Serial1.begin(38400);
  Wire.begin();
  Wire.beginTransmission(0x58);
  Wire.write(0x01);
  Wire.write(128);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x58);
  Wire.write(0x02);
  Wire.write(128);
  Wire.endTransmission();

  pinMode(A0 , OUTPUT);
  pinMode(A1 , OUTPUT);
  pinMode(A2 , OUTPUT);
  pinMode(A3 , OUTPUT);
}
void loop() {
 /* if (LED[0])
    digitalWrite(21 , HIGH);
  if (LED[1])
    digitalWrite(22, HIGH);
  if (LED[2])
    digitalWrite(23 , HIGH);
  if (LED[3])
    digitalWrite(24 , HIGH);
  if (LED[4])
    digitalWrite(25 , HIGH);
  if (LED[5])
    digitalWrite(26 , HIGH);
  if (LED[6])
    digitalWrite(27 , HIGH);
  if (LED[7])
    digitalWrite(28 , HIGH);*/

  if ((gwez[0] & 0x01) == 1) 
    digitalWrite(21 , HIGH);
  if ((gwez[0] & 0x02) == 2) 
    digitalWrite(22 , HIGH);
  if ((gwez[0] & 0x04) == 4) 
    digitalWrite(23 , HIGH);
   if ((gwez[0] & 0x08) == 8) 
    digitalWrite(24 , HIGH);
  if ((gwez[0] & 0x10) == 16) 
    digitalWrite(25 , HIGH);
  if ((gwez[0] & 0x20) == 32) 
    digitalWrite(26 , HIGH);
   if ((gwez[0] & 0x40) == 64) 
    digitalWrite(27 , HIGH);
  if ((gwez[0] & 0x80) == 128) 
    digitalWrite(28 , HIGH);

  byte a = motorSpeed[0];
  Wire.beginTransmission(0x58);
  Wire.write(0x01);
  Wire.write(a);
  Wire.endTransmission();

  byte b = motorSpeed[1];
  Wire.beginTransmission(0x58);
  Wire.write(0x02);
  Wire.write(b);
  Wire.endTransmission();
    
  key[0] = digitalRead(31);
  key[1] = digitalRead(33);
  key[2] = digitalRead(35);
  key[3] = digitalRead(37);
  
  gwez[0] = LEDToByte();
  gwez[1] = keyToByte();
  gwez[2] = motorSpeed[0];
  gwez[3] = motorSpeed[1];
  
  Serial.print(gwez[1]);

  adc[0] = analogRead(A0);
  gwez[4] = (byte) (adc[0] >> 8);
  gwez[5] = (byte) (adc[0] & 255);

  adc[1] = analogRead(A1);  
  gwez[6] = (byte) (adc[1] >> 8);
  gwez[7] = (byte) (adc[1] & 255);

  adc[2] = analogRead(A2);  
  gwez[8] = (byte) (adc[2] >> 8);
  gwez[9] = (byte) (adc[2] & 255);
  
  adc[3] = analogRead(A3);  
  gwez[10] = (byte) (adc[3] >> 8);
  gwez[11] = (byte) (adc[3] & 255);
  
  Serial1.write(255);
  Serial1.write(255);

  int checksum = 0;
  for (int i = 0 ; i < 12 ;  i++) {
    checksum += gwez[i];
    Serial1.write(gwez[i]);
  }
  Serial1.write(checksum & 255);

}
byte LEDToByte() {
  byte ans = 0;
  if (LED[0])
    ans += 1;
  if (LED[1])
    ans += 2;
  if (LED[2])
    ans += 4;
  if (LED[3])
    ans += 8;
  if (LED[4])
    ans += 16;
  if (LED[5])
    ans += 32;
  if (LED[6])
    ans += 64;
  if (LED[7])
    ans += 128;
  return ans;
}
byte keyToByte() {
  byte ans = 0;
  if (key[0])
    ans += 1;
  if (key[1])
    ans += 2;
  if (key[2])
    ans += 4;
  if (key[3])
    ans += 8;
  return ans;
}
void serialEvent1() {
  int temp[4];
   int x = 0;
  while (Serial1.available() > 0) {
      if (Serial1.read() == 255) {
        temp[0] = Serial1.read();
        temp[1] = Serial1.read();
        temp[2] = Serial1.read();
        temp[3] = Serial1.read();
      x = temp[0] + temp[1] + temp[2];
      }
     if ((x & 255) == temp[3]) {
        gwez[0] = temp[2];
        gwez[2] = temp[0];
        gwez[3] = temp[1];

        motorSpeed[0] = gwez[2];
        motorSpeed[1] = gwez[3];
       
      }
  }
}
void  byteToLED() {
  int ans = gwez[0];
  if (ans > 127) {
    LED[7] = true;
    ans -= 128;
  }
  if (ans > 63) {
    LED[6] = true;
    ans -= 64;
  }
  if (ans > 31) {
    LED[5] = true;
    ans -= 32;
  }
  if (ans > 15) {
    LED[4] = true;
    ans -= 16;
  }
  if (ans > 7) {
    LED[3] = true;
    ans -= 8;
  }
  if (ans > 3) {
    LED[2] = true;
    ans -= 4;
  }
  if (ans > 1) {
    LED[1] = true;
    ans -= 2;
  }
  if (ans > 0)
    LED[0] = true;
  return;
}
