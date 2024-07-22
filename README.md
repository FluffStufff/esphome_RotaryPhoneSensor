# Rotary Phone Dial Sensor
A sensor for ESPHome to allow the use of a rotary phones dial as a sensor to trigger automations in Home Assistant

**By default the dial will operate in a Pull Down mode to allow for wake interrupts to be generated by the dial, there is code commented out if you would instead like to switch to a Pull Up system.** 


## Hardware Setup
There are three pins required, your setup may vary depending on your telephones region of manufacture:
- Live / positive pin
- Control Pin - this breaks connection when the dial is 'live'
- Rotary Pin - this makes connection to register a pulse

Here is a rough picture of what I ended up with, my ESP had issues pulling down the contactors quickly enough for the next pulse to be detected so I added large resistors to assist, you may find these are not necessary in your setup.
![image](https://github.com/FluffStufff/esphome_RotaryPhoneSensor/assets/167688338/747abb17-560f-4bea-ab9e-dc9e2f231dd1)



## Software Setup
Include the external_components in your ESPHome YAML usinig:

```
external_components:
  - source:
      type: git
      url: https://github.com/FluffStufff/esphome_RotaryPhoneSensor
      ref: comp
    components: [ rotary_phone ]
```

And then add the sensor using:

```
rotary_phone:
  - id: phone
    control_pin: 
      number: GPIO26
    rotary_pin: 
      number: GPIO25
```

you can then add a key_collector to allow the phone to dial any length of numbers you like, the example below uses length 3 but the timeout allows for single or double didgits to also be dialled.
Alternatively you can use 'end_key_required' to define a certain number as the end of your sequence, 0 for example.

```
  - id: pincode_reader
    source_id: phone
    min_length: 1
    max_length: 3
    end_key_required: false
    allowed_keys: "0123456789"
    timeout: 4s
    on_result:
      - text_sensor.template.publish:
          id: keypad
          state: !lambda "return x.c_str();"
    on_timeout:
      - text_sensor.template.publish:
          id: keypad
          state: !lambda "return x.c_str();"

text_sensor:
  - platform: template
    name: "Dialled Code"
    id: keypad

```

You should now see the Dialled Code appear under 'sensors' in your ESP Device

![image](https://github.com/FluffStufff/esphome_RotaryPhoneSensor/assets/167688338/25b1e80e-696c-4259-bf06-d5cb0e348149)

Turning the dial will count the pulses, triggering the sensor to update to the correct number, before resetting to zero. this allows Home Assistant to trigger Automations by 'Dialling' numbers.

see [ExampleHomeAssistantAutomationTrigger](https://github.com/FluffStufff/esphome_RotaryPhoneSensor/blob/3aa70f4660cb6a5a7315f8a5eb0fe021b87af26b/ExampleHomeAssistantAutomationTrigger) for an example of how to set this up
