//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Eclo_STM32F4.h"
#include "Eclo_STM32F4_Eclo_STM32F4_BackupRegisters.h"

using namespace Eclo::STM32F4;


HRESULT Library_Eclo_STM32F4_Eclo_STM32F4_BackupRegisters::Native_BackupRegister_WriteRegister___STATIC__VOID__U1__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        BackupRegisters::Native_BackupRegister_WriteRegister( param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Eclo_STM32F4_Eclo_STM32F4_BackupRegisters::Native_BackupRegister_ReadRegister___STATIC__VOID__U1__BYREF_I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 0, param0 ) );

        INT32 * param1;
        UINT8 heapblock1[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32_ByRef( stack, heapblock1, 1, param1 ) );

        BackupRegisters::Native_BackupRegister_ReadRegister( param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock1, 1 ) );
    }
    TINYCLR_NOCLEANUP();
}
