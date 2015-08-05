/* Includes ------------------------------------------------------------------*/
#include "..\..\Include\HelperFunctions.h"


/*-----------------------------------------------*/
// RCC 
/**
  * @brief  Configures the External Low Speed oscillator (LSE).
  * @note   As the LSE is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable write access using 
  *         PWR_BackupAccessCmd(ENABLE) function before to configure the LSE
  *         (to be done once after reset).  
  * @note   After enabling the LSE (RCC_LSE_ON or RCC_LSE_Bypass), the application
  *         software should wait on LSERDY flag to be set indicating that LSE clock
  *         is stable and can be used to clock the RTC.
  * @param  RCC_LSE: specifies the new state of the LSE.
  *          This parameter can be one of the following values:
  *            @arg RCC_LSE_OFF: turn OFF the LSE oscillator, LSERDY flag goes low after
  *                              6 LSE oscillator clock cycles.
  *            @arg RCC_LSE_ON: turn ON the LSE oscillator
  *            @arg RCC_LSE_Bypass: LSE oscillator bypassed with external clock
  * @retval None
  */
void RCC_LSEConfig(uint8_t RCC_LSE)
{
  /* Check the parameters */
  ////assert_param(IS_RCC_LSE(RCC_LSE));

  /* Reset LSEON and LSEBYP bits before configuring the LSE ------------------*/
  /* Reset LSEON bit */
  *(__IO uint8_t *) BDCR_ADDRESS = 0x00;

  /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) */
  switch (RCC_LSE)
  {
    case RCC_LSE_ON:
      /* Set LSEON bit */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
      break;
    case RCC_LSE_Bypass:
      /* Set LSEBYP and LSEON bits */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;
    default:
      break;
  }
}

/**
  * @brief  Checks whether the specified RCC flag is set or not.
  * @param  RCC_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *            @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
  *            @arg RCC_FLAG_PLLRDY: main PLL clock ready
  *            @arg RCC_FLAG_PLLI2SRDY: PLLI2S clock ready
  *            @arg RCC_FLAG_PLLSAIRDY: PLLSAI clock ready (only for STM32F42xxx/43xxx devices)
  *            @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *            @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  *            @arg RCC_FLAG_BORRST: POR/PDR or BOR reset
  *            @arg RCC_FLAG_PINRST: Pin reset
  *            @arg RCC_FLAG_PORRST: POR/PDR reset
  *            @arg RCC_FLAG_SFTRST: Software reset
  *            @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *            @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *            @arg RCC_FLAG_LPWRRST: Low Power reset
  * @retval The new state of RCC_FLAG (SET or RESET).
  */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
  uint32_t tmp = 0;
  uint32_t statusreg = 0;
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  ////assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* Get the RCC register index */
  tmp = RCC_FLAG >> 5;
  if (tmp == 1)               /* The flag to check is in CR register */
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* The flag to check is in BDCR register */
  {
    statusreg = RCC->BDCR;
  }
  else                       /* The flag to check is in CSR register */
  {
    statusreg = RCC->CSR;
  }

  /* Get the flag position */
  tmp = RCC_FLAG & FLAG_MASK;
  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the flag status */
  return bitstatus;
}

/**
  * @brief  Configures the RTC clock (RTCCLK).
  * @note   As the RTC clock configuration bits are in the Backup domain and write
  *         access is denied to this domain after reset, you have to enable write
  *         access using PWR_BackupAccessCmd(ENABLE) function before to configure
  *         the RTC clock source (to be done once after reset).    
  * @note   Once the RTC clock is configured it can't be changed unless the  
  *         Backup domain is reset using RCC_BackupResetCmd() function, or by
  *         a Power On Reset (POR).
  *    
  * @param  RCC_RTCCLKSource: specifies the RTC clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_RTCCLKSource_LSE: LSE selected as RTC clock
  *            @arg RCC_RTCCLKSource_LSI: LSI selected as RTC clock
  *            @arg RCC_RTCCLKSource_HSE_Divx: HSE clock divided by x selected
  *                                            as RTC clock, where x:[2,31]
  *  
  * @note   If the LSE or LSI is used as RTC clock source, the RTC continues to
  *         work in STOP and STANDBY modes, and can be used as wakeup source.
  *         However, when the HSE clock is used as RTC clock source, the RTC
  *         cannot be used in STOP and STANDBY modes.    
  * @note   The maximum input clock frequency for RTC is 1MHz (when using HSE as
  *         RTC clock source).
  *  
  * @retval None
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  ////assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));

  if ((RCC_RTCCLKSource & 0x00000300) == 0x00000300)
  { /* If HSE is selected as RTC clock source, configure HSE division factor for RTC clock */
    tmpreg = RCC->CFGR;

    /* Clear RTCPRE[4:0] bits */
    tmpreg &= ~RCC_CFGR_RTCPRE;

    /* Configure HSE division factor for RTC clock */
    tmpreg |= (RCC_RTCCLKSource & 0xFFFFCFF);

    /* Store the new value */
    RCC->CFGR = tmpreg;
  }
    
  /* Select the RTC clock source */
  RCC->BDCR |= (RCC_RTCCLKSource & 0x00000FFF);
}

