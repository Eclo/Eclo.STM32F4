//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Eclo_Hardware.h"
#include "Eclo_Hardware_Eclo_Hardware_System.h"

using namespace Eclo::Hardware;


HRESULT Library_Eclo_Hardware_Eclo_Hardware_System::NativeGetHardwareSerial___STATIC__VOID__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 0, param0 ) );

        System::NativeGetHardwareSerial( param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}
