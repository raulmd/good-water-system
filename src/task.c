/*
 * task.c
 *
 *  Created on: 31 ago. 2018
 *      Author: raul
 */

#include "task.h"

int taskScheduleTypeIsWeekly(struct task *ltask)
{
	if((ltask->weekday & 0x80) == 0)
		return(1);
	return(0);
}

/**
 *
 * @param today Specifies the RTC Date WeekDay. This parameter can be a value of @ref RTC_WeekDay_Definitions
 * @param encodedStartDay
 * @return
 */
int calculateDistanceInDays(char today, char encodedStartDay)
{
	//day = ltask->weekday&0x7F;
	char NumberOfDays = 0;
	char day = today;
	char dayMask = 1<<(day-1);
		while (day>0)
		{
			if (dayMask & encodedStartDay)
				break;
			day--;
			dayMask = dayMask>>1;
		}
		if(day!=0)
		{
			NumberOfDays = today - day;
		} else
		{
			day = 7;
			dayMask = 1<<(day-1);
			while (day > today)
			{
				if (dayMask & encodedStartDay)
					break;
				day--;
				dayMask = dayMask>>1;
			}
			if(day > today)
				NumberOfDays = (today + 7) - day;
			else
				NumberOfDays = 7; //This can never be.
		}
		return(NumberOfDays);
}

int numberOfSecondsToStartTaskNotAccurate(RTC_DateTypeDef *dateNow, RTC_TimeTypeDef *timeNow, struct task *ltask)
{
	char NumberOfDays;
	int NumberOfSeconds;


	if (taskScheduleTypeIsWeekly(ltask))
	{
		NumberOfDays = calculateDistanceInDays(dateNow->WeekDay, ltask->weekday);
		NumberOfSeconds = (ltask->hour*3600 + ltask->minute*60) - (timeNow->Hours*3600 + timeNow->Minutes*60);


		//Number of days has been calculated, can be a number between 0 and 6. In case it is 0, it could be problematic.
	}


	}
return (-1);
}