/**
  * @brief  Enables or disables the RTC clock.
  * @note   This function must be used only after the RTC clock source was selected
  *         using the RCC_RTCCLKConfig function.
  * @param  NewState: new state of the RTC clock. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* Check the parameters */
  ////assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)NewState;
}

/**
  * @brief  Enables or disables the Internal Low Speed oscillator (LSI).
  * @note   After enabling the LSI, the application software should wait on 
  *         LSIRDY flag to be set indicating that LSI clock is stable and can
  *         be used to clock the IWDG and/or the RTC.
  * @note   LSI can not be disabled if the IWDG is running.  
  * @param  NewState: new state of the LSI.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   When the LSI is stopped, LSIRDY flag goes low after 6 LSI oscillator
  *         clock cycles. 
  * @retval None
  */
void RCC_LSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  ////assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)NewState;
}

/**
  * @brief  Enables or disables the Low Speed APB (APB1) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  * @param  RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg RCC_APB1Periph_TIM2:   TIM2 clock
  *            @arg RCC_APB1Periph_TIM3:   TIM3 clock
  *            @arg RCC_APB1Periph_TIM4:   TIM4 clock
  *            @arg RCC_APB1Periph_TIM5:   TIM5 clock
  *            @arg RCC_APB1Periph_TIM6:   TIM6 clock
  *            @arg RCC_APB1Periph_TIM7:   TIM7 clock
  *            @arg RCC_APB1Periph_TIM12:  TIM12 clock
  *            @arg RCC_APB1Periph_TIM13:  TIM13 clock
  *            @arg RCC_APB1Periph_TIM14:  TIM14 clock
  *            @arg RCC_APB1Periph_WWDG:   WWDG clock
  *            @arg RCC_APB1Periph_SPI2:   SPI2 clock
  *            @arg RCC_APB1Periph_SPI3:   SPI3 clock
  *            @arg RCC_APB1Periph_USART2: USART2 clock
  *            @arg RCC_APB1Periph_USART3: USART3 clock
  *            @arg RCC_APB1Periph_UART4:  UART4 clock
  *            @arg RCC_APB1Periph_UART5:  UART5 clock
  *            @arg RCC_APB1Periph_I2C1:   I2C1 clock
  *            @arg RCC_APB1Periph_I2C2:   I2C2 clock
  *            @arg RCC_APB1Periph_I2C3:   I2C3 clock
  *            @arg RCC_APB1Periph_CAN1:   CAN1 clock
  *            @arg RCC_APB1Periph_CAN2:   CAN2 clock
  *            @arg RCC_APB1Periph_PWR:    PWR clock
  *            @arg RCC_APB1Periph_DAC:    DAC clock
  *            @arg RCC_APB1Periph_UART7:  UART7 clock
  *            @arg RCC_APB1Periph_UART8:  UART8 clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* Check the parameters */
  //assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));  
  //assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->APB1ENR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1ENR &= ~RCC_APB1Periph;
  }
}
/*-----------------------------------------------*/

/*-----------------------------------------------*/
// RTC
static uint8_t RTC_ByteToBcd2(uint8_t Value);
static uint8_t RTC_Bcd2ToByte(uint8_t Value);

/**
  * @brief  Fills each RTC_InitStruct member with its default value.
  * @param  RTC_InitStruct: pointer to a RTC_InitTypeDef structure which will be 
  *         initialized.
  * @retval None
  */
void RTC_StructInit(RTC_InitTypeDef* RTC_InitStruct)
{
  /* Initialize the RTC_HourFormat member */
  RTC_InitStruct->RTC_HourFormat = RTC_HourFormat_24;
    
  /* Initialize the RTC_AsynchPrediv member */
  RTC_InitStruct->RTC_AsynchPrediv = (uint32_t)0x7F;

  /* Initialize the RTC_SynchPrediv member */
  RTC_InitStruct->RTC_SynchPrediv = (uint32_t)0xFF; 
}

