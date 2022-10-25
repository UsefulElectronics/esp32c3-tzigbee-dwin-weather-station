
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

const char weatherIconArray[WEATHER_ICON_STIRNG_COUNT][WEATHER_ICON_STIRNG_LENGTH] =
{
	"01d",
	"02d",
	"03d",
	"04d",
	"09d",
	"10d",
	"11d",
	"13d",
	"50d",
	"01n",
	"02n",
	"03n",
	"04n",
	"09n",
	"10n",
	"11n",
	"13n",
	"50n",

};

const uint8_t weatherIconId[WEATHER_ICON_STIRNG_COUNT] =
{
	0,
	1,
	2,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	2,
	2,
	3,
	10,
	5,
	6,
	7
};

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void parsedTimeUpdate			(char* time, int min, int hour);

static void parsedDateUpdate			(char* date, int day, int month, int year);

static void parsedTempretureUpdate		(uint8_t* weatherTemp, float tempreture);

static void parsedWeatherString			(char* weatherDesc, char* description);

static void parsedIconValue				(uint8_t* weatherIconValue ,char* weatherIconStirng);

static void parsedHumidityUpdate		(uint8_t* weatherHum, int humidity);
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
		    	//Parsing time and date value.
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

			    //Parsing temperature value.
			    jElement = cJSON_GetObjectItemCaseSensitive(ResponseJson, "main");

			    jElement->child = cJSON_GetObjectItemCaseSensitive(jElement, "temp");

			    parsedTempretureUpdate(WeatherParam.tempreture, jElement->child->valuedouble);

			    //Parsing humidity value.
			    jElement->child = cJSON_GetObjectItemCaseSensitive(jElement, "humidity");

			    parsedHumidityUpdate(WeatherParam.humidity, jElement->child->valueint);

			    ESP_LOGI(TAG, "humidity: %d.", jElement->child->valueint);

			    //Parsing weather description value.
			    jWeatherArray = cJSON_GetObjectItemCaseSensitive(ResponseJson, "weather");

			    jElement = cJSON_GetObjectItemCaseSensitive(jWeatherArray->child, "description");

				parsedWeatherString(WeatherParam.weatherDesc, jElement->valuestring);

				//Parsing weather icon value.
				jElement = cJSON_GetObjectItemCaseSensitive(jWeatherArray->child, "icon");

				parsedIconValue(WeatherParam.weatherIcon ,jElement->valuestring);

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
 * @param weatherHum
 * @param humidity
 */
static void parsedHumidityUpdate(uint8_t* weatherHum, int humidity)
{
	weatherHum[0] = 0;
	weatherHum[1] = (uint8_t) humidity;
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
/**
 * @brief 	Get the corresponding icon ID of display out of the received icon string from the API
 *
 * @param weatherIconValue
 * @param weatherIconStirng
 */
static void parsedIconValue(uint8_t* weatherIconValue ,char* weatherIconStirng)
{
	uint8_t counter = 0;

	const uint8_t maxLimit = WEATHER_ICON_STIRNG_COUNT + 1;


	for(;counter < maxLimit; ++counter)
	{
		if(0 == memcmp(weatherIconStirng, weatherIconArray[counter], 3))
		{
			ESP_LOGI(TAG, "icon found %s", weatherIconStirng);
			break;
		}
	}
	if(maxLimit != counter )
	{
		weatherIconValue[0] = 0;
		weatherIconValue[1] = weatherIconId[counter] ;
	}
}


/**************************  Useful Electronics  ****************END OF FILE***/
