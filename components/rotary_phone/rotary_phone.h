#pragma once

#include "esphome/components/key_provider/key_provider.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace rotary_phone {

struct PhoneStore {
  ISRInternalGPIOPin control;
  ISRInternalGPIOPin rotary;
  uint32_t debounce_time;
  uint32_t reset_time;
  uint32_t last_pulse_time{0};
  volatile bool publish{true};
  volatile uint8_t count{0};

  static void control_gpio_intr(PhoneStore *arg);
  static void rotary_gpio_intr(PhoneStore *arg);
};

class RotaryPhone : public key_provider::KeyProvider, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  void set_control_pin(InternalGPIOPin *pin) { this->control_pin_ = pin; };
  void set_rotary_pin(InternalGPIOPin *pin) { this->rotary_pin_ = pin; };
  void set_debounce_time(uint32_t time) { this->store_.debounce_time = time; };
  void set_reset_time(uint32_t time) { this->store_.reset_time = time; };

 protected:
  InternalGPIOPin *control_pin_;
  InternalGPIOPin *rotary_pin_;
  PhoneStore store_{};
  int last_count_{0};

};

}  // namespace rotary_phone
}  // namespace esphome
