/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cmsis_os.h"
#include "gpio_driver.h"
#include "memory_api.h"
#include "led_api.h"
#include "led_app.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define LED_APP_QUEUE_SIZE 10
#define LED_APP_MAX_TIME          60
#define LED_APP_MIN_TIME          1
#define LED_APP_MIN_BLINK_FREQ    1
#define LED_APP_MAX_BLINK_FREQ    100
#define LED_APP_QUEUE_WAIT_TIME   100
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct {
    osTimerId_t timer_id;
    uint32_t toggle_count;
} sLedAppWorkData_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osThreadAttr_t g_led_app_thread_attr = {
    .name = "LED_APP_Thread",
    .stack_size = 4 * 96,
    .priority = osPriorityNormal,
};

static const eLedApiNameEnum_t g_led_app_enums[eLedApi_Last] = {
    eLedApi_GpsFix,
    eLedApi_Status
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static sLedAppWorkData_t g_led_app_dynamic_lut[eLedApi_Last] = {
    [eLedApi_GpsFix] = {.timer_id = NULL, .toggle_count = 0, },
    [eLedApi_Status] = {.timer_id = NULL, .toggle_count = 0, }
};
static osMessageQueueId_t g_led_app_queue = NULL;
static osThreadId_t g_led_app_thread_id = NULL;
static sLedAppTask_t g_received_cmd = {0};
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void LED_Blink_Callback(void *argument);
static void LED_APP_Thread(void *argument);
static bool LED_APP_Blink(uint32_t led, uint32_t total_time, uint32_t frequency);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void LED_Blink_Callback (void *argument) {
    eLedApiNameEnum_t led = *((eLedApiNameEnum_t *)argument);
    if (g_led_app_dynamic_lut[led].toggle_count > 0) {
        LED_API_Toggle(led);
        g_led_app_dynamic_lut[led].toggle_count--;
    } else {
        osTimerStop(g_led_app_dynamic_lut[led].timer_id);
        LED_API_TurnOff(led);
    }
}

static bool LED_APP_Blink (uint32_t led, uint32_t total_time, uint32_t frequency) {
    if ((led < eLedApi_First) || (led >= eLedApi_Last) || (frequency == 0)) {
        return false;
    }
    if (g_led_app_dynamic_lut[led].timer_id == NULL) {
        return false;
    }
    uint32_t period = 1000 / frequency;
    g_led_app_dynamic_lut[led].toggle_count = total_time * frequency;
    return (osTimerStart(g_led_app_dynamic_lut[led].timer_id, period) == osOK);
}

static void LED_APP_Thread (void *argument) {
    while (1) {
        if (osMessageQueueGet(g_led_app_queue, &g_received_cmd, NULL, osWaitForever) != osOK) {
            continue;
        }
        switch (g_received_cmd.task) {
            case eLedAppTask_Set: {
                sLedBasicCommandParams_t *cmd_params = (sLedBasicCommandParams_t *)g_received_cmd.data;
                LED_API_TurnOn(cmd_params->led_number);
                break;
            }
            case eLedAppTask_Reset: {
                sLedBasicCommandParams_t *cmd_params = (sLedBasicCommandParams_t *)g_received_cmd.data;
                LED_API_TurnOff(cmd_params->led_number);
                break;
            }
            case eLedAppTask_Toggle: {
                sLedBasicCommandParams_t *cmd_params = (sLedBasicCommandParams_t *)g_received_cmd.data;
                LED_API_Toggle(cmd_params->led_number);
                break;
            }
            case eLedAppTask_Blink: {
                sBlinkCommandParams_t *blink_params = (sBlinkCommandParams_t *)g_received_cmd.data;
                LED_APP_Blink(blink_params->led_number, blink_params->time, blink_params->frequency);
                break;
            }
        }
        Memory_API_Free(g_received_cmd.data);
    }
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool LED_APP_Init (void) {
    if (g_led_app_queue == NULL) {
        g_led_app_queue = osMessageQueueNew(LED_APP_QUEUE_SIZE, sizeof(sLedAppTask_t), NULL);
        if (g_led_app_queue == NULL) {
            return false;
        }
    }
    for (eLedApiNameEnum_t led = eLedApi_First; led < eLedApi_Last; led++) {
        if (g_led_app_dynamic_lut[led].timer_id == NULL) {
            g_led_app_dynamic_lut[led].timer_id = osTimerNew(LED_Blink_Callback, osTimerPeriodic,(void *)&g_led_app_enums[led], NULL);
            if (g_led_app_dynamic_lut[led].timer_id == NULL) {
                return false;
            }
        }
    }
    if (g_led_app_thread_id == NULL) {
        g_led_app_thread_id = osThreadNew(LED_APP_Thread, NULL, &g_led_app_thread_attr);
        if (g_led_app_thread_id == NULL) {
            return false;
        }
    }
    return true;
}

bool LED_APP_AddTask (const sLedAppTask_t *params) {
    if (params == NULL) {
        return false;
    }
    if (params->data == NULL) {
        return false;
    }
    if ((params->task < eLedAppTask_First) || (params->task >= eLedAppTask_Last)) {
        return false;
    }
    if (g_led_app_queue == NULL) {
        return false;
    }
    osStatus_t status = osMessageQueuePut(g_led_app_queue, params, 0, LED_APP_QUEUE_WAIT_TIME);
    return (status == osOK);
}

bool LED_APP_IsTimeValid (uint32_t time) {
    return ((time >= LED_APP_MIN_TIME) && (time <= LED_APP_MAX_TIME));
}

bool LED_APP_IsFrequencyValid (uint32_t frequency) {
    return ((frequency >= LED_APP_MIN_BLINK_FREQ) && (frequency <= LED_APP_MAX_BLINK_FREQ));
}
