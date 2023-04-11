# ESP32_ChatGPT

Library for the ESP32 and hopefully the new R4 Uno WiFi when it's out

You'll need a root cert from https://api.openai.com inserting into the main.cpp 
  To obtain the root CA certificate, you can follow these steps:

  1. Visit the OpenAI API using your browser: https://api.openai.com
  2. Click on the padlock icon in the address bar.
  3. Click on "Certificate (Valid)" or similar text, depending on your browser.
  4. Navigate to the "Certification Path" tab.
  5. Click on the top certificate in the hierarchy (typically "DigiCert Global Root CA" or similar) and click "View Certificate."
  6. Go to the "Details" tab and click "Copy to File..." to export the certificate in the Base-64 encoded X.509 (CER) format.
  Once you have the root CA certificate, open the exported .cer file with a text editor and copy the content between -----BEGIN CERTIFICATE----- and -----END      CERTIFICATE-----.

This was built using PatformIO using ESP32 DEVKIT and only needs the ArduinoJson libary adding to build

Output:

entry 0x400805e4
Connecting to Wi-Fi...
Connected to Wi-Fi
Current time: Thu Jan  1 00:00:04 1970
What is the capital of France?
Sent...
The capital of France is Paris.

