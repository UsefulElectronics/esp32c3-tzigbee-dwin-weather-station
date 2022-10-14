
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  dwin.c
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 14, 2022
 * @brief   		:
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "dwin.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static const char *TAG = "DWIN";
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief 5a a5 05 82 52 00 10 00
 *
 * @param buffer
 * @param packetId
 * @return	The size of the formed packet, if this function returns 0 then packet has been formed.
 */

uint8_t dwinMakePacket(char* buffer, dwin_packetId_e packetId)
{
	uint8_t packetLength = 0;

	char tempBuffer[50] = {0};

	memcpy(tempBuffer, DWIN_START_BYTES, DWIN_START_BYTES_SIZE);



	tempBuffer[DWIN_OPERATION_POSISION] = DWIN_WRITE_OPERATION;

	switch (packetId)
	{
		case DWIN_WEATHER_TEXT:

			tempBuffer[DWIN_OPERATION_POSISION] = DWIN_WRITE_OPERATION;

			packetLength = strlen(buffer) + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, strlen(buffer));

			break;
		case DWIN_TEMPRETURE:
			tempBuffer[DWIN_SIZE_POSISION] = DWIN_TEMRETURE_SIZE;

			packetLength = DWIN_TEMRETURE_SIZE + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, DWIN_TEMRETURE_SIZE);
			break;
		case DWIN_TIME:
			tempBuffer[DWIN_SIZE_POSISION] = DWIN_TIME_SIZE;

			packetLength = DWIN_TIME_SIZE + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, DWIN_TIME_SIZE);

			break;
		case DWIN_DATE:
			tempBuffer[DWIN_SIZE_POSISION] = DWIN_DATE_SIZE;

			packetLength = DWIN_DATE_SIZE + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, DWIN_DATE_SIZE);


			break;
		default:
			break;
	}

	memcpy(buffer, tempBuffer, packetLength);

	return packetLength;
}

/**************************  Useful Electronics  ****************END OF FILE***/