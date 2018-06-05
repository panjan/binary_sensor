#define SWITCH_PIN    4  // GPIO4

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  int switch_state = digitalRead(SWITCH_PIN);
  Serial.println("Switch state: " + String(switch_state)); // 0 close, 1 open
  for(int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }
  ESP.deepSleep(1000000 * 1);
}

void loop() {}
