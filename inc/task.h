/*
 * task.h
 *
 *  Created on: 29 ago. 2018
 *      Author: raul
 */

#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"  // The clue is adding this
#include "stm32f4xx_hal_rtc.h"

struct task
{
	char channel;	//From 0 to 15 are the general purpose relays.
	char weekday;	//0x01 Means Monday, 0x02 Tuesday, 0x03 Monday and Tuesday, 0x7F means all days of the week. If the MSbit is 1, then the other 7 bits means the number of days between activations being 0x81 activation every day, 0x82 every other day.
	char hour;		//From 0 to 23
	char minute;	//From 0 to 59
	//uint16_t  length;	//Length in minutes
};

//Return number of seconds pending to start this task.
int numberOfSecondsToStartTaskNotAccurate(RTC_DateTypeDef *dateNow, RTC_TimeTypeDef *timeNow, struct task *ltask);


/*


//Return date and time when the task has to start.
//pending


//Returns when will be the time to turn off this task.
int timeToTurnOff(RTC_DateTypeDef dateNow, RTC_TimeTypeDef timeNow, struct task ltask);

// RTC_TimeTypeDef

// RTC_DateTypeDef

//
*/
#endif /* TASK_H_ */
