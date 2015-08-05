using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Eclo.STM32F4
{
    /// <summary>
    /// Real Time Clock features
    /// </summary>
    public static class RTC
    {
        /// <summary>
        /// Sets date & time for an alert.
        /// </summary>
        /// <param name="time">Time to set.</param>
        public static void SetAlarm(DateTime time)
        {
            Native_RTC_SetAlarm((byte)(time.Year - 2000), (byte)time.Month, (byte)time.Day, (byte)time.DayOfWeek, (byte)time.Hour, (byte)time.Minute, (byte)time.Second);
        }

        #region interop external methods declarations

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void Native_RTC_SetAlarm(byte year, byte month, byte day, byte dayOfWeek, byte hours, byte minutes, byte seconds);

        #endregion
    }
}
