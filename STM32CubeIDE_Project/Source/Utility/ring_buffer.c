/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdlib.h>
#include "memory_api.h"
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
sRingBuffer_t *Ring_Buffer_Init (size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }
    sRingBuffer_t *ring_buffer = (sRingBuffer_t *)Memory_API_Calloc(1, sizeof(sRingBuffer_t));
    if (ring_buffer == NULL) {
        return NULL;
    }
    ring_buffer->buffer = (uint8_t *)Memory_API_Calloc(capacity, sizeof(uint8_t));
    if (ring_buffer->buffer == NULL) {
        Memory_API_Free(ring_buffer);
        return NULL;
    }
    ring_buffer->capacity = capacity;
    return ring_buffer;
}

bool Ring_Buffer_Write (sRingBuffer_t *ring_buffer, uint8_t data) {
    if (ring_buffer == NULL) {
        return false;
    }
    if (ring_buffer->count == ring_buffer->capacity) {
        ring_buffer->tail = (ring_buffer->tail + 1) % ring_buffer->capacity;
    }
    ring_buffer->buffer[ring_buffer->head] = data;
    ring_buffer->head = (ring_buffer->head + 1) % ring_buffer->capacity;
    if (ring_buffer->count < ring_buffer->capacity) {
        ring_buffer->count++;
    }
    return true;
}

bool Ring_Buffer_Read (sRingBuffer_t *ring_buffer, uint8_t *data) {
    if ((ring_buffer == NULL) || (data == NULL)) {
        return false;
    }
    if (ring_buffer->count == 0) {
        return false;
    }
    *data = ring_buffer->buffer[ring_buffer->tail];
    ring_buffer->tail = (ring_buffer->tail + 1) % ring_buffer->capacity;
    ring_buffer->count--;
    return true;
}
