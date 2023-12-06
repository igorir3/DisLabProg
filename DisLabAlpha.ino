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

void setup() {
  pinMode(MotorOutput1, INPUT);
  pinMode(MotorOutput2, INPUT);
  Serial.begin(115200);

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

  if (Serial.available() > 0) {
    String serialinput = Serial.readStringUntil("\n");
    if (serialinput == "OUTPUT\n") {
      long a = analogRead(TermistorOutput1);
      long b = analogRead(TermistorOutput2);
      long c = analogRead(TermistorOutput3);
      long d = analogRead(TermistorOutput4);
      long e = analogRead(TermistorOutput5);
      long f = analogRead(TermistorOutput6);
      long motor1 = analogRead(MotorOutput1);
      long motor2 = analogRead(MotorOutput2);

      float tempC = beta / (log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;   // Цельсия
      float tempC1 = beta / (log((1025.0 * 10 / b - 10) / 10) + beta / 298.0) - 273.0;  // Цельсия
      float tempC2 = beta / (log((1025.0 * 10 / c - 10) / 10) + beta / 298.0) - 273.0;  // Цельсия
      float tempC3 = beta / (log((1025.0 * 10 / d - 10) / 10) + beta / 298.0) - 273.0;  // Цельсия
      float tempC4 = beta / (log((1025.0 * 10 / e - 10) / 10) + beta / 298.0) - 273.0;  // Цельсия
      float tempC5 = beta / (log((1025.0 * 10 / f - 10) / 10) + beta / 298.0) - 273.0;  // Цельсия

      Serial.println(String(tempC) + " | " + String(tempC1) + " | " + String(tempC2) + " | " + String(tempC3) + " | " + String(tempC4) + " | " + String(tempC5) + " | " + String(motor1) + " | " + String(motor2));
    }  else if (serialinput == "PARAM\n") {
      Serial.println(String(paramTermInputForward1) + " | " + String(paramTermInputForward2) + " | " + String(paramTermInputBack1) + " | " + String(paramTermInputBack2) + " | " + String(paramMotorAnalogInput1) + " | " + String(paramMotorAnalogInput2) + " | " + String(paramTermAnalogInput1) + " | " +  String(paramTermAnalogInput2));
    }
    else {
      if (serialinput != "OUTPUT\n" && serialinput != "\n" && serialinput.substring(0, 1) == "I") {
        paramMotorAnalogInput1 = serialinput.substring(5, 9).toInt() - 1000;
        paramMotorAnalogInput2 = serialinput.substring(9, 13).toInt() - 1000;
        paramTermAnalogInput1 = serialinput.substring(13, 17).toInt() - 1000;
        paramTermAnalogInput2 = serialinput.substring(17, 21).toInt() - 1000;
        paramTermInputForward1 = serialinput.substring(1, 2).toInt();
        paramTermInputForward2 = serialinput.substring(2, 3).toInt();
        paramTermInputBack1 = serialinput.substring(3, 4).toInt();
        paramTermInputBack2 = serialinput.substring(4, 5).toInt();
      }
    }
  }

}
