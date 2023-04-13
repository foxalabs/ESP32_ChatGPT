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

#ifndef ESP32_CHATGPT_H
#define ESP32_CHATGPT_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

class ChatGPT {
public:
  ChatGPT(const char* apiKey, const char* rootCA);
  String createCompletion(
    const JsonArray& messages, 
    const String& model, 
    unsigned long timeout = 15000,
    int maxTokens = -1, 
    float temperature = 1, 
    float top_p = 1, 
    int n = 1, 
    bool stream = false, 
    const String& stop = "", 
    float presence_penalty = 0, 
    float frequency_penalty = 0, 
    const JsonObject& logit_bias = JsonObject(), 
    const String& user = "");

private:
  const char* _apiKey;
  const char* _apiDomain = "api.openai.com";
  const char* _apiPath = "/v1/chat/completions";
  WiFiClientSecure _client;
};

#endif // ESP32_CHATGPT_H
