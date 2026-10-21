const int blinkSensorPin = 35;
const int mistPin = 4;
const int ldrPin = 34;

// للتجربة 5 ثواني. بعد ما تتأكدين رجعيها 60000
const unsigned long interval = 5000;
const unsigned long debounceMs = 50;

int lastState = LOW;
int blinkCount = 0;
unsigned long previousMillis = 0;
unsigned long lastBlinkTime = 0;

const int BLINK_RATE_THRESHOLD = 10;
const int LIGHT_THRESHOLD_LDR = 500;

void setup() {
  Serial.begin(115200);

  pinMode(blinkSensorPin, INPUT);
  pinMode(mistPin, OUTPUT);
  digitalWrite(mistPin, LOW);

  Serial.println("HydroLens - Integrated System Initialized Successfully.");
}

void loop() {
  unsigned long now = millis();

  int sensorValue = digitalRead(blinkSensorPin);
  if (sensorValue == HIGH && lastState == LOW && now - lastBlinkTime >= debounceMs) {
    blinkCount++;
    lastBlinkTime = now;
  }
  lastState = sensorValue;

  if (now - previousMillis >= interval) {
    previousMillis = now;

    int currentLight = analogRead(ldrPin);

    Serial.println("----------------------------------------");
    Serial.print("Blinks counted: ");
    Serial.println(blinkCount);
    Serial.print("Current Room Light (LDR): ");
    Serial.println(currentLight);

    bool isDryEyes = (blinkCount < BLINK_RATE_THRESHOLD);
    bool isLowLight = (currentLight < LIGHT_THRESHOLD_LDR);

    if (isDryEyes && isLowLight) {
      Serial.println("Decision: DRY EYES & LOW LIGHT -> Activating Intensive Mist!");
      digitalWrite(mistPin, HIGH);
    } else if (isDryEyes) {
      Serial.println("Decision: DRY EYES -> Activating Normal Mist.");
      digitalWrite(mistPin, HIGH);
    } else {
      Serial.println("Decision: SAFE CONDITIONS -> Mist OFF.");
      digitalWrite(mistPin, LOW);
    }
    Serial.println("----------------------------------------");

    blinkCount = 0;
  }
}