/**
  * @brief  Initializes the RTC registers according to the specified parameters 
  *         in RTC_InitStruct.
  * @param  RTC_InitStruct: pointer to a RTC_InitTypeDef structure that contains 
  *         the configuration information for the RTC peripheral.
  * @note   The RTC Prescaler register is write protected and can be written in 
  *         initialization mode only.  
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC registers are initialized
  *          - ERROR: RTC registers are not initialized  
  */
ErrorStatus RTC_Init(RTC_InitTypeDef* RTC_InitStruct)
{
  ErrorStatus status = ERROR;
  
  /* Check the parameters */
  ////assert_param(IS_RTC_HOUR_FORMAT(RTC_InitStruct->RTC_HourFormat));
  ////assert_param(IS_RTC_ASYNCH_PREDIV(RTC_InitStruct->RTC_AsynchPrediv));
  ////assert_param(IS_RTC_SYNCH_PREDIV(RTC_InitStruct->RTC_SynchPrediv));

  /* Set Initialization mode */
  if (RTC_EnterInitMode() == ERROR)
  {
    status = ERROR;
  } 
  else
  {
    /* Clear RTC CR FMT Bit */
    RTC->CR &= ((uint32_t)~(RTC_CR_FMT));
    /* Set RTC_CR register */
    RTC->CR |=  ((uint32_t)(RTC_InitStruct->RTC_HourFormat));
  
    /* Configure the RTC PRER */
    RTC->PRER = (uint32_t)(RTC_InitStruct->RTC_SynchPrediv);
    RTC->PRER |= (uint32_t)(RTC_InitStruct->RTC_AsynchPrediv << 16);

    // RTC is always read directly from registers
    RTC->CR |= RTC_CR_BYPSHAD; 

    /* Exit Initialization mode */
    RTC_ExitInitMode();

    status = SUCCESS;    
  }
  
  return status;
}

/**
  * @brief  Get the RTC current date. 
  * @param  RTC_Format: specifies the format of the returned parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_Format_BIN: Binary data format 
  *            @arg RTC_Format_BCD: BCD data format
  * @param RTC_DateStruct: pointer to a RTC_DateTypeDef structure that will 
  *                        contain the returned current date configuration.     
  * @retval None
  */
void RTC_GetDate(uint32_t RTC_Format, RTC_DateTypeDef* RTC_DateStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  ////assert_param(IS_RTC_FORMAT(RTC_Format));
  
  /* Get the RTC_TR register */
  tmpreg = (uint32_t)(RTC->DR & RTC_DR_RESERVED_MASK); 

  /* Fill the structure fields with the read parameters */
  RTC_DateStruct->RTC_Year = (uint8_t)((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16);
  RTC_DateStruct->RTC_Month = (uint8_t)((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8);
  RTC_DateStruct->RTC_Date = (uint8_t)(tmpreg & (RTC_DR_DT | RTC_DR_DU));
  RTC_DateStruct->RTC_WeekDay = (uint8_t)((tmpreg & (RTC_DR_WDU)) >> 13);

  /* Check the input parameters format */
  if (RTC_Format == RTC_Format_BIN)
  {
    /* Convert the structure parameters to Binary format */
    RTC_DateStruct->RTC_Year = (uint8_t)RTC_Bcd2ToByte(RTC_DateStruct->RTC_Year);
    RTC_DateStruct->RTC_Month = (uint8_t)RTC_Bcd2ToByte(RTC_DateStruct->RTC_Month);
    RTC_DateStruct->RTC_Date = (uint8_t)RTC_Bcd2ToByte(RTC_DateStruct->RTC_Date);
  }
}

/**
  * @brief  Get the RTC current Time.
  * @param  RTC_Format: specifies the format of the returned parameters.
  *          This parameter can be  one of the following values:
  *            @arg RTC_Format_BIN:  Binary data format 
  *            @arg RTC_Format_BCD:  BCD data format
  * @param  RTC_TimeStruct: pointer to a RTC_TimeTypeDef structure that will 
  *                        contain the returned current time configuration.     
  * @retval None
  */
void RTC_GetTime(uint32_t RTC_Format, RTC_TimeTypeDef* RTC_TimeStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  ////assert_param(IS_RTC_FORMAT(RTC_Format));

  /* Get the RTC_TR register */
  tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK); 
  
  /* Fill the structure fields with the read parameters */
  RTC_TimeStruct->RTC_Hours = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16);
  RTC_TimeStruct->RTC_Minutes = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >>8);
  RTC_TimeStruct->RTC_Seconds = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));
  RTC_TimeStruct->RTC_H12 = (uint8_t)((tmpreg & (RTC_TR_PM)) >> 16);  

  /* Check the input parameters format */
  if (RTC_Format == RTC_Format_BIN)
  {
    /* Convert the structure parameters to Binary format */
    RTC_TimeStruct->RTC_Hours = (uint8_t)RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Hours);
    RTC_TimeStruct->RTC_Minutes = (uint8_t)RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Minutes);
    RTC_TimeStruct->RTC_Seconds = (uint8_t)RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Seconds);   
  }
}

