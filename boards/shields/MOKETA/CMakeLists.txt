zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_sources(app PRIVATE src/behaviors/behavior_check_battery.c)
target_sources(app PRIVATE src/status_led.c)
target_sources(app PRIVATE src/layer_led.c)
