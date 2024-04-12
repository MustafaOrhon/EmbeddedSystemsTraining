/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory_api.h"
#include "gnss_api.h"
#include "gnss_cmd_handler.h"
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
bool GNSS_CMD_GNGGAHandler(const sCommandParams_t *cmd_params) {
    if (!CMD_API_CheckCmdParams(cmd_params)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid command parameters.\r");
        return false;
    }
    const char *delimiter = ",";
    char *saveptr = NULL;
    char *token = strtok_r((char*) cmd_params->params, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing timestamp parameter.\r");
        return false;
    }
    double timestamp = atof(token);
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing latitude parameter.\r");
        return false;
    }
    double lat_deg = atoi(token) / 100;
    double lat_min = atof(token) - (lat_deg * 100);
    double latitude = lat_deg + (lat_min / 60);
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL || (*token != 'N' && *token != 'S')) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid latitude direction.\r");
        return false;
    }
    if (*token == 'S') {
        latitude = -latitude;
    }
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing longitude parameter.\r");
        return false;
    }
    double lon_deg = atoi(token) / 100;
    double lon_min = atof(token) - (lon_deg * 100);
    double longitude = lon_deg + (lon_min / 60);
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL || (*token != 'E' && *token != 'W')) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid longitude direction.\r");
        return false;
    }
    if (*token == 'W') {
        longitude = -longitude;
    }
    GNSS_API_UpdateGNSSCoordinates(timestamp, latitude, longitude);
    return true;
}

