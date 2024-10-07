#include <platform_stdlib.h>
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
#include <platform_opts.h>
#elif defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_AMEBASMART)
#include <platform_autoconf.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#endif

#if defined(CONFIG_MATTER) && CONFIG_MATTER
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
#include <log_service.h>
#elif defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_AMEBASMART)
#include <atcmd_service.h>
#endif
#include <main.h>
#include <sys_api.h>
#include <wifi_conf.h>

extern u32 deinitPref(void);
#if defined(CONFIG_ENABLE_OTA_REQUESTOR) && CONFIG_ENABLE_OTA_REQUESTOR && \
    defined(CONFIG_EXAMPLE_MATTER_CHIPTEST) && CONFIG_EXAMPLE_MATTER_CHIPTEST
extern void amebaQueryImageCmdHandler();
extern void amebaApplyUpdateCmdHandler();
#endif
#if defined(CONFIG_PLATFORM_AMEBADPLUS)
#if defined(CONFIG_MATTER_SECURE) && (CONFIG_MATTER_SECURE == 1)
extern void NS_ENTRY vMatterPrintSecureHeapStatus(void);
#endif // CONFIG_PLATFORM_AMEBADPLUS
#endif // CONFIG_PLATFORM

// Queue for matter shell
QueueHandle_t shell_queue;

void fATchipapp(void *arg)
{
    /* To avoid gcc warnings */
    ( void ) arg;
    printf("xPortGetTotalHeapSize = %d\n",xPortGetTotalHeapSize());
    printf("xPortGetFreeHeapSize = %d\n",xPortGetFreeHeapSize());
    printf("xPortGetMinimumEverFreeHeapSize = %d\n",xPortGetMinimumEverFreeHeapSize());

    deinitPref();
    wifi_disconnect();
    sys_reset();
}

void fATchipapp1(void *arg)
{
#if defined(CONFIG_ENABLE_OTA_REQUESTOR) && CONFIG_ENABLE_OTA_REQUESTOR && \
    defined(CONFIG_EXAMPLE_MATTER_CHIPTEST) && CONFIG_EXAMPLE_MATTER_CHIPTEST
    printf("Calling amebaQueryImageCmdHandler\n");
    amebaQueryImageCmdHandler();
#endif
}

void fATchipapp2(void *arg)
{
#if defined(CONFIG_ENABLE_OTA_REQUESTOR) && CONFIG_ENABLE_OTA_REQUESTOR && \
    defined(CONFIG_EXAMPLE_MATTER_CHIPTEST) && CONFIG_EXAMPLE_MATTER_CHIPTEST
    (void) arg;
    printf("Chip Test: amebaApplyUpdateCmdHandler\n");

    amebaApplyUpdateCmdHandler();
#endif
}

void fATmattershell(void *arg)
{
    if (arg != NULL)
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
    {
        xQueueSend(shell_queue, arg, pdMS_TO_TICKS(10));
    }
    else
    {
        printf("No arguments provided for matter shell\n");
    }
#elif defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_AMEBASMART)
    {
        if(strcmp(arg, "factoryreset") == 0) {
            fATchipapp(NULL);
        } else if(strcmp(arg, "queryimage") == 0) {
            fATchipapp1(NULL);
        } else if(strcmp(arg, "applyupdate") == 0) {
            fATchipapp2(NULL);
#if defined(CONFIG_MATTER_SECURE) && (CONFIG_MATTER_SECURE == 1)
        } else if(strcmp(arg, "secureheapstatus") == 0) {
            vMatterPrintSecureHeapStatus();
#endif
        } else {
            xQueueSend(shell_queue, arg, pdMS_TO_TICKS(10));
        }
    }
    else
    {
        DiagPrintf("No arguments provided for matter shell, available commands:\n%s\n%s\n%s\n%s\n",
            "ATmatter factoryreset     : to factory reset the matter application",
            "ATmatter queryimage       : query image for matter ota requestor app",
            "ATmatter applyupdate      : apply update for matter ota requestor app",
            "ATmatter help             : to show other matter commands");
#if defined(CONFIG_MATTER_SECURE) && (CONFIG_MATTER_SECURE == 1)
        DiagPrintf("ATmatter secureheapstatus : to check secure heap status\n");
#endif
    }
#endif // CONFIG_PLATFORM
}

log_item_t at_matter_items[] = {
#ifndef CONFIG_INIC_NO_FLASH
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
#if ATCMD_VER == ATVER_1
    {"ATM$", fATchipapp, {NULL,NULL}},
    {"ATM%", fATchipapp1, {NULL,NULL}},
    {"ATM^", fATchipapp2, {NULL,NULL}},
    {"ATMS", fATmattershell, {NULL,NULL}},
#endif // end of #if ATCMD_VER == ATVER_1
#elif defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_AMEBASMART)
    {"matter", fATmattershell, {NULL, NULL}},
#endif // CONFIG_PLATFORM
#endif
};

void at_matter_init(void)
{
    shell_queue = xQueueCreate(3, 256); // backlog 3 commands max
#if defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_8721D)
    log_service_add_table(at_matter_items, sizeof(at_matter_items)/sizeof(at_matter_items[0]));
#elif defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_AMEBASMART)
    atcmd_service_add_table(at_matter_items, sizeof(at_matter_items)/sizeof(at_matter_items[0]));
#endif // CONFIG_PLATFORM
}

#if SUPPORT_LOG_SERVICE
log_module_init(at_matter_init);
#endif

#endif /* CONFIG_MATTER */
