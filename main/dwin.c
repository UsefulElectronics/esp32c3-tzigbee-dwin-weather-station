
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
 * @brief 	Example of DWIN display packet: 5a a5 05 82 52 00 10 00 (Hex)
 * 			Start Bytes: 	5a a5
 * 			PayloadSize:	05
 * 			Write Command:	82
 * 			Target Address:	52 00
 * 			Data:			10 00
 *
 *			This function will form the DWIN display packet according to passed packetId
 *
 * @param 	buffer: 	This buffer will have the data to be sent to display. It will be used for packet formation.
 * 						The formed packet will be also placed in the same buffer.
 *
 * @param 	packetId:	Select the address, packet size this packet is going to be sent to.
 *
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

			tempBuffer[DWIN_SIZE_POSISION] = strlen(buffer) + 3;

			packetLength = 40;

			memcpy(&tempBuffer[DWIN_ADDRESS_POSISION], DWIN_WEATHER_TEXT_ADDRESS, DWIN_ADDRESS_SIZE);

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, strlen(buffer));

			break;
		case DWIN_TEMPRETURE:
			tempBuffer[DWIN_SIZE_POSISION] = DWIN_TEMRETURE_SIZE + 3;

			packetLength = DWIN_TEMRETURE_SIZE + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_ADDRESS_POSISION], DWIN_TEMPRETURE_ADDRESS, DWIN_ADDRESS_SIZE);

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, DWIN_TEMRETURE_SIZE);
			break;
		case DWIN_TIME:
			tempBuffer[DWIN_SIZE_POSISION] = DWIN_TIME_SIZE + 3;

			packetLength = DWIN_TIME_SIZE + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_ADDRESS_POSISION], DWIN_TIME_ADDRESS, DWIN_ADDRESS_SIZE);

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, DWIN_TIME_SIZE);

			break;
		case DWIN_DATE:
			tempBuffer[DWIN_SIZE_POSISION] = DWIN_DATE_SIZE + 3;

			packetLength = DWIN_DATE_SIZE + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_ADDRESS_POSISION], DWIN_DATE_ADDRESS, DWIN_ADDRESS_SIZE);

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, DWIN_DATE_SIZE);
			break;

		case DWIN_ICON:
			tempBuffer[DWIN_SIZE_POSISION] = DWIN_ICON_SIZE + 3;

			packetLength = DWIN_ICON_SIZE + DWIN_OVERHEAD_SIZE;

			memcpy(&tempBuffer[DWIN_ADDRESS_POSISION], DWIN_ICON_ADDRESS, DWIN_ADDRESS_SIZE);

			memcpy(&tempBuffer[DWIN_DATA_START_POSISION], buffer, DWIN_DATE_SIZE);

			break;
		default:
			break;
	}


	memcpy(buffer, tempBuffer, packetLength);

	return packetLength;
}

/**************************  Useful Electronics  ****************END OF FILE***/
