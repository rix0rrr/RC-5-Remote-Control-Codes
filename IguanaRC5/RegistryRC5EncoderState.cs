using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;

namespace IguanaRC5
{
    /// <summary>
    /// Class that stores the encoder state in the registry
    /// </summary>
    internal class RegistryRC5EncoderState : IRC5EncoderState
    {
        const string KeyName = @"Software\IguanaRC5";

        public bool ToggleBit
        {
            get
            {
                using (var key = Registry.CurrentUser.OpenSubKey(KeyName))
                {
                    try
                    {
                        if (key == null) return false;

                        return (int)key.GetValue("ToggleBit", 0) == 1;
                    }
                    catch (InvalidCastException)
                    {
                        return false;
                    }
                }
            }
            set
            {
                using (var key = Registry.CurrentUser.CreateSubKey(KeyName))
                {
                    if (key == null) throw new AccessViolationException("Can't write to registry");

                    key.SetValue("ToggleBit", value ? 1 : 0);
                }
            }
        }
    }
}
