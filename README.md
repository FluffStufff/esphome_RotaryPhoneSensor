 A sensor for ESPHome to allow the use of a rotary phones dial as a sensor to trigger automations in HomeAssistant

Include the custom_component in your ESPHome YAML usinig:

```
esphome:
  includes:
  - custom_components/rotary_phone_sensor.h 
```

And then add the sensor using:

```
sensor:
  - platform: custom
    lambda: |-
      auto rotary_phone_sensor = new RotaryPhoneSensor();
      App.register_component(rotary_phone_sensor);
      return {rotary_phone_sensor};

    sensors:
      name: "Rotary Dial"
```

You should now see the Rotary Dial appear under 'sensors' in your ESP Device

![image](https://github.com/FluffStufff/esphome_RotaryPhoneSensor/assets/167688338/25b1e80e-696c-4259-bf06-d5cb0e348149)

Turning the dial will count the pulses, triggering the sensor to update to the correct number, before resetting to zero. this allows Home Assistant to trigger Automations by 'Dialling' numbers.

see [ExampleHomeAssistantAutomationTrigger](https://github.com/FluffStufff/esphome_RotaryPhoneSensor/blob/3aa70f4660cb6a5a7315f8a5eb0fe021b87af26b/ExampleHomeAssistantAutomationTrigger) for an example of how to set this up
