
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  unix_timestamp.h
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 11, 2022
 * @brief   		:
 *
 ******************************************************************************/

///>	NOTE: This library is taken from http://git.musl-libc.org/cgit/musl/tree/src/time/__secs_to_tm.c?h=v0.9.15
/// 	so this library can be extended as needed in the future.

#ifndef _UNIX_TIMESTAMP_H_
#define _UNIX_TIMESTAMP_H_

/* INCLUDES ------------------------------------------------------------------*/
#include "time.h"
/* MACROS --------------------------------------------------------------------*/
#define LEAPOCH (946684800LL + 86400*(31+29))

#define DAYS_PER_400Y (365*400 + 97)
#define DAYS_PER_100Y (365*100 + 24)
#define DAYS_PER_4Y   (365*4   + 1)
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
int time_unixStampToLocalTime(long long t, struct tm *tm);
/* MACRO FUNCTIONS -----------------------------------------------------------*/



#endif /* SRC_SYSTEM_TIME_UNIX_TIMESTAMP_H_ */


/**************************  Useful Electronics  ****************END OF FILE***/
