#include <FreeRTOS.h>
#include <task.h>
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
#include <platform/platform_stdlib.h>
#include <platform_opts.h>
#elif defined(CONFIG_PLATFORM_AMEBADPLUS)
#include <platform_stdlib.h>
#endif
#include <basic_types.h>
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
#include <wifi_constants.h>
#include <wifi/wifi_conf.h>
#elif defined(CONFIG_PLATFORM_AMEBADPLUS)
#include <rtw_wifi_constants.h>
#include <wifi_intf_drv_to_app_basic.h>
#endif

#if defined(CONFIG_EXAMPLE_MATTER_CHIPTEST) && CONFIG_EXAMPLE_MATTER_CHIPTEST
extern void ChipTest(void);

static void example_matter_task_thread(void *pvParameters)
{
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
    while (!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE)))
#elif defined(CONFIG_PLATFORM_AMEBADPLUS)
    while (!(wifi_is_running(WLAN0_IDX) || wifi_is_running(WLAN1_IDX))) 
#endif
    {
        vTaskDelay(500);
    }

#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
    wifi_set_autoreconnect(0); //Disable default autoreconnect
#elif defined(CONFIG_PLATFORM_AMEBADPLUS)
    wifi_config_autoreconnect(0);
#endif

#if defined(CONFIG_PLATFORM_8710C)
    matter_timer_init(); //Currently 8721D cannot use this implementation
#endif
    ChipTest();

    vTaskDelete(NULL);
    return;
}

void example_matter_task(void)
{
    if (xTaskCreate(example_matter_task_thread, ((const char*)"example_matter_task_thread"), 2048, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        printf("\n\r%s xTaskCreate(example_matter_task_thread) failed", __FUNCTION__);
    }
}
#endif /* CONFIG_EXAMPLE_MATTER_CHIPTEST */
