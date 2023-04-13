#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <time.h>
#include <ESP32_ChatGPT.h>

const char* ROOT_CA = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

static const char* WIFI_SSID = "";
static const char* WIFI_PASSWORD = "";
static const char* OPENAI_API_KEY = "";

const char* MODEL = "gpt-3.5-turbo";

ChatGPT chatGPT(OPENAI_API_KEY, ROOT_CA);


void setSystemTime() {
  // Configure NTP server and time zone
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  // Wait for time to be set
  time_t now = time(nullptr);
  int retries = 0;
  while (now < 8 * 3600 * 2 && retries < 30) {
    delay(100);
    now = time(nullptr);
    retries++;
  }

  // Print the current time
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.println(asctime(&timeinfo));
}

String readString() {
  String inputString = "";  // a String to hold the input
  while (true) {
    while (Serial.available()) {
      char c = Serial.read();  // read the next character
      if (c == '\n') {  // if newline character is received, return the input
        return inputString;
      }
      else {
        inputString += c;  // append the character to the input
        Serial.print(c);  // echo the character back to the Serial Monitor window
      }
    }
  }
}

void myTask(void* parameters) {
  String Question = "What is an apple?";
  // Your main code goes here, including the ChatGPT instance creation and
  // method calls
  StaticJsonDocument<1024> jsonDoc;
  JsonArray messages = jsonDoc.createNestedArray("messages");

  JsonObject message1 = messages.createNestedObject();
  message1["role"] = "system";
  message1["content"] = "You are a helpful assistant.";

  JsonObject message2 = messages.createNestedObject();
  message2["role"] = "user";
  Serial.println("Please type your question below:");
  while (1)
  {
    Question = readString();
    message2["content"] = Question;
    Serial.println(Question);
    String response = chatGPT.createCompletion(messages, MODEL);
    Serial.println(response);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  setSystemTime();
  xTaskCreatePinnedToCore(myTask, "My Task", 8192 * 2, NULL, 1, NULL, 1);
}

void loop() {

}
