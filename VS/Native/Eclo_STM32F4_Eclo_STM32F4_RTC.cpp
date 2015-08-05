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

void RTC_::Native_RTC_Init( UINT8 param0, INT8 param1, HRESULT &hr )
{
    UINT64 ticksStart;

	// enable power clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	// also on sleep mode
	RCC->APB1LPENR |= RCC_APB1LPENR_PWRLPEN;
		
	// Allow access to RTC 
	PWR_BackupAccessCmd(ENABLE);

	// clear BDRC
	RCC->BDCR = 0;
	
	// Wait for Backup domain Write protection disable 
    ticksStart = HAL_Time_CurrentTicks();
    
    while((PWR->CR & PWR_CR_DBP) == RESET)
    {
      if((HAL_Time_CurrentTicks()-ticksStart) > CPU_MillisecondsToTicks((UINT32)RCC_DBP_TIMEOUT_VALUE))
      {
        //HAL_TIMEOUT;
	  	hr = CLR_E_TIMEOUT;
		return;
      }      
    }
	
	if(param0 == 0)
	{
		///////////////////////////////////////////////
		// RTC clocked by external low-speed oscillator 
		
		// enable external low-speed oscillator
		// bypass LSE?
		if(param1 == 1)
		{
			RCC_LSEConfig(RCC_LSE_Bypass);			
		}
		else if(param1 == 0)
		{
			RCC_LSEConfig(RCC_LSE_ON);
		}
		else
		{
			// invalid option for LSE bypass
			hr = CLR_E_INVALID_PARAMETER;
			return;
		}
					
		// Get Start Tick
      	ticksStart = HAL_Time_CurrentTicks();

		// wait for external low-speed oscillator valid
	    while(!(RCC->BDCR & RCC_BDCR_LSERDY))
	    {
	      if((HAL_Time_CurrentTicks()-ticksStart) > CPU_MillisecondsToTicks((UINT32)RCC_LSE_TIMEOUT_VALUE))
	      {
			// external low-speed oscillator is not working or not fitted
			//HAL_TIMEOUT;
			hr = CLR_E_TIMEOUT;
			return;
	      }    
	    } 
			
		if(RCC->BDCR & RCC_BDCR_LSERDY)
		{
			// Select the RTC Clock Source
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		}
	}
	else if(param0 == 1)
	{
		////////////////////////////////////////////////
		// RTC clocked by internal low-speed internal RC 
		
		// enable internal low-power oscillator
		RCC_LSICmd(ENABLE);
		
		// Get Start Tick
		ticksStart = HAL_Time_CurrentTicks();
		    
		// Wait till LSI is ready
		while(!(RCC->CSR & RCC_CSR_LSIRDY))
		{
			if((HAL_Time_CurrentTicks()-ticksStart) > CPU_MillisecondsToTicks((UINT32)LSI_TIMEOUT_VALUE))
			{
				// external low-speed oscillator is not working
				//HAL_TIMEOUT;
				hr = CLR_E_TIMEOUT;
				return;
			} 
		}
		
		if(RCC->CSR & RCC_CSR_LSIRDY)
		{
			// Select the RTC Clock Source
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		}
	}
	else
	{
		// invalid option for low-speed oscillator
		hr = CLR_E_INVALID_PARAMETER;
	}
	
	// init RTC with default parameters
	RTC_InitTypeDef rtc_InitTypeDef;
	RTC_StructInit(&rtc_InitTypeDef);
	
	if(RTC_Init(&rtc_InitTypeDef) == ERROR) 
	{
		hr = CLR_E_INVALID_OPERATION;
    }
		
	// Enable the RTC Clock 
	RCC_RTCCLKCmd(ENABLE);
	
    PWR_BackupAccessCmd(DISABLE);
}

void RTC_::Native_RTC_GetDateTime( UINT8 * param0, UINT8 * param1, UINT8 * param2, UINT8 * param3, UINT8 * param4, UINT8 * param5, HRESULT &hr )
{
	// get date
	RTC_DateTypeDef rtc_DateStruct;
	
	RTC_GetDate(RTC_Format_BIN, &rtc_DateStruct);
	
	*param2 = rtc_DateStruct.RTC_Date;
	*param1 = rtc_DateStruct.RTC_Month;
	*param0 = rtc_DateStruct.RTC_Year;
	
	// get time
	RTC_TimeTypeDef rtc_TimeStruct;
			
	RTC_GetTime(RTC_Format_BIN, &rtc_TimeStruct);

	*param3 = rtc_TimeStruct.RTC_Hours;
	*param4 = rtc_TimeStruct.RTC_Minutes;
	*param5 = rtc_TimeStruct.RTC_Seconds;
}

void RTC_::Native_RTC_SetDateTime( UINT8 param0, UINT8 param1, UINT8 param2, UINT8 param3, UINT8 param4, UINT8 param5, UINT8 param6, HRESULT &hr )
{
	// set date
	RTC_DateTypeDef rtc_DateStruct;
	
	rtc_DateStruct.RTC_WeekDay = param3;
	rtc_DateStruct.RTC_Date = param2;
	rtc_DateStruct.RTC_Month = param1;
	rtc_DateStruct.RTC_Year = param0;

	// set time
	RTC_TimeTypeDef rtc_TimeStruct;
	
	rtc_TimeStruct.RTC_Hours = param4;
	rtc_TimeStruct.RTC_Minutes = param5;
	rtc_TimeStruct.RTC_Seconds = param6;

	PWR_BackupAccessCmd(ENABLE);
	
	if(RTC_SetTime(RTC_Format_BIN, &rtc_TimeStruct) != SUCCESS)
	{
		hr = CLR_E_FAIL;
	}	
	else
	{		
		if(RTC_SetDate(RTC_Format_BIN, &rtc_DateStruct) != SUCCESS)
		{
			hr = CLR_E_FAIL;
		}
	}	
	
	// Wait for RTC APB registers synchronisation
	//RTC_WaitForSynchro();
  
	PWR_BackupAccessCmd(DISABLE);
}

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
