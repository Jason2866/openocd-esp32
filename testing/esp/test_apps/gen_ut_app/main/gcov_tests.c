#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "gen_ut_app.h"
#include "esp_app_trace.h"

#if CONFIG_ESP32_GCOV_ENABLE

void gcov_dummy_func(void);

void gcov_task(void *pvParameter)
{
    bool dump = (bool)pvParameter;
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        printf("Toggle LED\n");
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gcov_dummy_func();
        if (dump) {
            // Dump gcov data
            printf("Ready to dump GCOV data...\n");
            esp_gcov_dump();
            printf("GCOV data have been dumped.\n");
        }
    }
}
#endif //CONFIG_ESP32_GCOV_ENABLE

ut_result_t gcov_test_do(int test_num, int core_num)
{
    if (core_num < 0 || core_num >= portNUM_PROCESSORS)
        core_num = portNUM_PROCESSORS-1;
#if CONFIG_ESP32_GCOV_ENABLE
    if (TEST_ID_MATCH("test_gcov.GcovTests*.test_on_the_fly*", test_num)) {
        xTaskCreatePinnedToCore(&gcov_task, "gcov_task", 8192, (void *)false, 5, NULL, core_num);
    } else if (TEST_ID_MATCH("test_gcov.GcovTests*.test_simple*", test_num)) {
        xTaskCreatePinnedToCore(&gcov_task, "gcov_task", 8192, (void *)true, 5, NULL, core_num);
    } else {
        return UT_UNSUPPORTED;
    }
    return UT_OK;
#endif //CONFIG_ESP32_GCOV_ENABLE
    return UT_UNSUPPORTED;
}
