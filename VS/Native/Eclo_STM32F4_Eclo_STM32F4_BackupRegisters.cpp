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
#include "Eclo_STM32F4_Eclo_STM32F4_BackupRegisters.h"

#include "..\..\Include\helperfunctions.h"

using namespace Eclo::STM32F4;

void BackupRegisters::Native_BackupRegister_WriteRegister( UINT8 param0, INT32 param1, HRESULT &hr )
{
	// base address is RTC->BKP0R for all SMT32F4 variants: 0x40002850
	volatile uint32_t* base_addr = &(RTC->BKP0R);

    // enable PWR APB1 Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   

	// enable access to backup domain
	PWR_BackupAccessCmd(ENABLE);
  
	// write data to register
	// addresses are offset from first register address (BKP0R)
	*(base_addr + param0) = param1;
  
	// disable access to backup domain
	PWR_BackupAccessCmd(DISABLE);
}

void BackupRegisters::Native_BackupRegister_ReadRegister( UINT8 param0, INT32 * param1, HRESULT &hr )
{
	volatile uint32_t* base_addr = &(RTC->BKP0R);

	// addresses are offset by from first register address (BKP0R)
	*param1 = *(base_addr + param0);
}

