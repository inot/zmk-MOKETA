#include <zephyr/kernel.h>
#include <zmk/keymap.h>
#include <zmk/layer_led.h>
#include <zephyr/drivers/led.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

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
extern int LAYER_LOWER;
extern int LAYER_RAISE;
#define LAYER_LWR 3 // Предполагаем, что LWR это слой с индексом 3

// RGB device and LED indices for IJKL keys
#if DT_NODE_EXISTS(DT_INST(0, zmk_rgb_underglow))
static const struct device *rgb_dev = DEVICE_DT_GET(DT_INST(0, zmk_rgb_underglow));
#define LED_I_INDEX 8  // Индекс светодиода под кнопкой I
#define LED_J_INDEX 9  // Индекс светодиода под кнопкой J
#define LED_K_INDEX 10 // Индекс светодиода под кнопкой K
#define LED_L_INDEX 11 // Индекс светодиода под кнопкой L
#define RGB_AVAILABLE 1
#else
#define RGB_AVAILABLE 0
#endif

#if RGB_AVAILABLE
static void set_rgb_led_color(uint32_t led_index, uint8_t r, uint8_t g, uint8_t b) {
    if (rgb_dev && device_is_ready(rgb_dev)) {
        led_set_color(rgb_dev, led_index, r, g, b);
    }
}

static void turn_on_rgb_leds_for_lwr_layer(bool lwr_active) {
    if (lwr_active) {
        // Включаем светодиоды зеленым цветом
        set_rgb_led_color(LED_I_INDEX, 0, 255, 0); // Зеленый
        set_rgb_led_color(LED_J_INDEX, 0, 255, 0); // Зеленый
        set_rgb_led_color(LED_K_INDEX, 0, 255, 0); // Зеленый
        set_rgb_led_color(LED_L_INDEX, 0, 255, 0); // Зеленый
    } else {
        // Выключаем светодиоды
        set_rgb_led_color(LED_I_INDEX, 0, 0, 0); // Черный (выключено)
        set_rgb_led_color(LED_J_INDEX, 0, 0, 0); // Черный (выключено)
        set_rgb_led_color(LED_K_INDEX, 0, 0, 0); // Черный (выключено)
        set_rgb_led_color(LED_L_INDEX, 0, 0, 0); // Черный (выключено)
    }
}
#endif

// LED status definitions
#define LED_STATUS_ON 100
#define LED_STATUS_OFF 0

void update_layer_leds(void) {
    bool wow_active = zmk_keymap_layer_active(LAYER_WOW);
    bool fps_active = zmk_keymap_layer_active(LAYER_FPS);
    bool lower_active = zmk_keymap_layer_active(LAYER_LOWER);
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

    // LOWER → LED_3
    if (lower_active) {
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

    // LWR layer RGB control
#if RGB_AVAILABLE
    turn_on_rgb_leds_for_lwr_layer(lwr_active);
#endif
}
