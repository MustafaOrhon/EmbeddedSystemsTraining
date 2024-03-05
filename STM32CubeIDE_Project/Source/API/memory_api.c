/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdlib.h>
#include "cmsis_os.h"
#include "memory_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osMutexAttr_t g_mem_mutex_attr = {
    "Memory Mutex",
    osMutexRecursive,
    NULL,
    0U
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static int32_t g_mem_alloc_counter = 0;
static osMutexId_t g_mem_mutex_id = NULL;
static bool g_kernel_running = false;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool Memory_API_Init (void) {
    if (osKernelGetState() == osKernelInactive) {
        return false;
    }
    g_mem_mutex_id = osMutexNew(&g_mem_mutex_attr);
    if (g_mem_mutex_id == NULL) {
        return false;
    }
    return true;
}

void *Memory_API_Alloc (bool is_calloc, size_t count, size_t size) {
    if (g_kernel_running == false) {
        osKernelState_t kernel_state = osKernelGetState();
        if (kernel_state != osKernelRunning) {
            return NULL;
        }
        g_kernel_running = true;
    }
    if (osThreadGetId() == NULL) {
        return NULL;
    }
    if ((count == 0) || (size == 0)) {
        return NULL;
    }
    if (osMutexAcquire(g_mem_mutex_id, osWaitForever) != osOK) {
        return NULL;
    }
    void *ptr = NULL;
    ptr = is_calloc ? calloc(count, size) : malloc(size);
    if (ptr != NULL) {
        g_mem_alloc_counter++;
    }
    if (osMutexRelease(g_mem_mutex_id) != osOK) {
        return NULL;
    }
    return ptr;
}

void Memory_API_Free (void *ptr) {
    if ((g_kernel_running == false) || (osThreadGetId() == NULL)) {
        return;
    }
    if (ptr == NULL) {
        return;
    }
    if (osMutexAcquire(g_mem_mutex_id, osWaitForever) != osOK) {
        return;
    }
    free(ptr);
    g_mem_alloc_counter--;
    if (osMutexRelease(g_mem_mutex_id) != osOK) {
        return;
    }
}

int32_t Memory_API_GetAllocCounter (void) {
    if (g_kernel_running == true) {
        return g_mem_alloc_counter;
    }
    return -1;
}
