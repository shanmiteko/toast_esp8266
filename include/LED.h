#pragma once

#define LED_ON digitalWrite(LED_BUILTIN, LOW)
#define LED_OFF digitalWrite(LED_BUILTIN, HIGH)
#define LED_BLINK(something) \
    LED_ON;                  \
    something;               \
    LED_OFF;
