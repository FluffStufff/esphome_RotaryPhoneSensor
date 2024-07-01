#include "esphome.h"

class RotaryPhoneSensor : public Component, public Sensor {
private:
  const int controlPin = 26; // Control, white, D5
  const int rotaryPin = 25;  // Rotary, blue, D6

  volatile int rotaryCount = 0;
  volatile bool publishFlag = false;
  
  unsigned long lastRotaryPulseTime = 0;
  const unsigned long debounceDelay = 100; // Debounce delay in milliseconds
  const unsigned long resetDelay = 500; // Reset delay in milliseconds

  static void IRAM_ATTR onRotaryPulse(void* arg) {
    auto* self = static_cast<RotaryPhoneSensor*>(arg);
    unsigned long currentTime = millis();
    if (currentTime - self->lastRotaryPulseTime > self->debounceDelay) {
      self->rotaryCount++;
      self->lastRotaryPulseTime = currentTime;
    }
  }

  static void IRAM_ATTR onControlChange(void* arg) {
    auto* self = static_cast<RotaryPhoneSensor*>(arg);
    self->publishFlag = true;
  }

public:
  void setup() override {
    //pinMode(controlPin, INPUT_PULLUP);
    //pinMode(rotaryPin, INPUT_PULLUP);
    pinMode(controlPin, INPUT_PULLDOWN);
    pinMode(rotaryPin, INPUT_PULLDOWN);
    
    //attachInterruptArg(digitalPinToInterrupt(rotaryPin), onRotaryPulse, this, FALLING);
    attachInterruptArg(digitalPinToInterrupt(rotaryPin), onRotaryPulse, this, RISING);
    attachInterruptArg(digitalPinToInterrupt(controlPin), onControlChange, this, CHANGE);
  }

  void loop() override {
    unsigned long currentTime = millis();
    if (publishFlag) {
      publishFlag = false;
      if (rotaryCount > 0) {
        publish_state(rotaryCount);
        rotaryCount = 0;
        delay(resetDelay);
        publish_state(0);
      }
    }
  }
};