/**
  * @brief  Set the RTC current date.
  * @param  RTC_Format: specifies the format of the entered parameters.
  *          This parameter can be  one of the following values:
  *            @arg RTC_Format_BIN:  Binary data format 
  *            @arg RTC_Format_BCD:  BCD data format
  * @param  RTC_DateStruct: pointer to a RTC_DateTypeDef structure that contains 
  *                         the date configuration information for the RTC.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC Date register is configured
  *          - ERROR: RTC Date register is not configured
  */
ErrorStatus RTC_SetDate(uint32_t RTC_Format, RTC_DateTypeDef* RTC_DateStruct)
{
  uint32_t tmpreg = 0;
  ErrorStatus status = ERROR;
  
  /* Check the parameters */
  ////assert_param(IS_RTC_FORMAT(RTC_Format));

  if ((RTC_Format == RTC_Format_BIN) && ((RTC_DateStruct->RTC_Month & 0x10) == 0x10))
  {
    RTC_DateStruct->RTC_Month = (RTC_DateStruct->RTC_Month & (uint32_t)~(0x10)) + 0x0A;
  }  
  if (RTC_Format == RTC_Format_BIN)
  {
    ////assert_param(IS_RTC_YEAR(RTC_DateStruct->RTC_Year));
    ////assert_param(IS_RTC_MONTH(RTC_DateStruct->RTC_Month));
    ////assert_param(IS_RTC_DATE(RTC_DateStruct->RTC_Date));
  }
  else
  {
    ////assert_param(IS_RTC_YEAR(RTC_Bcd2ToByte(RTC_DateStruct->RTC_Year)));
    tmpreg = RTC_Bcd2ToByte(RTC_DateStruct->RTC_Month);
    ////assert_param(IS_RTC_MONTH(tmpreg));
    tmpreg = RTC_Bcd2ToByte(RTC_DateStruct->RTC_Date);
    ////assert_param(IS_RTC_DATE(tmpreg));
  }
  ////assert_param(IS_RTC_WEEKDAY(RTC_DateStruct->RTC_WeekDay));

  /* Check the input parameters format */
  if (RTC_Format != RTC_Format_BIN)
  {
    tmpreg = ((((uint32_t)RTC_DateStruct->RTC_Year) << 16) | \
              (((uint32_t)RTC_DateStruct->RTC_Month) << 8) | \
              ((uint32_t)RTC_DateStruct->RTC_Date) | \
              (((uint32_t)RTC_DateStruct->RTC_WeekDay) << 13)); 
  }  
  else
  {
    tmpreg = (((uint32_t)RTC_ByteToBcd2(RTC_DateStruct->RTC_Year) << 16) | \
              ((uint32_t)RTC_ByteToBcd2(RTC_DateStruct->RTC_Month) << 8) | \
              ((uint32_t)RTC_ByteToBcd2(RTC_DateStruct->RTC_Date)) | \
              ((uint32_t)RTC_DateStruct->RTC_WeekDay << 13));
  }

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  /* Set Initialization mode */
  if (RTC_EnterInitMode() == ERROR)
  {
    status = ERROR;
  } 
  else
  {
    /* Set the RTC_DR register */
    RTC->DR = (uint32_t)(tmpreg & RTC_DR_RESERVED_MASK);

    /* Exit Initialization mode */
    RTC_ExitInitMode(); 

    /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if ((RTC->CR & RTC_CR_BYPSHAD) == RESET)
    {
		if(RTC_WaitForSynchro() == ERROR)
		{
		  status = ERROR;
		}
		else
		{
		  status = SUCCESS;
		}
	}
    else
    {
      status = SUCCESS;
    }
  }
  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF;   
  
  return status;
}

/**
  * @brief  Set the RTC current time.
  * @param  RTC_Format: specifies the format of the entered parameters.
  *          This parameter can be  one of the following values:
  *            @arg RTC_Format_BIN:  Binary data format 
  *            @arg RTC_Format_BCD:  BCD data format
  * @param  RTC_TimeStruct: pointer to a RTC_TimeTypeDef structure that contains 
  *                        the time configuration information for the RTC.     
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC Time register is configured
  *          - ERROR: RTC Time register is not configured
  */
