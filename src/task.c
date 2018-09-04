/*
 * task.c
 *
 *  Created on: 31 ago. 2018
 *      Author: raul
 */

#include "task.h"
#define CHANNEL_BUF_SIZE (1024)

unsigned char buf[CHANNEL_BUF_SIZE];
unsigned int bufIndex = 0;

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

/**
 *
 * @param today  Specifies the RTC Date WeekDay. This parameter can be a value of @ref RTC_WeekDay_Definitions
 * @return
 */
char nextDayRTC(char today)
{
	if(today == RTC_WEEKDAY_SUNDAY)
	{
		return(RTC_WEEKDAY_MONDAY);
	}
	return(today + 1);
}

int numberOfSecondsToStartTaskNotAccurate(RTC_DateTypeDef *dateNow, RTC_TimeTypeDef *timeNow, struct task *ltask)
{
	char NumberOfDays;
	int NumberOfSeconds;


	if (taskScheduleTypeIsWeekly(ltask))
	{
		NumberOfDays = calculateDistanceInDays(dateNow->WeekDay, ltask->weekday);
		//Number of days has been calculated, can be a number between 0 and 6. In case it is 0, it could be problematic.
		NumberOfSeconds = (int)NumberOfDays*(24*3600) + (ltask->hour*3600 + ltask->minute*60) - (timeNow->Hours*3600 + timeNow->Minutes*60);
		if(NumberOfSeconds<0)
		{
			NumberOfDays = calculateDistanceInDays(nextDayRTC(dateNow->WeekDay), ltask->weekday);
			NumberOfSeconds = (int)NumberOfDays*(24*3600) + (ltask->hour*3600 + ltask->minute*60) - (timeNow->Hours*3600 + timeNow->Minutes*60);
		}
		return(NumberOfSeconds);
	}
	return(-1); // This happens, for example, when the schedule type is not implemented yet.
}


/**
 * In this memory, the fist byte of every record always identify what kind of struct it is, so there is no problem knowing the size of the data.
 * @return -1 when no space is available
 * 			A positive number with the index of the first byte available when there is space.
 */
unsigned int getIndexOfFirstAvailableSlot()
{
	//The search starts from the top.
	uint32_t i = 0;
	while (i<CHANNEL_BUF_SIZE)
	{
		if (buf[i] == 0xFF)
			return(i);
		else
		{
			if ((buf[i]>=0) & (buf[i]<=15))
			{
				i += sizeof(struct task);
			}
			else
			{
				return(-1); //A function not implemented
			}
		}
	}
	return(-1);
}
