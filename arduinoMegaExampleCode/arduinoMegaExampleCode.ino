#include <Wire.h>
int stat_LED;  
byte i2c_rcv;

void setup() {
  Wire.begin(44);
  Serial.begin(9600);
  stat_LED = 0;
  pinMode(13, OUTPUT);
  Wire.onReceive(dataRcv);
	Wire.onRequest(dataRqst);

}

void loop() {
  digitalWrite(13, stat_LED);
}

void dataRcv(int numBytes)
{
	while(Wire.available()) {
		i2c_rcv = Wire.read();
    Serial.println(i2c_rcv);
    if (i2c_rcv == 97)
      {stat_LED = 1;}
    else if (i2c_rcv == 98)
      {stat_LED = 0;}
	}
}
void dataRqst()
{
  Serial.println("CHECK");
  if (stat_LED == 0)
	  {Wire.write(98);}
  else if (stat_LED == 1)
	  {Wire.write(97);}
}