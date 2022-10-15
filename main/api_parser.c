
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  api_parser.c
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 10, 2022
 * @brief   		:
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "api_parser.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "JSON";

QueueHandle_t apiWeather_queue;

int prevTime = 0;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void parsedTimeUpdate			(char* time, int min, int hour);

static void parsedDateUpdate			(char* date, int day, int month, int year);

static void parsedTempretureUpdate		(uint8_t* weatherTemp, float tempreture);

static void parsedWeatherString			(char* weatherDesc, char* description);
/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief
 *
 * @param pvParameters
 */
void http_json_parser(void *pvParameters)
{
	hHttpPort_t hHttpResponse;

	struct tm dateTime;

	apiWeather_t WeatherParam;

	apiWeather_queue = xQueueCreate(5, sizeof(apiWeather_t));

	while(1)
	{
		if(xQueueReceive(httpRx_queue, (void * )&hHttpResponse, (portTickType)portMAX_DELAY))
		{
//			ESP_LOGI(TAG, "Packet size=%d JSON: %s.", hHttpResponse.packetSize, hHttpResponse.packetBuffer);

		    cJSON *jElement 		= NULL;

		    cJSON *jWeatherArray	= NULL;

		    cJSON *ResponseJson 	= cJSON_Parse(hHttpResponse.packetBuffer);

		    if(ResponseJson != NULL)
		    {
			    jElement = cJSON_GetObjectItemCaseSensitive(ResponseJson, "dt");

//			    ESP_LOGI(TAG, "time: %d.", jElement->valueint);

			    prevTime = jElement->valueint;

			    time_unixStampToLocalTime(jElement->valueint, &dateTime);

			    dateTime.tm_mon += 1;
			    dateTime.tm_year += 1900;
			    dateTime.tm_hour += 3;
			    if(dateTime.tm_hour >= 24)
			    {
			    	dateTime.tm_hour -= 24;
			    }


			    parsedTimeUpdate(WeatherParam.timeString, dateTime.tm_min, dateTime.tm_hour);

			    parsedDateUpdate(WeatherParam.dateString, dateTime.tm_mday, dateTime.tm_mon, dateTime.tm_year);

			    jElement = cJSON_GetObjectItemCaseSensitive(ResponseJson, "main");



			    jElement->child = cJSON_GetObjectItemCaseSensitive(jElement, "temp");

//			    ESP_LOGI(TAG, "temp: %.02f", jElement->child->valuedouble);

			    parsedTempretureUpdate(WeatherParam.tempreture, jElement->child->valuedouble);

			    jWeatherArray = cJSON_GetObjectItemCaseSensitive(ResponseJson, "weather");

			    jElement = cJSON_GetObjectItemCaseSensitive(jWeatherArray->child, "description");


//				if(cJSON_IsString(jElement))
//				{
//					ESP_LOGI(TAG, "Weather Desc: %s",jElement->valuestring);
//				}

				parsedWeatherString(WeatherParam.weatherDesc, jElement->valuestring);



				xQueueSendToBack(apiWeather_queue, (void *)&WeatherParam, portMAX_DELAY);

		    }
		}
	}
}
/**
 * @brief
 *
 * @param time
 * @param min
 * @param hour
 */
static void parsedTimeUpdate(char* time, int min, int hour)
{
	sprintf(time, "%02d:%02d",hour, min);
}
/**
 * @brief
 *
 * @param date
 * @param day
 * @param month
 * @param year
 */
static void parsedDateUpdate(char* date, int day, int month, int year)
{
	sprintf(date, "%02d.%02d.%04d",day, month, year);
}
/**
 * @brief
 *
 * @param weatherTemp
 * @param tempreture
 */
static void parsedTempretureUpdate(uint8_t* weatherTemp, float tempreture)
{
	weatherTemp[0] = 0;
	weatherTemp[1] = (uint8_t) floor(tempreture);
}
/**
 * @brief
 *
 * @param weatherDesc
 * @param description
 */
static void parsedWeatherString(char* weatherDesc, char* description)
{
	strcpy(weatherDesc, description);
}
/**************************  Useful Electronics  ****************END OF FILE***/