ErrorStatus RTC_SetTime(uint32_t RTC_Format, RTC_TimeTypeDef* RTC_TimeStruct)
{
  uint32_t tmpreg = 0;
  ErrorStatus status = ERROR;
    
  /* Check the parameters */
  ////assert_param(IS_RTC_FORMAT(RTC_Format));
  
  if (RTC_Format == RTC_Format_BIN)
  {
    if ((RTC->CR & RTC_CR_FMT) != (uint32_t)RESET)
    {
      ////assert_param(IS_RTC_HOUR12(RTC_TimeStruct->RTC_Hours));
      ////assert_param(IS_RTC_H12(RTC_TimeStruct->RTC_H12));
    } 
    else
    {
      RTC_TimeStruct->RTC_H12 = 0x00;
      ////assert_param(IS_RTC_HOUR24(RTC_TimeStruct->RTC_Hours));
    }
    ////assert_param(IS_RTC_MINUTES(RTC_TimeStruct->RTC_Minutes));
    ////assert_param(IS_RTC_SECONDS(RTC_TimeStruct->RTC_Seconds));
  }
  else
  {
    if ((RTC->CR & RTC_CR_FMT) != (uint32_t)RESET)
    {
      tmpreg = RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Hours);
      ////assert_param(IS_RTC_HOUR12(tmpreg));
      ////assert_param(IS_RTC_H12(RTC_TimeStruct->RTC_H12)); 
    } 
    else
    {
      RTC_TimeStruct->RTC_H12 = 0x00;
      ////assert_param(IS_RTC_HOUR24(RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Hours)));
    }
    ////assert_param(IS_RTC_MINUTES(RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Minutes)));
    ////assert_param(IS_RTC_SECONDS(RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Seconds)));
  }
  
  /* Check the input parameters format */
  if (RTC_Format != RTC_Format_BIN)
  {
    tmpreg = (((uint32_t)(RTC_TimeStruct->RTC_Hours) << 16) | \
             ((uint32_t)(RTC_TimeStruct->RTC_Minutes) << 8) | \
             ((uint32_t)RTC_TimeStruct->RTC_Seconds) | \
             ((uint32_t)(RTC_TimeStruct->RTC_H12) << 16)); 
  }  
  else
  {
    tmpreg = (uint32_t)(((uint32_t)RTC_ByteToBcd2(RTC_TimeStruct->RTC_Hours) << 16) | \
                   ((uint32_t)RTC_ByteToBcd2(RTC_TimeStruct->RTC_Minutes) << 8) | \
                   ((uint32_t)RTC_ByteToBcd2(RTC_TimeStruct->RTC_Seconds)) | \
                   (((uint32_t)RTC_TimeStruct->RTC_H12) << 16));
  }  
  
  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  /* Set Initialization mode */
  if (RTC_EnterInitMode() == ERROR)
  {
    status = ERROR;
  } 
  else
  {       
    /* Set the RTC_TR register */
    RTC->TR = (uint32_t)(tmpreg & RTC_TR_RESERVED_MASK);

    /* Exit Initialization mode */
    RTC_ExitInitMode(); 

    /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
    if ((RTC->CR & RTC_CR_BYPSHAD) == RESET)
    {
  		if(RTC_WaitForSynchro() == ERROR)
  		{
  		  status = ERROR;
  		}
  		else
  		{
  		  status = SUCCESS;
  		}
	  }
    else
    {
      status = SUCCESS;
    }
  }
  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF; 
    
  return status;
}

/**
  * @brief  Enters the RTC Initialization mode.
  * @note   The RTC Initialization mode is write protected, use the 
  *         RTC_WriteProtectionCmd(DISABLE) before calling this function.    
  * @param  None
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC is in Init mode
  *          - ERROR: RTC is not in Init mode  
  */
ErrorStatus RTC_EnterInitMode(void)
{
  __IO uint32_t initcounter = 0x00;
  ErrorStatus status = ERROR;
  uint32_t initstatus = 0x00;
     
  /* Check if the Initialization mode is set */
  if ((RTC->ISR & RTC_ISR_INITF) == (uint32_t)RESET)
  {
    /* Set the Initialization mode */
    RTC->ISR = (uint32_t)RTC_INIT_MASK;
    
    /* Wait till RTC is in INIT state and if Time out is reached exit */
    do
    {
      initstatus = RTC->ISR & RTC_ISR_INITF;
      initcounter++;  
    } while((initcounter != INITMODE_TIMEOUT) && (initstatus == 0x00));
    //while((RTC->ISR & RTC_ISR_INITF) == (uint32_t)RESET);
    
    if ((RTC->ISR & RTC_ISR_INITF) != RESET)
    {
      status = SUCCESS;
    }
    else
    {
      status = ERROR;
    }        
  }
  else
  {
    status = SUCCESS;  
  } 
    
  return (status);  
}

