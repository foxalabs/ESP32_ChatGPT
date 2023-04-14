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

#define JsonMaxSize 4096
#include "ESP32_ChatGPT.h"
#include <WiFiClientSecure.h>
ChatGPT::ChatGPT(const char* apiKey, const char* rootCA): _apiKey(apiKey) {
    _client.setCACert(rootCA);
}

bool isHex(const String &str) {
    if (str.length() == 0) {
        return false;
    }
    for (size_t i = 0; i < str.length() - 1; i++) {
        char c = str[i];
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
            return false;
        }
    }
    return true;
}


String ChatGPT::createCompletion(const JsonArray& messages,
    const String& model,
    unsigned long timeout,
    int maxTokens,
    float temperature,
    float top_p,
    int n,
    bool stream,
    const String& stop,
    float presence_penalty,
    float frequency_penalty,
    const JsonObject& logit_bias,
    const String& user) {
    if (!_client.connect(_apiDomain, 443)) {
        Serial.println("Connection failed");
        return "";
    }
    DynamicJsonDocument jsonDoc(JsonMaxSize);
    JsonObject root = jsonDoc.to<JsonObject>();
    root["model"] = model;
    root["messages"] = messages;
    root["temperature"] = temperature;
    root["top_p"] = top_p;
    root["n"] = n;
    root["stream"] = stream;
    if (maxTokens != -1) {
        root["max_tokens"] = maxTokens;
    }
    if (stop.length() > 0) {
        root["stop"] = stop;
    }
    root["presence_penalty"] = presence_penalty;
    root["frequency_penalty"] = frequency_penalty;
    if (!logit_bias.isNull()) {
        root["logit_bias"] = logit_bias;
    }
    if (user.length() > 0) {
        root["user"] = user;
    }

    String payload;
    String authorizationHeader = "";
    serializeJson(root, payload);
    authorizationHeader = "Authorization: Bearer " + String(_apiKey);
    _client.println("POST " + String(_apiPath) + " HTTP/1.1");
    _client.println("Host: " + String(_apiDomain));
    _client.println("Content-Type: application/json");
    _client.println(authorizationHeader);
    _client.println("Content-Length: " + String(payload.length()));
    _client.println();
    _client.print(payload);

    String response = "";
    bool header_passed = false;
    unsigned long start = millis();

    while (_client.connected() && (millis() - start < timeout)) {
    if (_client.available()) {
        String line = _client.readStringUntil('\n');
        //Serial.println("Line: " + line); // Debug print statement
        if (!header_passed && line == "\r") {
            header_passed = true;
        }
        else if (header_passed) {
            if (line.length() <= 5 && isHex(line)) {
                // Skip chunk length lines and final "0" line
                continue;
            }
            response += line;
                StaticJsonDocument<JsonMaxSize> jsonResponse;
                DeserializationError error = deserializeJson(jsonResponse, response);
                if (!error) {
                    String finish_reason = jsonResponse["choices"][0]["finish_reason"].as<String>();
                    if (!finish_reason.isEmpty()) {
                        if (finish_reason == "stop") {
                            // Handle 'stop' finish_reason
                            break;
                        }
                        else {
                            // Handle any other finish_reasons
                            Serial.print("Finish reason: ");
                            Serial.println(finish_reason);
                            break;
                        }
                    }
                }
            }
        }
    }
    if (millis() - start > timeout) {
        Serial.println("Error: Timeout.");
    }
    StaticJsonDocument<JsonMaxSize> jsonResponse;
    //deserializeJson(jsonResponse, response);
    response.trim();
    DeserializationError error = deserializeJson(jsonResponse, response);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    }
    String chatResponse = jsonResponse["choices"][0]["message"]["content"].as<String>();
    //String chatResponse = jsonResponse["choices"][0]["message"]["content"].as<String>();
    //Serial.println(response);
    //Serial.println(chatResponse);
    _client.stop();
    return chatResponse;
}
