
#include <zephyr/kernel.h>
#include <zmk/keymap.h>
#include <zmk/layer_led.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>

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

// RGB device and LED indices for IJKL keys
#if DT_NODE_EXISTS(DT_CHOSEN(zmk_underglow))
static const struct device *rgb_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_underglow));
#define LED_I_INDEX 26  // Индекс светодиода под кнопкой I
#define LED_J_INDEX 31  // Индекс светодиода под кнопкой J
#define LED_K_INDEX 32  // Индекс светодиода под кнопкой K
#define LED_L_INDEX 33  // Индекс светодиода под кнопкой L
#define RGB_AVAILABLE 1
#else
#define RGB_AVAILABLE 0
#endif

#define LED_STATUS_ON 100
#define LED_STATUS_OFF 0

#if RGB_AVAILABLE
static void set_rgb_led_color(uint32_t led_index, uint8_t r, uint8_t g, uint8_t b) {
    LOG_DBG("set_rgb_led_color called: led_index=%d, r=%d, g=%d, b=%d", led_index, r, g, b);
    if (rgb_dev && device_is_ready(rgb_dev)) {
        LOG_DBG("Setting LED color");
        // Попробуем использовать функцию led_set_color из Zephyr
        uint8_t color[3] = {r, g, b};
        int ret = led_set_color(rgb_dev, led_index, 3, color);
        if (ret) {
            LOG_ERR("Failed to set LED color, error: %d", ret);
        } else {
            LOG_DBG("LED color set successfully");
        }
    } else {
        LOG_DBG("RGB device not ready or not available");
    }
}

static void turn_on_rgb_leds_for_lwr_layer(bool lwr_active) {
    LOG_DBG("turn_on_rgb_leds_for_lwr_layer called with lwr_active=%d", lwr_active);
    if (lwr_active) {
        // Включаем светодиоды зеленым цветом
        LOG_DBG("Turning on RGB LEDs for IJKL keys");
        LOG_DBG("Setting LED_I_INDEX (%d) to green", LED_I_INDEX);
        set_rgb_led_color(LED_I_INDEX, 0, 255, 0); // Зеленый
        LOG_DBG("Setting LED_J_INDEX (%d) to green", LED_J_INDEX);
        set_rgb_led_color(LED_J_INDEX, 0, 255, 0); // Зеленый
        LOG_DBG("Setting LED_K_INDEX (%d) to green", LED_K_INDEX);
        set_rgb_led_color(LED_K_INDEX, 0, 255, 0); // Зеленый
        LOG_DBG("Setting LED_L_INDEX (%d) to green", LED_L_INDEX);
        set_rgb_led_color(LED_L_INDEX, 0, 255, 0); // Зеленый
    } else {
        // Выключаем светодиоды
        LOG_DBG("Turning off RGB LEDs for IJKL keys");
        LOG_DBG("Setting LED_I_INDEX (%d) to black", LED_I_INDEX);
        set_rgb_led_color(LED_I_INDEX, 0, 0, 0); // Черный (выключено)
        LOG_DBG("Setting LED_J_INDEX (%d) to black", LED_J_INDEX);
        set_rgb_led_color(LED_J_INDEX, 0, 0, 0); // Черный (выключено)
        LOG_DBG("Setting LED_K_INDEX (%d) to black", LED_K_INDEX);
        set_rgb_led_color(LED_K_INDEX, 0, 0, 0); // Черный (выключено)
        LOG_DBG("Setting LED_L_INDEX (%d) to black", LED_L_INDEX);
        set_rgb_led_color(LED_L_INDEX, 0, 0, 0); // Черный (выключено)
    }
}
#endif

void update_layer_leds(void) {
    LOG_DBG("update_layer_leds called");

    bool wow_active = zmk_keymap_layer_active(LAYER_WOW);
    bool fps_active = zmk_keymap_layer_active(LAYER_FPS);
    bool raise_active = zmk_keymap_layer_active(LAYER_RAISE);
    bool lwr_active = zmk_keymap_layer_active(LAYER_LWR);

    LOG_DBG("Layer states: WOW=%d, FPS=%d, RAISE=%d, LWR=%d", 
            wow_active, fps_active, raise_active, lwr_active);

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

    // RAISE → LED_4
    if (raise_active) {
        set_individual_led_brightness(3, LED_STATUS_ON);
    } else {
        set_individual_led_brightness(3, LED_STATUS_OFF);
    }

    // LWR layer RGB control
#if RGB_AVAILABLE
    LOG_DBG("LWR layer active: %d", lwr_active);
    turn_on_rgb_leds_for_lwr_layer(lwr_active);
#endif
}