/**
  * @brief  Exits the RTC Initialization mode.
  * @note   When the initialization sequence is complete, the calendar restarts 
  *         counting after 4 RTCCLK cycles.  
  * @note   The RTC Initialization mode is write protected, use the 
  *         RTC_WriteProtectionCmd(DISABLE) before calling this function.      
  * @param  None
  * @retval None
  */
void RTC_ExitInitMode(void)
{ 
  /* Exit Initialization mode */
  RTC->ISR &= (uint32_t)~RTC_ISR_INIT;  
}

/**
  * @brief  Waits until the RTC Time and Date registers (RTC_TR and RTC_DR) are 
  *         synchronized with RTC APB clock.
  * @note   The RTC Resynchronization mode is write protected, use the 
  *         RTC_WriteProtectionCmd(DISABLE) before calling this function. 
  * @note   To read the calendar through the shadow registers after Calendar 
  *         initialization, calendar update or after wakeup from low power modes 
  *         the software must first clear the RSF flag. 
  *         The software must then wait until it is set again before reading 
  *         the calendar, which means that the calendar registers have been 
  *         correctly copied into the RTC_TR and RTC_DR shadow registers.   
  * @param  None
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC registers are synchronised
  *          - ERROR: RTC registers are not synchronised
  */
ErrorStatus RTC_WaitForSynchro(void)
{
  __IO uint32_t synchrocounter = 0;
  ErrorStatus status = ERROR;
  uint32_t synchrostatus = 0x00;

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
    
  /* Clear RSF flag */
  RTC->ISR &= (uint32_t)RTC_RSF_MASK;
    
  /* Wait the registers to be synchronised */
  do
  {
    synchrostatus = RTC->ISR & RTC_ISR_RSF;
    synchrocounter++;  
  } while((synchrocounter != SYNCHRO_TIMEOUT) && (synchrostatus == 0x00));
    
  if ((RTC->ISR & RTC_ISR_RSF) != RESET)
  {
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }        

  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF; 
    
  return (status); 
}

/**
  * @brief  Enables or Disables the Bypass Shadow feature.
  * @note   When the Bypass Shadow is enabled the calendar value are taken 
  *         directly from the Calendar counter.
  * @param  NewState: new state of the Bypass Shadow feature.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
*/
void RTC_BypassShadowCmd(FunctionalState NewState)
{
  /* Check the parameters */
  ////assert_param(IS_FUNCTIONAL_STATE(NewState));

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
  
  if (NewState != DISABLE)
  {
    /* Set the BYPSHAD bit */
    RTC->CR |= (uint8_t)RTC_CR_BYPSHAD;
  }
  else
  {
    /* Reset the BYPSHAD bit */
    RTC->CR &= (uint8_t)~RTC_CR_BYPSHAD;
  }

  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF;
}

/**
  * @brief  Set the specified RTC Alarm.
  * @note   The Alarm register can only be written when the corresponding Alarm
  *         is disabled (Use the ////assert_param(DISABLE)).    
  * @param  RTC_Format: specifies the format of the returned parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_Format_BIN: Binary data format 
  *            @arg RTC_Format_BCD: BCD data format
  * @param  RTC_Alarm: specifies the alarm to be configured.
  *          This parameter can be one of the following values:
  *            @arg RTC_Alarm_A: to select Alarm A
  *            @arg RTC_Alarm_B: to select Alarm B  
  * @param  RTC_AlarmStruct: pointer to a RTC_AlarmTypeDef structure that 
  *                          contains the alarm configuration parameters.     
  * @retval None
  */
