
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

#include "math.h"
/* MACROS --------------------------------------------------------------------*/
#define TIME_STRING_SIZE			5
#define DATE_STRING_SIZE			10
#define TEMPRETURE_WORD_SIZE		5

#define WEATHER_ICON_STIRNG_COUNT	18
#define WEATHER_ICON_STIRNG_LENGTH 	4
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
typedef struct
{
	char 	timeString	[5];
	char 	dateString	[12];
	char 	weatherDesc [40];
	uint8_t tempreture	[2];
	uint8_t weatherIcon	[2];


}apiWeather_t;
/* VARIABLES -----------------------------------------------------------------*/
extern QueueHandle_t apiWeather_queue;
extern int prevTime;
/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void http_json_parser(void *pvParameters);


#endif /* MAIN_API_PARSER_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
