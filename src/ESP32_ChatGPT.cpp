#include "ESP32_ChatGPT.h"

ChatGPT::ChatGPT(const char* apiKey, const char* rootCA): _apiKey(apiKey) {
    _client.setCACert(rootCA);
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

    DynamicJsonDocument jsonDoc(2048);
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
    serializeJson(root, payload);

    String authorizationHeader = "Authorization: Bearer " + String(_apiKey);
    //Serial.println(authorizationHeader);
    _client.println("POST " + String(_apiPath) + " HTTP/1.1");
    _client.println("Host: " + String(_apiDomain));
    _client.println("Content-Type: application/json");
    _client.println(authorizationHeader);
    _client.println("Content-Length: " + String(payload.length()));
    _client.println();
    _client.print(payload);

    String response = "";
    bool header_passed = false;
    bool finish_reason_found = false;
    unsigned long start = millis();

    while (_client.connected() && !finish_reason_found && (millis() - start < timeout)) {
        if (_client.available()) {
            String line = _client.readStringUntil('\n');

            if (!header_passed && line == "\r") {
                header_passed = true;
            }
            else if (header_passed) {
                response += line;

                // Deserialize the JSON response and check for the finish_reason field
                StaticJsonDocument<2048> jsonResponse;
                DeserializationError error = deserializeJson(jsonResponse, response);
                if (!error) {
                    String finish_reason = jsonResponse["choices"][0]["finish_reason"].as<String>();
                    if (!finish_reason.isEmpty()) {
                        finish_reason_found = true;
                        if (finish_reason != "stop") {
                            Serial.println("Error: finish_reason is not 'stop'.");
                        }
                    }
                }
            }
        }
    }
    if (millis() - start > timeout) {
        Serial.println("Error: Timeout.");
    }
    StaticJsonDocument<2048> jsonResponse;
    deserializeJson(jsonResponse, response);
    String chatResponse = jsonResponse["choices"][0]["message"]["content"].as<String>();

    return chatResponse;
}