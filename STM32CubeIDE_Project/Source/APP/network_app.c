/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "cmsis_os.h"
#include "tcp_api.h"
#include "debug_api.h"
#include "memory_api.h"
#include "network_app.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define NETWORK_APP_QUEUE_SIZE        10
#define NETWORK_APP_QUEUE_WAIT_TIME   300
#define NETWORK_APP_TIMER_PERIOD      1000
#define NETWORK_APP_TEXT          ("Hello Teltonika!\r\n")
#define NETWORK_APP_TEXT_LENGTH   (sizeof(NETWORK_APP_TEXT) - 1)
DEFINE_DEBUG_MODULE_TAG(NETWORK_APP);
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osThreadAttr_t g_network_app_thread_attr = {
    .name = "NETWORK_APP_Thread",
    .stack_size = 4 * 200,
    .priority = osPriorityAboveNormal,
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osMessageQueueId_t g_network_app_queue = NULL;
static osThreadId_t g_network_app_thread_id = NULL;
static sNetworkAPPTaskParams_t g_received_task = {0};
static osMessageQueueId_t g_network_app_socket_queue = NULL;
static uint8_t g_socket_id = 0;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void Network_APP_Thread (void *argument);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void Network_APP_Thread (void *argument) {
    while (1) {
        if (osMessageQueueGet(g_network_app_queue, &g_received_task, NULL, osWaitForever) == osOK) {
            switch (g_received_task.task) {
                case eNetworkAPPTask_Connect: {
                    sNetworkAppConnectParams *connect_params = (sNetworkAppConnectParams *)g_received_task.params;
                    if (TCP_API_Connect(connect_params->ip, connect_params->port) == false) {
                        TRACE_WARNING("Connecting to TCP Failed!\r");
                    }
                    osMessageQueueGet(g_network_app_socket_queue, &g_socket_id, NULL, NETWORK_APP_QUEUE_WAIT_TIME);
                    break;
                }
                case eNetworkAPPTask_Send: {
                    sNetworkAppSendParams *send_params = (sNetworkAppSendParams *)g_received_task.params;
                    TCP_API_Send(send_params->data, send_params->data_length, g_socket_id);
                    break;
                }
                case eNetworkAPPTask_Disconnect: {
                    if (TCP_API_Disconnect(g_socket_id) == false) {
                        TRACE_WARNING("Disconnecting from TCP Failed!\r");
                    }
                    break;
                }
            }
        }
    }
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool Network_APP_Init (void){
    if (g_network_app_queue == NULL) {
        g_network_app_queue = osMessageQueueNew(NETWORK_APP_QUEUE_SIZE, sizeof(sNetworkAPPTaskParams_t), NULL);
        if (g_network_app_queue == NULL) {
            return false;
        }
    }
    if (g_network_app_socket_queue == NULL) {
        g_network_app_socket_queue = osMessageQueueNew(NETWORK_APP_QUEUE_SIZE, sizeof(uint8_t), NULL);
        if (g_network_app_socket_queue == NULL) {
            return false;
        }
    }
    if (g_network_app_thread_id == NULL) {
        g_network_app_thread_id = osThreadNew(Network_APP_Thread, NULL, &g_network_app_thread_attr);
        if (g_network_app_thread_id == NULL) {
            return false;
        }
    }
    return true;

}

bool Network_APP_AddTask (const sNetworkAPPTaskParams_t *params) {
    if (params == NULL) {
        return false;
    }
    if ((params->task < eNetworkAPPTask_First) || (params->task >= eNetworkAPPTask_Last)) {
        return false;
    }
    if (g_network_app_queue == NULL) {
        return false;
    }
    osStatus_t status = osMessageQueuePut(g_network_app_queue, params, 0, NETWORK_APP_QUEUE_WAIT_TIME);
    return (status == osOK);
}

bool Network_APP_QueueSocketId (uint8_t *socket_id) {
    if (socket_id == NULL) {
        return false;
    }
    osStatus_t status = osMessageQueuePut(g_network_app_socket_queue, socket_id, 0, NETWORK_APP_QUEUE_WAIT_TIME);
    return (status == osOK);
}
