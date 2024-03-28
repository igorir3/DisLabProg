 
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>
 
// Вставьте сюда свои учетные данные сети
const char* ssid = " ";
const char* password = " ";
byte state;
 
// Запускаем бот
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
    Serial.println(text);
 
    String from_name = bot.messages[i].from_name;
 
    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      Wire.beginTransmission(44);
      Wire.write('a');             // отправить байт значения  
      Wire.endTransmission();
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      Wire.beginTransmission(44);
      Wire.write('b');             // отправить байт значения  
      Wire.endTransmission();
    }
    
    if (text == "/state") {
      Wire.requestFrom(44, 1);
      while(Wire.available())     // ведомое устройство может послать меньше, чем запрошено
      { 
        byte state = Wire.read();     // принять байт как символ
        Serial.print(state);          // напечатать символ
      }
      if (state == byte(97)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else if (state == byte(98)){
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
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
