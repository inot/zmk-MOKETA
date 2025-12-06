
#include <zephyr/kernel.h>
#include <zmk/keymap.h>
#include <zmk/layer_led.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// External dependencies from status_led.c
struct Led {
    const struct device *dev;
    uint32_t id;
};
extern struct Led individual_leds[];
extern void set_individual_led_brightness(int led, uint8_t brightness);

// Layer indices (keep in sync with keymap: WOW=1, FPS=2, LWR=3, RSE=4)
extern int LAYER_WOW;
extern int LAYER_FPS;
extern int LAYER_LWR;
extern int LAYER_RAISE;

#define LED_STATUS_ON 100
#define LED_STATUS_OFF 0

void update_layer_leds(void) {
    bool wow_active = zmk_keymap_layer_active(LAYER_WOW);
    bool fps_active = zmk_keymap_layer_active(LAYER_FPS);
    bool raise_active = zmk_keymap_layer_active(LAYER_RAISE);
    bool lwr_active = zmk_keymap_layer_active(LAYER_LWR);

    // WOW → LED_1
    if (wow_active) {
        set_individual_led_brightness(0, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(0, LED_STATUS_OFF);
    }

    // FPS → LED_2
    if (fps_active) {
        set_individual_led_brightness(1, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(1, LED_STATUS_OFF);
    }

    // LWR → LED_3
    if (lwr_active) {
        set_individual_led_brightness(2, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(2, LED_STATUS_OFF);
    }

    // RAISE → LED_4
    if (raise_active) {
        set_individual_led_brightness(3, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(3, LED_STATUS_OFF);
    }
}

