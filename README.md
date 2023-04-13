# ESP32_ChatGPT

Library for the ESP32 and hopefully the new R4 Uno WiFi when it's out
(Note: json buffers set to 2048 bytes, so long replys may get truncated)

You'll need a root cert from https://api.openai.com inserting into the main.cpp 
  To obtain the root CA certificate, you can follow these steps:

  1. Visit the OpenAI API using your browser: https://api.openai.com
  2. Click on the padlock icon in the address bar.
   
  ![image](https://user-images.githubusercontent.com/22841036/231742547-eb272077-f799-4145-b82c-96ab769fa670.png)
  3. Click on "Certificate (Valid)" or similar text, depending on your browser.
  
  ![image](https://user-images.githubusercontent.com/22841036/231742851-48bc0238-f624-4fdf-afed-93555d865cf7.png)
  
  ![image](https://user-images.githubusercontent.com/22841036/231743096-2f1ddedf-0a1e-4358-804a-715e8d50ddb9.png)
  
  4. Go to the "Details" tab.
  5. Click on the top certificate in the hierarchy (typically "Baltimore Root CA" or similar)
  6. Export or Save to File.


![image](https://user-images.githubusercontent.com/22841036/231741137-70f8ad28-062a-4a94-b875-20b9112ac2d1.png)

  Once you have the root CA certificate, open the exported .cer file with a text editor and copy the content between -----BEGIN CERTIFICATE----- and -----END      CERTIFICATE-----.
  
  Please note that this simple example code uses SSL communication to ensure privacy, it does not perform any API_KEY obfuscation on the source code itself. While this may be acceptable for personal projects on your own hardware, more robust authentication methods should be employed for commercial ventures. 

This was built using PatformIO using ESP32 DEVKIT and only needs the ArduinoJson libary adding to build

There are two demos to chosoe from (use the enviroment selector in VSCode to choose)

## For CLI

```
Connecting to Wi-Fi...
Connected to Wi-Fi
Current time: Wed Apr 12 22:12:47 2023
Please type your question below:
Hi what's it like in there?
I'm an AI language model, so I don't have the ability to experience things like humans do. However, I am here to assist you in any way I can with tasks and answering questions. Is there anything specific that you need help with today?
what is an apple?
An apple is a round, edible fruit that comes in a variety of colors, such as green, red, and yellow. It is a good source of dietary fiber and vitamin C, and is often eaten raw as a healthy snack or used in various recipes, such as pies and sauces.
```

## For Web

![image](https://user-images.githubusercontent.com/22841036/231598671-a30f7cd6-edff-488f-b2d2-29b5fc3f8e1c.png) 