void RTC_SetAlarm(uint32_t RTC_Format, uint32_t RTC_Alarm, RTC_AlarmTypeDef* RTC_AlarmStruct)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  ////assert_param(IS_RTC_FORMAT(RTC_Format));
  ////assert_param(IS_RTC_ALARM(RTC_Alarm));
  ////assert_param(IS_ALARM_MASK(RTC_AlarmStruct->RTC_AlarmMask));
  ////assert_param(IS_RTC_ALARM_DATE_WEEKDAY_SEL(RTC_AlarmStruct->RTC_AlarmDateWeekDaySel));

  if (RTC_Format == RTC_Format_BIN)
  {
    if ((RTC->CR & RTC_CR_FMT) != (uint32_t)RESET)
    {
      ////assert_param(IS_RTC_HOUR12(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours));
      ////assert_param(IS_RTC_H12(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12));
    } 
    else
    {
      RTC_AlarmStruct->RTC_AlarmTime.RTC_H12 = 0x00;
      ////assert_param(IS_RTC_HOUR24(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours));
    }
    ////assert_param(IS_RTC_MINUTES(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes));
    ////assert_param(IS_RTC_SECONDS(RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds));
    
    if(RTC_AlarmStruct->RTC_AlarmDateWeekDaySel == RTC_AlarmDateWeekDaySel_Date)
    {
      ////assert_param(IS_RTC_ALARM_DATE_WEEKDAY_DATE(RTC_AlarmStruct->RTC_AlarmDateWeekDay));
    }
    else
    {
      ////assert_param(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(RTC_AlarmStruct->RTC_AlarmDateWeekDay));
    }
  }
  else
  {
    if ((RTC->CR & RTC_CR_FMT) != (uint32_t)RESET)
    {
      tmpreg = RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours);
      ////assert_param(IS_RTC_HOUR12(tmpreg));
      ////assert_param(IS_RTC_H12(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12));
    } 
    else
    {
      RTC_AlarmStruct->RTC_AlarmTime.RTC_H12 = 0x00;
      ////assert_param(IS_RTC_HOUR24(RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours)));
    }
    
    ////assert_param(IS_RTC_MINUTES(RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes)));
    ////assert_param(IS_RTC_SECONDS(RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds)));
    
    if(RTC_AlarmStruct->RTC_AlarmDateWeekDaySel == RTC_AlarmDateWeekDaySel_Date)
    {
      tmpreg = RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmDateWeekDay);
      ////assert_param(IS_RTC_ALARM_DATE_WEEKDAY_DATE(tmpreg));    
    }
    else
    {
      tmpreg = RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmDateWeekDay);
      ////assert_param(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(tmpreg));      
    }    
  }

  /* Check the input parameters format */
  if (RTC_Format != RTC_Format_BIN)
  {
    tmpreg = (((uint32_t)(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours) << 16) | \
              ((uint32_t)(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes) << 8) | \
              ((uint32_t)RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds) | \
              ((uint32_t)(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12) << 16) | \
              ((uint32_t)(RTC_AlarmStruct->RTC_AlarmDateWeekDay) << 24) | \
              ((uint32_t)RTC_AlarmStruct->RTC_AlarmDateWeekDaySel) | \
              ((uint32_t)RTC_AlarmStruct->RTC_AlarmMask)); 
  }  
  else
  {
    tmpreg = (((uint32_t)RTC_ByteToBcd2(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours) << 16) | \
              ((uint32_t)RTC_ByteToBcd2(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes) << 8) | \
              ((uint32_t)RTC_ByteToBcd2(RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds)) | \
              ((uint32_t)(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12) << 16) | \
              ((uint32_t)RTC_ByteToBcd2(RTC_AlarmStruct->RTC_AlarmDateWeekDay) << 24) | \
              ((uint32_t)RTC_AlarmStruct->RTC_AlarmDateWeekDaySel) | \
              ((uint32_t)RTC_AlarmStruct->RTC_AlarmMask)); 
  } 

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  /* Configure the Alarm register */
  if (RTC_Alarm == RTC_Alarm_A)
  {
    RTC->ALRMAR = (uint32_t)tmpreg;
  }
  else
  {
    RTC->ALRMBR = (uint32_t)tmpreg;
  }

  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF;   
}

/**
  * @brief  Fills each RTC_AlarmStruct member with its default value
  *         (Time = 00h:00mn:00sec / Date = 1st day of the month/Mask =
  *         all fields are masked).
  * @param  RTC_AlarmStruct: pointer to a @ref RTC_AlarmTypeDef structure which
  *         will be initialized.
  * @retval None
  */
void RTC_AlarmStructInit(RTC_AlarmTypeDef* RTC_AlarmStruct)
{
  /* Alarm Time Settings : Time = 00h:00mn:00sec */
  RTC_AlarmStruct->RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours = 0;
  RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes = 0;
  RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds = 0;

  /* Alarm Date Settings : Date = 1st day of the month */
  RTC_AlarmStruct->RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStruct->RTC_AlarmDateWeekDay = 1;

  /* Alarm Masks Settings : Mask =  all fields are not masked */
  RTC_AlarmStruct->RTC_AlarmMask = RTC_AlarmMask_None;
}

