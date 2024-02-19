/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "ring_buffer.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
 
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
bool GenericRingBuffer_Init (sGenericRingBuffer_t *ringBuffer, size_t element_size, size_t capacity) {
    ringBuffer->buffer = malloc(capacity * element_size);
    if (ringBuffer->buffer == NULL) {
        return false;
    }
    ringBuffer->element_size = element_size;
    ringBuffer->capacity = capacity;
    ringBuffer->head = 0;
    ringBuffer->tail = 0;
    ringBuffer->count = 0;
    return true;
}
bool GenericRingBuffer_Write (sGenericRingBuffer_t *ringBuffer, const void *data) {
    if (ringBuffer->count == ringBuffer->capacity) {
        return false;
    }
    size_t index = ringBuffer->head * ringBuffer->element_size;
    memcpy((char*) ringBuffer->buffer + index, data, ringBuffer->element_size);
    ringBuffer->head = (ringBuffer->head + 1) % ringBuffer->capacity;
    ringBuffer->count++;
    return true;
}
bool GenericRingBuffer_Read (sGenericRingBuffer_t *ringBuffer, void *data) {
    if (ringBuffer->count == 0) {
        return false;
    }
    size_t index = ringBuffer->tail * ringBuffer->element_size;
    memcpy(data, (char*) ringBuffer->buffer + index, ringBuffer->element_size);
    ringBuffer->tail = (ringBuffer->tail + 1) % ringBuffer->capacity;
    ringBuffer->count--;
    return true;
}