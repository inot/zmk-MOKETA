# zmk-MOKETA

## Tip

- To switch to the adjust layer, hold down the keys for layer 1 and 2 at the same time
- The layout uses home row mods, to use the modifier (WIN, CTRL, ALT) press the desired key in the home row
- To check the battery level, press the check_bat key on the lower layer
- To control the backlight, use the RGB keys on the adjust Layer
- To change or clear the bluetooth profile, use the adjust layer

## Keymap

![](image/default.png)
![](image/lower.png)
![](image/raise.png)
![](image/adjust.png)

## Layer LED indication

The shield has 4 discrete status LEDs driven via `boards/shields/MOKETA/src/status_led.c`. Brightness for both animations and layer indicators is controlled by the `LED_STATUS_ON` / `LED_STATUS_OFF` macros in that file.

Currently:

- **LED_1** indicates the `LOWER` layer (LWR, index `1`).
- **LED_2** indicates the `RAISE` layer (RSE, index `2`).
- Other LEDs are used by the startup/battery/profile animations.

This is implemented by a simple listener:

- `status_led.c` subscribes to `zmk_layer_state_changed`.
- On each layer change it calls `update_layer_leds()`, which checks `zmk_keymap_layer_active()` for specific layer indices and turns LEDs on/off.

### How to add indication for a new layer

Assume you add a new layer in the keymap (for example `GAM`):

```c
#define DEF 0
#define LWR 1
#define RSE 2
#define ADJ 3
#define GAM 4
```

1. **Expose the new index in `status_led.c`**

   Add (or update) the layer defines near the top of `status_led.c`:

   ```c
   // Keep in sync with keymap: DEF=0, LWR=1, RSE=2, ADJ=3, GAM=4
   #define LAYER_LOWER 1
   #define LAYER_RAISE 2
   #define LAYER_GAME  4
   ```

2. **Decide which LED(s) represent the new layer**

   LEDs are enumerated as:

   ```c
   typedef enum {
       LED_1,
       LED_2,
       LED_3,
       LED_4,
       LED_COUNT
   } LedType;
   ```

   You can:

   - Use a **single LED** per layer (e.g. `GAM` → `LED_3`).
   - Or treat LEDs as bits and light **combinations** (e.g. for a "fifth" layer use `LED_1 + LED_3`).

3. **Extend `update_layer_leds()`**

   The function already handles `LOWER` and `RAISE` using `set_individual_led_brightness()` and `LED_STATUS_ON` / `LED_STATUS_OFF`:

   ```c
   static void update_layer_leds(void) {
       bool lower_active = zmk_keymap_layer_active(LAYER_LOWER);
       bool raise_active = zmk_keymap_layer_active(LAYER_RAISE);

       // LOWER → LED_1
       if (lower_active) {
           set_individual_led_brightness(LED_1, LED_STATUS_ON);
       } else {
           set_individual_led_brightness(LED_1, LED_STATUS_OFF);
       }

       // RAISE → LED_2
       if (raise_active) {
           set_individual_led_brightness(LED_2, LED_STATUS_ON);
       } else {
           set_individual_led_brightness(LED_2, LED_STATUS_OFF);
       }
   }
   ```

   To add a new layer, e.g. `GAM`, and light **two LEDs at once** (for example `LED_1 + LED_2`) when it is active:

   ```c
   static void update_layer_leds(void) {
       bool lower_active = zmk_keymap_layer_active(LAYER_LOWER);
       bool raise_active = zmk_keymap_layer_active(LAYER_RAISE);
       bool game_active  = zmk_keymap_layer_active(LAYER_GAME);

       // LOWER → LED_1
       if (lower_active) {
           set_individual_led_brightness(LED_1, LED_STATUS_ON);
       } else if (!game_active) {
           // Do not override if GAME uses this LED as part of a combination
           set_individual_led_brightness(LED_1, LED_STATUS_OFF);
       }

       // RAISE → LED_2
       if (raise_active) {
           set_individual_led_brightness(LED_2, LED_STATUS_ON);
       } else if (!game_active) {
           set_individual_led_brightness(LED_2, LED_STATUS_OFF);
       }

       // GAME layer → combination of LEDs (here LED_1 + LED_2)
       if (game_active) {
           set_individual_led_brightness(LED_1, LED_STATUS_ON);
           set_individual_led_brightness(LED_2, LED_STATUS_ON);
       }
   }
   ```

   The example above uses a combination of two indicators for a single layer – effectively positional (bitmask) encoding: you can assign different bit patterns over the 4 LEDs to different layers (up to 2^4 = 16 distinct states).

4. **Build and flash the firmware**

   After changing the `keymap` and `status_led.c`, rebuild and flash the firmware. When the new layer is activated, the configured LED combination(s) should light up as documented above.
