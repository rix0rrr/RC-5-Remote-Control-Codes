using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IguanaRC5
{
    /// <summary>
    /// Class for managing the encoder's state
    /// </summary>
    interface IRC5EncoderState
    {
        /// <summary>
        /// The state of the toggle bit
        /// </summary>
        /// <remarks>
        /// Every new non-repeated word toggles this bit
        /// </remarks>
        bool ToggleBit { get; set; }
    }
}
