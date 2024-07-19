#include "rotary_phone.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rotary_phone {

static const char *const TAG = "rotary_phone";

void IRAM_ATTR HOT PhoneStore::control_gpio_intr(PhoneStore *store) {
  uint32_t now = millis();
  if (now - store->last_pulse_time < store->debounce_time)
    return;
  store->count++;
  store->last_pulse_time = now;
}

void IRAM_ATTR HOT PhoneStore::rotary_gpio_intr(PhoneStore *store) {
  if (store->control.digital_read()) {
    store->count = 0;
  } else {
    store->publish = true;
  }
}

void RotaryPhone::setup() {
  this->control_pin_->setup();
  this->store_.control = this->control_pin_->to_isr();
  this->rotary_pin_->setup();
  this->store_.rotary = this->rotary_pin_->to_isr();
    
  this->control_pin_->attach_interrupt(PhoneStore::control_gpio_intr, &this->store_, gpio::INTERRUPT_RISING_EDGE);
  this->rotary_pin_->attach_interrupt(PhoneStore::rotary_gpio_intr, &this->store_, gpio::INTERRUPT_ANY_EDGE);
}

void RotaryPhone::loop() {
  int count = this->store_.count;
  if (count != this->last_count_) {
    ESP_LOGV(TAG, "counting %d", count);
    this->last_count_ = count;
  }
  if (!this->store_.publish)
    return;
  this->store_.publish = false;
  if (count == 0) {
    ESP_LOGW(TAG, "dialed with no count");
    return;
  }
  if (count == 10)
    count = 0;
  this->send_key_('0' + count);
  ESP_LOGD(TAG, "dialed a %d", count);
  this->store_.count = 0;
  this->last_count_ = 0;
}

void RotaryPhone::dump_config() {
  ESP_LOGCONFIG("", "Rotary Phone:");
}

}  // namespace rotary_phone
}  // namespace esphome
