/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "cmsis_os.h"
#include "memory_api.h"
#include "network_app.h"
#include "record_sending.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define RECORD_SENDING_APP_QUEUE_SIZE 10
#define RECORD_SENDING_APP_QUEUE_WAIT_TIME 300
#define RECORD_SENDING_APP_TIMER_PERIOD      10000
#define RECORD_SENDING_APP_TEXT          ("Hello Teltonika!\r\n")
#define RECORD_SENDING_APP_TEXT_LENGTH   (sizeof(RECORD_SENDING_APP_TEXT) - 1)
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osThreadAttr_t g_record_sendig_app_thread_attr = {
    .name = "RECORD_SENDING_APP_Thread",
    .stack_size = 4 * 80,
    .priority = osPriorityNormal,
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osMessageQueueId_t g_record_sending_app_queue = NULL;
static osThreadId_t g_record_sending_app_thread_id = NULL;
static osTimerId_t g_record_sending_app_timer_id = NULL;
static sRecordSendingAPPTask_t g_received_task = {0};
static sNetworkAPPTaskParams_t g_sending_network_task = {0};
static sNetworkAppConnectParams g_connect_params = {0};
static sNetworkAppSendParams g_send_params = {
    .data = (const uint8_t*) RECORD_SENDING_APP_TEXT,
    .data_length = RECORD_SENDING_APP_TEXT_LENGTH
};
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void RecordSending_APP_Thread (void *argument);
static void RecordSending_APP_TimerCallback (void *argument);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void RecordSending_APP_TimerCallback (void *argument) {
    g_sending_network_task.params = &g_connect_params;
    g_sending_network_task.task = eNetworkAPPTask_Connect;
    Network_APP_AddTask(&g_sending_network_task);
    g_sending_network_task.params = &g_send_params;
    g_sending_network_task.task = eNetworkAPPTask_Send;
    Network_APP_AddTask(&g_sending_network_task);
    g_sending_network_task.params = NULL;
    g_sending_network_task.task = eNetworkAPPTask_Disconnect;
    Network_APP_AddTask(&g_sending_network_task);
}

static void RecordSending_APP_Thread (void *argument) {
    while (1) {
        if (osMessageQueueGet(g_record_sending_app_queue, &g_received_task, NULL, osWaitForever) != osOK) {
            continue;
        }
        if (g_received_task.params == NULL) {
            continue;
        }
        switch (g_received_task.task) {
            case eRecordSendingAPPTask_Start: {
                strncpy(g_connect_params.ip, g_received_task.params->ip, sizeof(g_connect_params.ip) - 1);
                g_connect_params.ip[sizeof(g_connect_params.ip) - 1] = '\0';
                g_connect_params.port = g_received_task.params->port;
                osTimerStart(g_record_sending_app_timer_id, RECORD_SENDING_APP_TIMER_PERIOD);
                break;
            }
            case eRecordSendingAPPTask_Stop: {
                osTimerStop(g_record_sending_app_timer_id);
                break;
            }
        }
        Memory_API_Free(g_received_task.params);
    }
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool RecordSending_APP_Init (void) {
    if (g_record_sending_app_queue == NULL) {
        g_record_sending_app_queue = osMessageQueueNew(RECORD_SENDING_APP_QUEUE_SIZE, sizeof(sRecordSendingAPPTask_t), NULL);
        if (g_record_sending_app_queue == NULL) {
            return false;
        }
    }
    if (g_record_sending_app_timer_id == NULL) {
        g_record_sending_app_timer_id = osTimerNew(RecordSending_APP_TimerCallback, osTimerPeriodic, NULL, NULL);
        if (g_record_sending_app_timer_id == NULL) {
            return false;
        }
    }
    if (g_record_sending_app_thread_id == NULL) {
        g_record_sending_app_thread_id = osThreadNew(RecordSending_APP_Thread, NULL, &g_record_sendig_app_thread_attr);
        if (g_record_sending_app_thread_id == NULL) {
            return false;
        }
    }
    return true;
}
bool RecordSending_APP_AddTask (const sRecordSendingAPPTask_t *params) {
    if (params == NULL || params->params == NULL) {
        return false;
    }
    if ((params->task < eRecordSendingAPPTask_First) || (params->task >= eRecordSendingAPPTask_Last)) {
        return false;
    }
    if (g_record_sending_app_queue == NULL) {
        return false;
    }
    osStatus_t status = osMessageQueuePut(g_record_sending_app_queue, params, 0, RECORD_SENDING_APP_QUEUE_WAIT_TIME);
    return (status == osOK);
}
