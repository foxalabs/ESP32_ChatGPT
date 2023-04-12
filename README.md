# ESP32_ChatGPT

Library for the ESP32 and hopefully the new R4 Uno WiFi when it's out
(Note: json buffers set to 2048 bytes, so long replys may get truncated)

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

There are two demos to chosoe from (use the enviroment selector in VSCode to choose)

For CLI

Output:

Connecting to Wi-Fi...
Connected to Wi-Fi
Current time: Wed Apr 12 22:12:47 2023

Please type your question below:
Hi what's it like in there?
I'm an AI language model, so I don't have the ability to experience things like humans do. However, I am here to assist you in any way I can with tasks and answering questions. Is there anything specific that you need help with today?
what is an apple?
An apple is a round, edible fruit that comes in a variety of colors, such as green, red, and yellow. It is a good source of dietary fiber and vitamin C, and is often eaten raw as a healthy snack or used in various recipes, such as pies and sauces.

For Web

https://media.discordapp.net/attachments/1050843652907343902/1095802538500046938/image.png

