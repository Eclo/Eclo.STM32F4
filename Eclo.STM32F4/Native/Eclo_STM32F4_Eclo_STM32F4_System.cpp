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
#include "Eclo_STM32F4_Eclo_STM32F4_System.h"

using namespace Eclo::STM32F4;

BOOL HWSN_GetSerial(UINT8 data[]) {
	//hardware serial real size is 12 bytes
	//right packed
	memcpy((void*)&data[4], (const void*)0x1FFF7A10, 12);
	return TRUE;
}

void System::NativeGetHardwareSerial( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
	if (param0.GetSize() < 16) {
		hr=CLR_E_BUFFER_TOO_SMALL;
		return;
	}

	if(!HWSN_GetSerial(param0.GetBuffer()))
		hr = CLR_E_FAIL;
}

