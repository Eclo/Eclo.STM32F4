//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Eclo_STM32F4.h"
#include "Eclo_STM32F4_Eclo_STM32F4_RTC.h"

using namespace Eclo::STM32F4;


HRESULT Library_Eclo_STM32F4_Eclo_STM32F4_RTC::Native_RTC_SetAlarm___STATIC__VOID__U1__U1__U1__U1__U1__U1__U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 0, param0 ) );

        UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 1, param1 ) );

        UINT8 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 2, param2 ) );

        UINT8 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 3, param3 ) );

        UINT8 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 4, param4 ) );

        UINT8 param5;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 5, param5 ) );

        UINT8 param6;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 6, param6 ) );

        RTC_::Native_RTC_SetAlarm( param0, param1, param2, param3, param4, param5, param6, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}
