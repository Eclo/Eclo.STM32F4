using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Eclo.STM32F4
{
    /// <summary>
    /// Provides access to the RTC backup registers
    /// </summary>
    public class BackupRegisters
    {
        /// <summary>
        /// Available backup registers
        /// </summary>
        public enum Registers
        {
            Register_00 = 0,
            Register_01 = 1,
            Register_02 = 2,
            Register_03 = 3,
            Register_04 = 4,
            Register_05 = 5,
            Register_06 = 6,
            Register_07 = 7,
            Register_08 = 8,
            Register_09 = 9,
            Register_10 = 10,
            Register_11 = 11,
            Register_12 = 12,
            Register_13 = 13,
            Register_14 = 14,
            Register_15 = 15,
            Register_16 = 16,
            Register_17 = 17,
            Register_18 = 18,
            Register_19 = 19
        }

        /// <summary>
        /// Write data in backup register
        /// </summary>
        public static void WriteRegister(Registers register, int data)
        {
            Native_BackupRegister_WriteRegister((byte)register, data);
        }

        /// <summary>
        /// Read data from backup register
        /// </summary>
        public static int ReadRegister(Registers register)
        {
            int registerData;
            Native_BackupRegister_ReadRegister((byte)register, out registerData);

            return registerData;
        }

        #region interop external methods declarations

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void Native_BackupRegister_WriteRegister(byte register, int data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void Native_BackupRegister_ReadRegister(byte register, out int data);

        #endregion
    }
}
