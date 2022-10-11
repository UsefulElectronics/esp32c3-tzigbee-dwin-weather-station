
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  api_parser.h
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 10, 2022
 * @brief   		:
 *
 ******************************************************************************/

#ifndef MAIN_API_PARSER_H_
#define MAIN_API_PARSER_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"

#include "unix_timestamp.h"
#include "http_request.h"
#include "cJSON.h"
/* MACROS --------------------------------------------------------------------*/

/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void http_json_parser(void *pvParameters);


#endif /* MAIN_API_PARSER_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
