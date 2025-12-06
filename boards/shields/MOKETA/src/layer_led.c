#include <zephyr/kernel.h>
#include <zmk/keymap.h>
#include <zmk/layer_led.h>

// External dependencies from status_led.c
extern struct Led {
    const struct device *dev;
    uint32_t id;
};

extern struct Led individual_leds[];
extern void set_individual_led_brightness(int led, uint8_t brightness);

// Layer indices (keep in sync with keymap: WOW=1, FPS=2, LWR=3, RSE=4)
extern int LAYER_WOW;
extern int LAYER_FPS;
extern int LAYER_LOWER;
extern int LAYER_RAISE;

// LED enumeration
typedef enum {
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_COUNT
} LedType;

// LED status definitions
#define LED_STATUS_ON 100
#define LED_STATUS_OFF 0

void update_layer_leds(void) {
    bool wow_active = zmk_keymap_layer_active(LAYER_WOW);
    bool fps_active = zmk_keymap_layer_active(LAYER_FPS);
    bool lower_active = zmk_keymap_layer_active(LAYER_LOWER);
    bool raise_active = zmk_keymap_layer_active(LAYER_RAISE);

    // WOW → LED_1
    if (wow_active) {
        set_individual_led_brightness(LED_1, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(LED_1, LED_STATUS_OFF);
    }

    // FPS → LED_2
    if (fps_active) {
        set_individual_led_brightness(LED_2, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(LED_2, LED_STATUS_OFF);
    }

    // LOWER → LED_3
    if (lower_active) {
        set_individual_led_brightness(LED_3, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(LED_3, LED_STATUS_OFF);
    }

    // RAISE → LED_4
    if (raise_active) {
        set_individual_led_brightness(LED_4, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(LED_4, LED_STATUS_OFF);
    }
}
