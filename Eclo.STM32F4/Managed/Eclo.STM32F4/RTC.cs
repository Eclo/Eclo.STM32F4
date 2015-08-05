using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Eclo.STM32F4
{
    /// <summary>
    /// Real Time Clock functionality
    /// </summary>
    public static class RTC
    {
        /// <summary>
        /// Option for low speed oscilator
        /// </summary>
        public enum LowSpeedOscilator
        {
            /// <summary>
            /// Internal low-speed internal RC (LSI) 
            /// </summary>
            Internal,
            /// <summary>
            /// External low-speed oscillator (LSE)
            /// </summary>
            External
        }

        /// <summary>
        /// Initializes the Real Time Clock
        /// </summary>
        /// <param name="oscilator">Option for low speed oscilator (default is internal RC oscilator)</param>
        /// <param name="bypassOscilator">bypass internal oscilator when using external oscilator with drive circuit</param>
        public static void Initialize(LowSpeedOscilator oscilator = LowSpeedOscilator.Internal, bool bypassOscilator = false)
        {
            Native_RTC_Init((byte)oscilator, bypassOscilator);
        }

        /// <summary>
        /// Returns the RTC date & time
        /// </summary>
        /// <returns>Time</returns>
        public static DateTime GetDateTime()
        {
            byte hours, minutes, seconds;
            byte year, month, day;

            Native_RTC_GetDateTime(out year, out month, out day, out hours, out minutes, out seconds);

            return new DateTime((year + 2000), month, day, hours, minutes, seconds);
        }

        /// <summary>
        /// Sets the RTC date & time.
        /// </summary>
        /// <param name="time">Time to set.</param>
        public static void SetDateTime(DateTime time)
        {
            Native_RTC_SetDateTime(
                // must send only tens of year
                (byte)(time.Year - 2000), 
                (byte)time.Month, 
                (byte)time.Day, 
                // Sunday is 0 in .NETMF but needs to be 7 for STM32F4 RTC register
                (time.DayOfWeek != DayOfWeek.Sunday ? (byte)time.DayOfWeek : (byte)7), 
                (byte)time.Hour, 
                (byte)time.Minute, 
                (byte)time.Second);
        }

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
        private static extern void Native_RTC_Init(byte oscilatorOption, bool bypassOscilator);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void Native_RTC_GetDateTime(out byte year, out byte month, out byte day, out byte hours, out byte minutes, out byte seconds);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void Native_RTC_SetDateTime(byte year, byte month, byte day, byte dayOfWeek, byte hours, byte minutes, byte seconds);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void Native_RTC_SetAlarm(byte year, byte month, byte day, byte dayOfWeek, byte hours, byte minutes, byte seconds);

        #endregion
    }
}
