#ifndef SOURCE_APP_NETWORK_APP_H_
#define SOURCE_APP_NETWORK_APP_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdint.h"
#include "stdbool.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef enum {
    eNetworkAPPTask_First = 0,
    eNetworkAPPTask_Connect = eNetworkAPPTask_First,
    eNetworkAPPTask_Send,
    eNetworkAPPTask_Disconnect,
    eNetworkAPPTask_Last,
} eNetworkAPPTaskEnum_t;

typedef struct {
    char ip[16];
    uint16_t port;
} sNetworkAppConnectParams;

typedef struct {
    const uint8_t *data;
    size_t data_length;
} sNetworkAppSendParams;

typedef struct {
    eNetworkAPPTaskEnum_t task;
    void *params;
} sNetworkAPPTaskParams_t;
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool Network_APP_Init (void);
bool Network_APP_AddTask(const sNetworkAPPTaskParams_t *params);
bool Network_APP_QueueSocketId (uint8_t *socket_id);
#endif /* SOURCE_APP_NETWORK_APP_H_ */
