
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  dwin.h
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 14, 2022
 * @brief   		:
 *
 ******************************************************************************/

#ifndef MAIN_DWIN_H_
#define MAIN_DWIN_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
/* MACROS --------------------------------------------------------------------*/
#define DWIN_START_BYTES				"\x5A\xA5"
#define DWIN_WEATHER_TEXT_ADDRESS		"\x53\x00"
#define DWIN_TEMPRETURE_ADDRESS			"\x52\x00"
#define DWIN_DATE_ADDRESS				"\x51\x00"
#define DWIN_TIME_ADDRESS				"\x50\x00"

#define DWIN_ADDRESS_SIZE				2
#define DWIN_START_BYTES_SIZE			2
#define DWIN_OVERHEAD_SIZE				6
#define DWIN_DATE_SIZE					10
#define DWIN_TIME_SIZE					5
#define DWIN_TEMRETURE_SIZE				2

#define DWIN_WRITE_OPERATION			0x82
#define DWIN_READ_OPERATION				0x83

#define DWIN_ADDRESS_POSISION			0x04
#define DWIN_OPERATION_POSISION			0x03
#define DWIN_SIZE_POSISION				0x02
#define DWIN_DATA_START_POSISION		0x06

/* ENUMORATIONS --------------------------------------------------------------*/
typedef enum
{
	DWIN_WEATHER_TEXT = 0,
	DWIN_TEMPRETURE,
	DWIN_TIME,
	DWIN_DATE
}dwin_packetId_e;
/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
uint8_t dwinMakePacket(char* buffer, dwin_packetId_e packetId);


#endif /* MAIN_DWIN_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
