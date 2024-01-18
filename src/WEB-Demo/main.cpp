 /*Copyright [2023-04-11] [Spencer Bentley]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <time.h>
#include <WebServer.h>
#include <ESP32_ChatGPT.h>
#include "..\config.h"

const char* MODEL = "gpt-3.5-turbo";

String userQuestion = "";

ChatGPT chatGPT(OPENAI_API_KEY, ROOT_CA);

WebServer server(80);

String getWebPage() {
  String html = R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chatbot</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: #1f1f1f;
        color: #ffffff;
        margin: 0;
        padding: 0;
      }
      h1 {
        background-color: #3f3f3f;
        padding: 20px;
        margin: 0;
        text-align: center;
      }
      #chatbox {
        width: 100%;
        height: 300px;
        overflow-y: scroll;
        padding: 5px;
        margin-top: 20px;
        background-color: #1f1f1f;
      }
      .message {
        display: inline-block;
        max-width: 60%;
        padding: 8px;
        border-radius: 15px;
        margin-bottom: 8px;
      }
      .user {
        background-color: #0b93f6;
        color: #ffffff;
        margin-right: 5%;
      }
      .assistant {
        background-color: #3f3f3f;
        color: #ffffff;
        margin-left: 5%;
      }
      #question {
        width: calc(100% - 10px);
        margin-top: 20px;
        box-sizing: border-box;
      }
      button {
        width: 100%;
        background-color: #3f3f3f;
        color: #ffffff;
        border: 1px solid #3f3f3f;
        margin-top: 10px;
        padding: 5px 0;
        cursor: pointer;
      }
      button:hover {
        background-color: #4f4f4f;
      }
    </style>
  </head>
  <body>
    <h1>ESP32 Chatbot</h1>
    <div id="chatbox"></div>
    <input type="text" id="question" placeholder="Type your question..." onkeydown="if (event.keyCode == 13) { sendQuestion(); }">
    <button onclick="sendQuestion()">Ask</button>
    <script>
      function sendQuestion() {
        var question = document.getElementById('question').value;
        if (!question) return;
        var userMessage = '<div style="text-align: right;"><div class="message user">' + question + '</div></div>';
        document.getElementById('chatbox').insertAdjacentHTML('beforeend', userMessage);
        fetch('/question?text=' + encodeURIComponent(question))
          .then(response => response.text())
          .then(text => {
            var assistantMessage = '<div style="text-align: left;"><div class="message assistant">' + text + '</div></div>';
            document.getElementById('chatbox').insertAdjacentHTML('beforeend', assistantMessage);
            document.getElementById('question').value = '';
          });
      }
    </script>
  </body>
  </html>
  )=====";
  return html;
}




void handleRoot() {
  server.send(200, "text/html", getWebPage());
}

void setSystemTime() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    int retries = 0;
    while (now < 8 * 3600 * 2 && retries < 30) {
        delay(100);
        now = time(nullptr);
        retries++;
    }
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.println(asctime(&timeinfo));

}

void chatGPTTask(void* parameters) {
  while (1) {
    if (userQuestion != "") {
      StaticJsonDocument<4096> jsonDoc;
      JsonArray messages = jsonDoc.createNestedArray("messages");

      JsonObject message1 = messages.createNestedObject();
      message1["role"] = "system";
      message1["content"] = "You are a helpful assistant. At the end of the chat you should put a suitable emoji, and also include emojis whereever they seem appropriate in the text.";

      JsonObject message2 = messages.createNestedObject();
      message2["role"] = "user";
      message2["content"] = userQuestion;
      
      String response = chatGPT.createCompletion(messages, MODEL);
      
      server.send(200, "text/plain", response);
      userQuestion = "";
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void handleQuestion() {
  if (server.hasArg("text")) {
    userQuestion = server.arg("text");
    while (userQuestion != "") {
      delay(10);
    }
  } else {
    server.send(400, "text/plain", "Bad Request");
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
  server.on("", handleRoot);
  server.on("/", handleRoot);
  server.on("/question", handleQuestion);
  server.begin();
  Serial.println("Server Started.");
  Serial.println("Connected to Wi-Fi, IP address: " + WiFi.localIP().toString());
  xTaskCreatePinnedToCore(chatGPTTask, "ChatGPT Task", 8192 * 3, NULL, 1, NULL, 1);
}

void loop() {
    server.handleClient();
    delay(10);
}
