/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "memory_api.h"
#include "str_utils.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
static const char g_hex_lut[16] = "0123456789ABCDEF";
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
char *StringUtils_ConvertToHex(const uint8_t *data, size_t data_len) {
    char *hex_string = (char *)Memory_API_Malloc((data_len * 2) + 1);
    if (hex_string == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < data_len; ++i) {
        unsigned char byte = data[i];
        hex_string[i * 2] = g_hex_lut[byte >> 4];
        hex_string[i * 2 + 1] = g_hex_lut[byte & 0x0F];
    }
    hex_string[data_len * 2] = '\0';
    return hex_string;
}