/**
  * @brief  Enables or disables the specified RTC interrupts.
  * @param  RTC_IT: specifies the RTC interrupt sources to be enabled or disabled. 
  *          This parameter can be any combination of the following values:
  *            @arg RTC_IT_TS:  Time Stamp interrupt mask
  *            @arg RTC_IT_WUT:  WakeUp Timer interrupt mask
  *            @arg RTC_IT_ALRB:  Alarm B interrupt mask
  *            @arg RTC_IT_ALRA:  Alarm A interrupt mask
  *            @arg RTC_IT_TAMP: Tamper event interrupt mask
  * @param  NewState: new state of the specified RTC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_ITConfig(uint32_t RTC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  //assert_param(IS_RTC_CONFIG_IT(RTC_IT));
  //assert_param(IS_FUNCTIONAL_STATE(NewState));

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  if (NewState != DISABLE)
  {
    /* Configure the Interrupts in the RTC_CR register */
    RTC->CR |= (uint32_t)(RTC_IT & ~RTC_TAFCR_TAMPIE);
    /* Configure the Tamper Interrupt in the RTC_TAFCR */
    RTC->TAFCR |= (uint32_t)(RTC_IT & RTC_TAFCR_TAMPIE);
  }
  else
  {
    /* Configure the Interrupts in the RTC_CR register */
    RTC->CR &= (uint32_t)~(RTC_IT & (uint32_t)~RTC_TAFCR_TAMPIE);
    /* Configure the Tamper Interrupt in the RTC_TAFCR */
    RTC->TAFCR &= (uint32_t)~(RTC_IT & RTC_TAFCR_TAMPIE);
  }
  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF; 
}

/**
  * @brief  Enables or disables the specified RTC Alarm.
  * @param  RTC_Alarm: specifies the alarm to be configured.
  *   This parameter can be any combination of the following values:
  *     @arg RTC_Alarm_A: to select Alarm A
  *     @arg RTC_Alarm_B: to select Alarm B  
  * @param  NewState: new state of the specified alarm.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RTC Alarm is enabled/disabled
  *          - ERROR: RTC Alarm is not enabled/disabled  
  */
ErrorStatus RTC_AlarmCmd(uint32_t RTC_Alarm, FunctionalState NewState)
{
  __IO uint32_t alarmcounter = 0x00;
  uint32_t alarmstatus = 0x00;
  ErrorStatus status = ERROR;
    
  /* Check the parameters */
  //assert_param(IS_RTC_CMD_ALARM(RTC_Alarm));
  //assert_param(IS_FUNCTIONAL_STATE(NewState));

  /* Disable the write protection for RTC registers */
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  /* Configure the Alarm state */
  if (NewState != DISABLE)
  {
    RTC->CR |= (uint32_t)RTC_Alarm;

    status = SUCCESS;    
  }
  else
  { 
    /* Disable the Alarm in RTC_CR register */
    RTC->CR &= (uint32_t)~RTC_Alarm;
   
    /* Wait till RTC ALRxWF flag is set and if Time out is reached exit */
    do
    {
      alarmstatus = RTC->ISR & (RTC_Alarm >> 8);
      alarmcounter++;  
    } while((alarmcounter != INITMODE_TIMEOUT) && (alarmstatus == 0x00));
    
    if ((RTC->ISR & (RTC_Alarm >> 8)) == RESET)
    {
      status = ERROR;
    } 
    else
    {
      status = SUCCESS;
    }        
  } 

  /* Enable the write protection for RTC registers */
  RTC->WPR = 0xFF; 
  
  return status;
}

/**
  * @brief  Converts a 2 digit decimal to BCD format.
  * @param  Value: Byte to be converted.
  * @retval Converted byte
  */
static uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint8_t bcdhigh = 0;
  
  while (Value >= 10)
  {
    bcdhigh++;
    Value -= 10;
  }
  
  return  ((uint8_t)(bcdhigh << 4) | Value);
}

/**
  * @brief  Convert from 2 digit BCD to Binary.
  * @param  Value: BCD value to be converted.
  * @retval Converted word
  */
static uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint8_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

/*-----------------------------------------------*/

/*-----------------------------------------------*/
// GPIO


/*-----------------------------------------------*/

/*-----------------------------------------------*/
// PWR

/**
  * @brief  Enables or disables access to the backup domain (RTC registers, RTC 
  *         backup data registers and backup SRAM).
  * @note   If the HSE divided by 2, 3, ..31 is used as the RTC clock, the 
  *         Backup Domain Access should be kept enabled.
  * @param  NewState: new state of the access to the backup domain.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_BackupAccessCmd(FunctionalState NewState)
{
  /* Check the parameters */
  ////////assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)NewState;
}


/*-----------------------------------------------*/


/*-----------------------------------------------*/
// EXTI


/*-----------------------------------------------*/
