#define beta 4090
#define resistance 10

#define MotorOutput1 A0
#define MotorOutput2 A1

#define TermInputForward1 25
#define TermInputForward2 22
#define TermInputBack1 24
#define TermInputBack2 23

#define TermAnalogInput1 2
#define TermAnalogInput2 3

#define MotorInputForward1 26
#define MotorInputForward2 27

#define MotorAnalogInput1 5
#define MotorAnalogInput2 4

#define TermistorOutput1 A2
#define TermistorOutput2 A3
#define TermistorOutput3 A4
#define TermistorOutput4 A5
#define TermistorOutput5 A6
#define TermistorOutput6 A7

int paramTermInputForward1 = 0;
int paramTermInputForward2 = 0;
int paramTermInputBack1 = 0;
int paramTermInputBack2 = 0;

int paramMotorAnalogInput1 = 255;
int paramMotorAnalogInput2 = 255;
int paramTermAnalogInput1 = 0;
int paramTermAnalogInput2 = 0;

String i2c_rcv;
int sender = 0;

void setup() {
  Wire.begin(44);
  Wire.onReceive(dataRcv);
	Wire.onRequest(dataRqst);

  pinMode(MotorOutput1, INPUT);
  pinMode(MotorOutput2, INPUT);

  pinMode(TermInputForward1, OUTPUT);
  pinMode(TermInputForward2, OUTPUT);
  pinMode(TermInputBack1, OUTPUT);
  pinMode(TermInputBack2, OUTPUT);

  pinMode(TermAnalogInput1, OUTPUT);
  pinMode(TermAnalogInput2, OUTPUT);

  pinMode(MotorInputForward1, OUTPUT);
  pinMode(MotorInputForward2, OUTPUT);

  pinMode(MotorAnalogInput1, OUTPUT);
  pinMode(MotorAnalogInput2, OUTPUT);
}

void loop() {
  digitalWrite(MotorInputForward1, 1);
  digitalWrite(MotorInputForward2, 1);

  digitalWrite(TermInputForward1, paramTermInputForward1);
  digitalWrite(TermInputForward2, paramTermInputForward2);
  digitalWrite(TermInputBack1, paramTermInputBack1);
  digitalWrite(TermInputBack2, paramTermInputBack2);

  analogWrite(MotorAnalogInput1, paramMotorAnalogInput1);
  analogWrite(MotorAnalogInput2, paramMotorAnalogInput2);
  analogWrite(TermAnalogInput1, paramTermAnalogInput1);
  analogWrite(TermAnalogInput2, paramTermAnalogInput2);
}

void dataRqst()
{
    switch (sender){
    case 1:
      long a = analogRead(TermistorOutput1);
      float tempC = beta / (log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;   // Цельсия
      Wire.write(tempC)
    break;
    case 2:
      long b = analogRead(TermistorOutput2);
      float tempC1 = beta / (log((1025.0 * 10 / b - 10) / 10) + beta / 298.0) - 273.0;  // Цельсия
      Wire.write(tempC1)
    break;
    case 3:
      long c = analogRead(TermistorOutput3);
      float tempC2 = beta / (log((1025.0 * 10 / c - 10) / 10) + beta / 298.0) - 273.0;
      Wire.write(tempC2)
    break;
    case 4:
      long d = analogRead(TermistorOutput4);
      float tempC3 = beta / (log((1025.0 * 10 / d - 10) / 10) + beta / 298.0) - 273.0;
      Wire.write(tempC3)
    break;
    case 5:
      long e = analogRead(TermistorOutput5);
      float tempC4 = beta / (log((1025.0 * 10 / e - 10) / 10) + beta / 298.0) - 273.0;
      Wire.write(tempC4)
    break;
    case 6:
      long f = analogRead(TermistorOutput6);
      float tempC5 = beta / (log((1025.0 * 10 / f - 10) / 10) + beta / 298.0) - 273.0;
      Wire.write(tempC5)
    break;
    case 7:
      long motor1 = analogRead(MotorOutput1);
      Wire.write(motor1)
    break;
    case 8:
      long motor2 = analogRead(MotorOutput2);
      Wire.write(motor2)
    break;
    case 9:
      Wire.write(paramTermInputForward1)
    break;
    case 10:
      Wire.write(paramTermInputForward2)
    break;
    case 11:
      Wire.write(paramTermInputBack1)
    break;
    case 12:
      Wire.write(paramTermInputBack2)
    break;
    case 13:
      Wire.write(paramMotorAnalogInput1)
    break;
    case 14:
      Wire.write(paramMotorAnalogInput2)
    break;
    case 15:
      Wire.write(paramTermAnalogInput1)
    break;
    case 16:
      Wire.write(paramTermAnalogInput2)
    break;
}

void dataRcv(int numBytes) {
  while(Wire.available()) {
    if (numBytes > 2){
		  String i2c_rcv = Wire.read();
      paramMotorAnalogInput1 = serialinput.substring(5, 9).toInt() - 1000;
      paramMotorAnalogInput2 = serialinput.substring(9, 13).toInt() - 1000;
      paramTermAnalogInput1 = serialinput.substring(13, 17).toInt() - 1000;
      paramTermAnalogInput2 = serialinput.substring(17, 21).toInt() - 1000;
      paramTermInputForward1 = serialinput.substring(1, 2).toInt();
      paramTermInputForward2 = serialinput.substring(2, 3).toInt();
      paramTermInputBack1 = serialinput.substring(3, 4).toInt();
      paramTermInputBack2 = serialinput.substring(4, 5).toInt();
    }
    else{
      int sender = Wire.read();
    }
    
  }
}
