using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Eclo.STM32F4
{
    public class System
    {
        private static byte[] _hardwareSerial;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void NativeGetHardwareSerial(byte[] data);

        /// <summary>
        /// Gets the hardware unique serial ID (16 bytes)
        /// </summary>
        public static byte[] HardwareSerial
        {
            get
            {
                if (_hardwareSerial == null)
                {
                    _hardwareSerial = new byte[16];
                    NativeGetHardwareSerial(_hardwareSerial);
                }
                return _hardwareSerial;
            }
        }
    }
}
