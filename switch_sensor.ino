#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#import "secret.h"
#define SWITCH_PIN        4  // GPIO4
#define BUTTON_PIN        5  // GPIO5
#define BUTTON_PRESSED    0

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  connectToWiFi();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  int button_state = digitalRead(BUTTON_PIN);
  Serial.println("Button state: " + String(button_state));

  if(button_state == BUTTON_PRESSED) {
    handleOTA();
  } else {
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    int switch_state = digitalRead(SWITCH_PIN);
    Serial.println("Switch state: " + String(switch_state)); // 0 close, 1 open
  }

  ESP.deepSleep(1000000 * 1);
}

void handleOTA() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  for(int i = 0; i < 100; i++)
  {
    ArduinoOTA.handle();
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}

void loop() {}
