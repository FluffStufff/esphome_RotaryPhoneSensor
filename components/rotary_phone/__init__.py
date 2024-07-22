import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import key_provider
from esphome.const import (
    CONF_ID,
    CONF_KEY,
)

CODEOWNERS = ["@FluffStufff", "@ssieb"]

AUTO_LOAD = ["key_provider"]

MULTI_CONF = True

rotary_phone_ns = cg.esphome_ns.namespace("rotary_phone")
RotaryPhone = rotary_phone_ns.class_(
    "RotaryPhone", key_provider.KeyProvider, cg.Component
)

CONF_CONTROL_PIN = "control_pin"
CONF_ROTARY_PIN = "rotary_pin"
CONF_DEBOUNCE_TIME = "debounce_time"
CONF_RESET_TIME = "reset_time"

CONFIG_SCHEMA = cv.All(
    cv.COMPONENT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(RotaryPhone),
            cv.Required(CONF_CONTROL_PIN): pins.gpio_input_pin_schema,
            cv.Required(CONF_ROTARY_PIN): pins.gpio_input_pin_schema,
            cv.Optional(CONF_DEBOUNCE_TIME, default="100ms"): cv.positive_time_period_milliseconds,
            cv.Optional(CONF_RESET_TIME, default="500ms"): cv.positive_time_period_milliseconds,
        }
    ),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    pin = await cg.gpio_pin_expression(config[CONF_CONTROL_PIN])
    cg.add(var.set_control_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_ROTARY_PIN])
    cg.add(var.set_rotary_pin(pin))
    cg.add(var.set_debounce_time(config[CONF_DEBOUNCE_TIME]))
    cg.add(var.set_reset_time(config[CONF_RESET_TIME]))
