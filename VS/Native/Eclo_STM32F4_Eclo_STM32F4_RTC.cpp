//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "Eclo_STM32F4.h"
#include "Eclo_STM32F4_Eclo_STM32F4_RTC.h"

#include "..\..\Include\helperfunctions.h"

#define RCC_LSE_TIMEOUT_VALUE      ((uint32_t)5000) // 5000 ms
#define LSI_TIMEOUT_VALUE          ((uint32_t)100)  // 100 ms
#define RCC_DBP_TIMEOUT_VALUE      ((uint32_t)100)	// 100 ms

using namespace Eclo::STM32F4;

void RTC_::Native_RTC_SetAlarm( UINT8 param0, UINT8 param1, UINT8 param2, UINT8 param3, UINT8 param4, UINT8 param5, UINT8 param6, HRESULT &hr )
{
	// (++) To wake up from the Standby mode with an RTC alarm event, it is necessary to:
	// (+++) Enable the RTC Alarm Interrupt using the RTC_ITConfig() function
	// (+++) Configure the RTC to generate the RTC alarm using the RTC_SetAlarm() 
	// and RTC_AlarmCmd() functions.

	RTC_AlarmTypeDef rtc_AlarmStruct;
	
	rtc_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_None;
	rtc_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	
	rtc_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	rtc_AlarmStruct.RTC_AlarmTime.RTC_Hours =  param4;
	rtc_AlarmStruct.RTC_AlarmTime.RTC_Minutes =  param5;
	rtc_AlarmStruct.RTC_AlarmTime.RTC_Seconds =  param6;
	rtc_AlarmStruct.RTC_AlarmDateWeekDay = param2;

	// enable backup domain access 
	PWR_BackupAccessCmd(ENABLE);
	
	// need to disable alarm first
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE); 

	//Disable the RTC alarm interrupt for Alarm A (ALRAIE bit in the RTC_CR register)
	RTC->CR &= ((uint32_t)~(RTC_CR_ALRAIE));
	
	//Clear the RTC alarm (ALRAF/ALRBF) flag
	RTC->ISR &= ((uint32_t)~(RTC_ISR_ALRAF));
	
	// check if Alarm A register is accessible
	while(!(RTC->ISR & RTC_ISR_ALRAWF));
	
	// set alarm
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &rtc_AlarmStruct);
	
	// enable Alarm A
	if(RTC_AlarmCmd(RTC_Alarm_A, ENABLE) != SUCCESS)
	{
		hr = CLR_E_FAIL;
	}
	else
	{
		// enable interrupt for Alarm A
		RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	}
	
	// disable backup domain access 
	PWR_BackupAccessCmd(DISABLE);
}

