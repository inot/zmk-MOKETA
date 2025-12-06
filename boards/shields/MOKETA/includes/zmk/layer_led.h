#pragma once

/**
 * @file layer_led.h
 * @brief Layer LED indication for MOKETA keyboard
 *
 * This module provides LED indication for active layers on the MOKETA keyboard.
 * It controls 4 discrete status LEDs based on the currently active layer.
 * Each layer is associated with a specific LED:
 * - WOW layer → LED_1
 * - FPS layer → LED_2
 * - LOWER layer → LED_3
 * - RAISE layer → LED_4
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Update LED states based on active layers
 *
 * This function checks which layers are currently active and updates
 * the LED states accordingly. Each layer can be associated with one
 * or more LEDs.
 */
void update_layer_leds(void);

#ifdef __cplusplus
}
#endif
