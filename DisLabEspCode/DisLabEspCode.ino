#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>

const char* ssid = " ";
const char* password = " ";
byte state;

#define BOTtoken " "  // Вставляем токен бота.
 
// Используйте @myidbot, чтобы получить ID пользователя или группы
// Помните, что бот сможет вам писать только после нажатия
// вами кнопки /start
#define CHAT_ID "2090935945"
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 
// Каждую секунду проверяет новые сообщения
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
 
const int ledPin = 2;
bool ledState = LOW;
 
// Задаем действия при получении новых сообщений 
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
 
  for (int i=0; i<numNewMessages; i++) {
    // Идентификатор чата запроса
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Выводим полученное сообщение
    String text = bot.messages[i].text;
    String subcommand = text.substring(0, 5);
    Serial.println(text);
 
    String from_name = bot.messages[i].from_name;
 
    if (subcommand == "/send") {
      Serial.println(" ");
      Serial.println("=== SEND COMMAND! ===");
      Wire.beginTransmission(44);
      Wire.write(text.substring(6, 27).c_str());             // отправить байт значения  
      Wire.endTransmission();

      bot.sendMessage(chat_id, "Done!", "");
      Serial.println("=====================");
    }
    
    if (text == "/getparametrs") {
      Serial.println(" ");
      Serial.println("=== PARAM COMMAND! ===");
      for (int x=9; x<=16; x++){
        Serial.print(x);
        Wire.beginTransmission(44);
        Wire.write(x);             // отправить байт значения  
        Wire.endTransmission();
        Wire.requestFrom(44, 2);
        while(Wire.available())     // ведомое устройство может послать меньше, чем запрошено
        { 
          int data = Wire.read();     // принять байт как символ
          Serial.print(" GET: ");
          Serial.println(data);
          bot.sendMessage(chat_id, String(data), "");
        }
      }

      bot.sendMessage(chat_id, "Done!", "");
      Serial.println("======================");
    }
    if (text == "/getdata") {
      Serial.println(" ");
      Serial.println("=== DATA COMMAND! ===");
      for (int x=1; x<=8; x++){
        Serial.print(x);
        Wire.beginTransmission(44);
        Wire.write(x);             // отправить байт значения  
        Wire.endTransmission();
        Wire.requestFrom(44, 2);
        while(Wire.available())     // ведомое устройство может послать меньше, чем запрошено
        { 
          if (x < 7){
            float data = Wire.read();
            Serial.print(" GET:");
            Serial.println(data);
            bot.sendMessage(chat_id, String(data), "");
          }
          else{
            long data = Wire.read();
            Serial.print(" GET:");
            Serial.println(data);
            bot.sendMessage(chat_id, String(data), "");
          }
               // принять байт как символ
        }
      }

      bot.sendMessage(chat_id, "Done!", "");
      Serial.println("======================");
    }
  }
}
 
void setup() {
  Serial.begin(115200);
  Wire.begin();
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Подключаемся к Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Добавляем корневой сертификат для api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Выводим IP ESP32
  Serial.println(WiFi.localIP());
 
  bot.sendMessage(CHAT_ID, "Bot Started", "");
}
 
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
